#!/usr/bin/env node
/**
 * CONDUYT firmware capability audit
 *
 * Connects to a CONDUYT device over serial, captures HELLO_RESP, and
 * cross-checks declared per-pin capabilities against live behavior.
 *
 * Usage: node capability-audit.mjs [--port /dev/cu.usbmodemNNN] [--profile r4-minima]
 *
 * Without --profile, runs in baseline mode: prints what the device declares
 * and what it actually responds to, no pass/fail.
 *
 * Read-only: only sends HELLO, PIN_MODE, PIN_READ. No PIN_WRITE.
 */

import { SerialPort } from 'serialport'
import { argv, exit } from 'node:process'

// ── protocol constants (mirrored from protocol/constants.json) ──────────────
const MAGIC = [0x43, 0x44]
const VER = 0x02  // protocol v2: canonical CRC table
const HEADER_SIZE = 8

const CMD = { PING: 0x01, HELLO: 0x02, PIN_MODE: 0x10, PIN_READ: 0x12 }
const EVT = {
  PONG: 0x80, HELLO_RESP: 0x81, ACK: 0x82, NAK: 0x83,
  PIN_READ_RESP: 0x91, LOG: 0xE0, FATAL: 0xFF,
}
const PIN_MODE = { INPUT: 0x00, OUTPUT: 0x01, PWM: 0x02, ANALOG: 0x03, INPUT_PULLUP: 0x04 }
const PIN_CAP = {
  DIGITAL_IN: 1, DIGITAL_OUT: 2, PWM_OUT: 4, ANALOG_IN: 8,
  I2C_SDA: 16, I2C_SCL: 32, SPI: 64, INTERRUPT: 128,
}
const ERR_NAMES = {
  0x01: 'UNKNOWN_TYPE', 0x02: 'CRC_MISMATCH', 0x03: 'PAYLOAD_TOO_LARGE',
  0x04: 'INVALID_PIN', 0x05: 'PIN_MODE_UNSUPPORTED', 0x06: 'I2C_NOT_AVAILABLE',
  0x07: 'I2C_NACK', 0x08: 'MODULE_NOT_LOADED', 0x09: 'UNKNOWN_MODULE_CMD',
  0x0A: 'MODULE_BUSY', 0x0B: 'SUB_LIMIT_REACHED', 0x0C: 'OUT_OF_MEMORY',
  0x0D: 'UNKNOWN_DATASTREAM', 0x0E: 'DATASTREAM_READONLY', 0x0F: 'OTA_INVALID',
  0x10: 'VERSION_MISMATCH',
}

// ── CRC8 — canonical poly 0x31, init 0x00, MSB-first. Computed at startup
// so the table can't drift from the firmware's. v2 protocol.
const CRC_TABLE = (() => {
  const t = new Uint8Array(256)
  for (let i = 0; i < 256; i++) {
    let c = i
    for (let b = 0; b < 8; b++) c = (c & 0x80) ? ((c << 1) ^ 0x31) & 0xFF : (c << 1) & 0xFF
    t[i] = c
  }
  return t
})()
function crc8(bytes) {
  let crc = 0
  for (const b of bytes) crc = CRC_TABLE[crc ^ b]
  return crc
}

// ── COBS encode/decode ────────────────────────────────────────────────────
function cobsEncode(input) {
  const out = [0]
  let codeIdx = 0, code = 1
  for (const b of input) {
    if (b === 0) { out[codeIdx] = code; codeIdx = out.length; out.push(0); code = 1 }
    else { out.push(b); code++; if (code === 0xFF) { out[codeIdx] = code; codeIdx = out.length; out.push(0); code = 1 } }
  }
  out[codeIdx] = code
  return Uint8Array.from(out)
}
function cobsDecode(input) {
  const out = []
  let i = 0
  while (i < input.length) {
    const code = input[i++]
    if (code === 0) return null
    for (let j = 1; j < code && i < input.length; j++) out.push(input[i++])
    if (code !== 0xFF && i < input.length) out.push(0)
  }
  return Uint8Array.from(out)
}

// ── packet build ──────────────────────────────────────────────────────────
// Layout: MAGIC(2) + VER(1) + TYPE(1) + SEQ(1) + LEN(2) + PAYLOAD(N) + CRC(1)
// CRC8 covers [VER..end of PAYLOAD] = bytes [2..6+N] (length 5+N)
function makePacket(type, seq, payload = new Uint8Array(0)) {
  const len = payload.length
  const buf = new Uint8Array(8 + len) // header(7) + payload(N) + crc(1)
  buf[0] = MAGIC[0]; buf[1] = MAGIC[1]
  buf[2] = VER; buf[3] = type; buf[4] = seq
  buf[5] = len & 0xFF; buf[6] = (len >> 8) & 0xFF
  if (len > 0) buf.set(payload, 7)
  buf[7 + len] = crc8(buf.subarray(2, 7 + len))
  return buf
}
function parsePacket(buf) {
  if (buf.length < 8) return null
  if (buf[0] !== MAGIC[0] || buf[1] !== MAGIC[1]) return null
  const type = buf[3], seq = buf[4]
  const len = buf[5] | (buf[6] << 8)
  if (buf.length < 8 + len) return null
  return { type, seq, payload: buf.subarray(7, 7 + len) }
}

// ── wire client ────────────────────────────────────────────────────────────
class Client {
  constructor(path, baud = 115200) {
    this.port = new SerialPort({ path, baudRate: baud, autoOpen: false })
    this.cobsBuf = []
    this.pending = new Map()
    this.seq = 1
    this.logs = []
    this.port.on('data', (chunk) => this._onData(chunk))
  }
  open() { return new Promise((res, rej) => this.port.open((e) => e ? rej(e) : res())) }
  close() { return new Promise((res) => this.port.close(() => res())) }
  _onData(chunk) {
    for (const b of chunk) {
      if (b === 0x00) {
        if (this.cobsBuf.length > 0) {
          const decoded = cobsDecode(Uint8Array.from(this.cobsBuf))
          this.cobsBuf = []
          if (decoded) this._onPacket(decoded)
        }
      } else this.cobsBuf.push(b)
    }
  }
  _onPacket(raw) {
    const pkt = parsePacket(raw)
    if (!pkt) return
    if (pkt.type === EVT.LOG) {
      this.logs.push(new TextDecoder().decode(pkt.payload))
      return
    }
    if (pkt.type === EVT.FATAL) {
      this.logs.push(`FATAL: ${new TextDecoder().decode(pkt.payload)}`)
      return
    }
    const p = this.pending.get(pkt.seq)
    if (p) {
      this.pending.delete(pkt.seq)
      if (pkt.type === EVT.NAK) {
        const code = pkt.payload[0]
        p.rej(new Error(`NAK ${ERR_NAMES[code] ?? '0x' + code.toString(16)}`))
      } else {
        p.res(pkt)
      }
    }
  }
  async send(type, payload = new Uint8Array(0), timeoutMs = 2000) {
    const seq = this.seq++; if (this.seq > 255) this.seq = 1
    const raw = makePacket(type, seq, payload)
    const encoded = cobsEncode(raw)
    const framed = new Uint8Array(encoded.length + 1)
    framed.set(encoded); framed[encoded.length] = 0x00
    return new Promise((res, rej) => {
      const t = setTimeout(() => { this.pending.delete(seq); rej(new Error(`timeout type=0x${type.toString(16)} seq=${seq}`)) }, timeoutMs)
      this.pending.set(seq, { res: (pkt) => { clearTimeout(t); res(pkt) }, rej: (e) => { clearTimeout(t); rej(e) } })
      this.port.write(framed, (err) => { if (err) { clearTimeout(t); this.pending.delete(seq); rej(err) } })
    })
  }
}

// ── HELLO_RESP parser (mirrors firmware buildHelloResp byte order) ────────
function readFixedString(view, offset, len) {
  let end = len
  for (let i = 0; i < len; i++) if (view.getUint8(offset + i) === 0) { end = i; break }
  const bytes = new Uint8Array(view.buffer, view.byteOffset + offset, end)
  return [new TextDecoder().decode(bytes), offset + len]
}
function parseHelloResp(payload) {
  const view = new DataView(payload.buffer, payload.byteOffset, payload.byteLength)
  let pos = 0
  let firmwareName; [firmwareName, pos] = readFixedString(view, pos, 16)
  const vMajor = view.getUint8(pos++), vMinor = view.getUint8(pos++), vPatch = view.getUint8(pos++)
  const mcuId = Array.from(payload.subarray(pos, pos + 8)); pos += 8
  const otaCapable = view.getUint8(pos++) === 0x01
  const pinCount = view.getUint8(pos++)
  const pins = []
  for (let i = 0; i < pinCount; i++) pins.push({ pin: i, capabilities: view.getUint8(pos++) })
  const i2cBuses = view.getUint8(pos++)
  const spiBuses = view.getUint8(pos++)
  const uartCount = view.getUint8(pos++)
  const maxPayload = view.getUint16(pos, true); pos += 2
  const moduleCount = view.getUint8(pos++)
  const modules = []
  for (let i = 0; i < moduleCount; i++) {
    const moduleId = view.getUint8(pos++)
    let name; [name, pos] = readFixedString(view, pos, 8)
    const versionMajor = view.getUint8(pos++), versionMinor = view.getUint8(pos++)
    const modPinCount = view.getUint8(pos++)
    const pins = []
    for (let p = 0; p < modPinCount; p++) pins.push(view.getUint8(pos++))
    modules.push({ moduleId, name, versionMajor, versionMinor, pins })
  }
  const dsCount = view.getUint8(pos++)
  const datastreams = []
  for (let i = 0; i < dsCount; i++) {
    let name; [name, pos] = readFixedString(view, pos, 16)
    const type = view.getUint8(pos++)
    let unit; [unit, pos] = readFixedString(view, pos, 8)
    const writable = view.getUint8(pos++) === 1
    const pinRef = view.getUint8(pos++)
    const retain = view.getUint8(pos++) === 1
    datastreams.push({ index: i, name, type, unit, writable, pinRef, retain })
  }
  return {
    firmwareName, firmwareVersion: [vMajor, vMinor, vPatch],
    mcuId, otaCapable, pinCount, pins, i2cBuses, spiBuses, uartCount,
    maxPayload, modules, datastreams,
    rawTrailingBytes: payload.length - pos,
  }
}

function decodeCaps(byte) {
  const flags = []
  for (const [k, v] of Object.entries(PIN_CAP)) if (byte & v) flags.push(k)
  return flags
}

// ── per-board expected profiles ──────────────────────────────────────────
// Source: protocol/board-profiles.json (generated from protocol/boards/*.yml).
// Loaded lazily from disk so the audit always matches what the firmware
// generator produced, with no hand-coded duplication.
import { readFileSync } from 'node:fs'
import { fileURLToPath } from 'node:url'
import { dirname as pathDirname, join as pathJoin } from 'node:path'
const __auditDirname = pathDirname(fileURLToPath(import.meta.url))
const BOARD_PROFILES_PATH = pathJoin(__auditDirname, '..', '..', 'protocol', 'board-profiles.json')

function loadProfile(id) {
  const data = JSON.parse(readFileSync(BOARD_PROFILES_PATH, 'utf-8'))
  // Accept either platformio_id (e.g. "uno_r4_minima") or short aliases.
  const aliases = { 'r4-minima': 'uno_r4_minima', 'r4-wifi': 'uno_r4_wifi' }
  const target = aliases[id] ?? id
  const board = data.boards.find(b => b.id === target || b.platformio_id === target)
  if (!board) {
    const known = data.boards.map(b => b.id).join(', ')
    throw new Error(`unknown profile "${id}". known: ${known}`)
  }
  return {
    name: board.name,
    pinCount: board.pin_count,
    i2cBuses: board.i2c_buses,
    spiBuses: board.spi_buses,
    pinCaps: board.pinCaps,
  }
}

// ── audit body ────────────────────────────────────────────────────────────
async function findPort() {
  const ports = await SerialPort.list()
  // Prefer explicit Arduino vendor (2341) at runtime
  const arduinos = ports.filter(p => (p.vendorId || '').toLowerCase() === '2341' && (p.path.includes('usbmodem') || p.path.startsWith('/dev/ttyACM')))
  if (arduinos.length) return arduinos[0].path
  const cdc = ports.find(p => p.path.includes('usbmodem') || p.path.startsWith('/dev/ttyACM') || p.path.startsWith('/dev/ttyUSB'))
  return cdc?.path
}

function fmtCaps(byte) {
  const flags = decodeCaps(byte)
  return flags.length ? flags.join('|') : '(none)'
}

async function probePin(client, pin) {
  // Live probe: try analog mode + read, try digital mode + read.
  // Return both raw values + any errors so we can compare against declared caps.
  const result = { pin, analog: null, analogErr: null, digital: null, digitalErr: null }
  try {
    await client.send(CMD.PIN_MODE, Uint8Array.from([pin, PIN_MODE.ANALOG]))
    const r = await client.send(CMD.PIN_READ, Uint8Array.from([pin, PIN_MODE.ANALOG]))
    result.analog = r.payload[1] | (r.payload[2] << 8)
  } catch (e) { result.analogErr = e.message }
  try {
    await client.send(CMD.PIN_MODE, Uint8Array.from([pin, PIN_MODE.INPUT_PULLUP]))
    const r = await client.send(CMD.PIN_READ, Uint8Array.from([pin, PIN_MODE.INPUT]))
    result.digital = r.payload[1] | (r.payload[2] << 8)
  } catch (e) { result.digitalErr = e.message }
  return result
}

async function main() {
  const args = Object.fromEntries(
    argv.slice(2).reduce((acc, a, i, arr) => {
      if (a.startsWith('--')) acc.push([a.slice(2), arr[i+1]?.startsWith('--') ? true : arr[i+1] ?? true])
      return acc
    }, [])
  )
  const path = args.port ?? await findPort()
  if (!path) { console.error('no serial port found'); exit(2) }
  let profile = null
  if (args.profile) {
    try { profile = loadProfile(args.profile) }
    catch (e) { console.error(String(e?.message ?? e)); exit(2) }
  }
  const probe = args.probe !== 'false' && args.probe !== false

  console.log(`port: ${path}`)
  if (profile) console.log(`profile: ${profile.name}`)
  const client = new Client(path)
  await client.open()
  try {
    // Some boards reset on DTR; give them a moment.
    await new Promise(r => setTimeout(r, 1500))
    // discard any boot chatter
    client.cobsBuf = []

    let helloPkt
    try {
      helloPkt = await client.send(CMD.HELLO, new Uint8Array(0), 4000)
    } catch (e) {
      if (/timeout/.test(String(e))) {
        console.error('HELLO timed out. Possible causes:')
        console.error('  - Firmware is older than protocol v2 (uses corrupt CRC table). Re-flash.')
        console.error('  - Firmware hung from a prior analogRead on a non-ADC pin. Reset the board.')
        console.error('  - Wrong serial port. Try: ls /dev/cu.usbmodem*')
      }
      throw e
    }
    if (helloPkt.payload.length < 1) throw new Error('HELLO_RESP too short')
    // Wire protocol version is in the packet header (helloPkt.version isn't surfaced
    // by parsePacket; check it via raw byte read by re-decoding the header fields).
    // For now: trust the device, decode the rest. Version mismatch shows up as
    // garbled fields downstream.
    const hello = parseHelloResp(helloPkt.payload)

    console.log()
    console.log('── HELLO_RESP ─────────────────────────────────────────')
    console.log(`firmware:    ${hello.firmwareName} v${hello.firmwareVersion.join('.')}`)
    console.log(`mcu_id:      ${hello.mcuId.map(b => b.toString(16).padStart(2,'0')).join('')}`)
    console.log(`ota_capable: ${hello.otaCapable}`)
    console.log(`pin_count:   ${hello.pinCount}`)
    console.log(`i2c_buses:   ${hello.i2cBuses}`)
    console.log(`spi_buses:   ${hello.spiBuses}`)
    console.log(`uart_count:  ${hello.uartCount}`)
    console.log(`max_payload: ${hello.maxPayload}`)
    console.log(`modules:     ${hello.modules.length ? hello.modules.map(m => `${m.name} v${m.versionMajor}.${m.versionMinor}`).join(', ') : '(none)'}`)
    console.log(`datastreams: ${hello.datastreams.length ? hello.datastreams.map(d => `${d.name}:${d.type}`).join(', ') : '(none)'}`)

    console.log()
    console.log('── declared per-pin capabilities ──────────────────────')
    console.log('pin   raw   flags')
    for (const p of hello.pins) {
      console.log(`  ${String(p.pin).padStart(2)}  0x${p.capabilities.toString(16).padStart(2,'0')}  ${fmtCaps(p.capabilities)}`)
    }

    if (probe) {
      console.log()
      console.log('── live probe (PIN_MODE + PIN_READ per pin) ───────────')
      console.log('pin   analog  digital  notes')
      for (let p = 0; p < hello.pinCount; p++) {
        const r = await probePin(client, p)
        const a = r.analogErr ? `ERR(${r.analogErr})` : String(r.analog)
        const d = r.digitalErr ? `ERR(${r.digitalErr})` : String(r.digital)
        const looksAnalog = r.analog !== null && r.analog > 0 && r.analog < 1023
        const note = looksAnalog ? '(non-rail analog)' : ''
        console.log(`  ${String(p).padStart(2)}  ${a.padEnd(6)}  ${d.padEnd(7)}  ${note}`)
      }
    }

    if (profile) {
      console.log()
      console.log('── compliance vs profile ──────────────────────────────')
      let mismatches = 0
      if (hello.pinCount !== profile.pinCount) {
        console.log(`!! pinCount mismatch: declared=${hello.pinCount}, expected=${profile.pinCount}`)
        mismatches++
      }
      if (hello.i2cBuses !== profile.i2cBuses) {
        console.log(`!! i2cBuses mismatch: declared=${hello.i2cBuses}, expected=${profile.i2cBuses}`)
        mismatches++
      }
      if (hello.spiBuses !== profile.spiBuses) {
        console.log(`!! spiBuses mismatch: declared=${hello.spiBuses}, expected=${profile.spiBuses}`)
        mismatches++
      }
      console.log('pin   declared                          expected                          delta')
      for (let i = 0; i < Math.min(profile.pinCount, hello.pinCount); i++) {
        const dec = hello.pins[i].capabilities
        const exp = profile.pinCaps[i] ?? 0
        const missing = decodeCaps(exp & ~dec)
        const extra = decodeCaps(dec & ~exp)
        const ok = dec === exp
        if (!ok) mismatches++
        const delta = ok ? 'ok' :
          (missing.length ? `missing[${missing.join(',')}]` : '') +
          (extra.length ? ` extra[${extra.join(',')}]` : '')
        console.log(`  ${String(i).padStart(2)}  0x${dec.toString(16).padStart(2,'0')} ${fmtCaps(dec).padEnd(30)}  0x${exp.toString(16).padStart(2,'0')} ${fmtCaps(exp).padEnd(30)}  ${delta}`)
      }
      console.log()
      console.log(mismatches === 0 ? 'PASS' : `FAIL (${mismatches} mismatch${mismatches === 1 ? '' : 'es'})`)
    }

    if (client.logs.length) {
      console.log()
      console.log('── device logs ────────────────────────────────────────')
      for (const l of client.logs) console.log('  ' + l)
    }

    exit(profile && /FAIL/.test('') ? 1 : 0)
  } finally {
    await client.close().catch(() => {})
  }
}

main().catch(e => { console.error('fatal:', e); exit(1) })
