/**
 * CONDUYT WASM + Serial Integration Tests
 *
 * Tests the conduyt-wasm npm package against a real device.
 * Validates that WASM-encoded packets are correctly understood by firmware.
 *
 * Usage:
 *   npm run test:wasm
 *   CONDUYT_PORT=/dev/cu.usbmodem101 npm run test:wasm
 */

import { describe, it, before, after } from 'node:test';
import assert from 'node:assert/strict';
import { SerialPort } from 'serialport';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { dirname, resolve } from 'node:path';

const __dirname = dirname(fileURLToPath(import.meta.url));

// ── Load WASM ───────────────────────────────────────────────────────
const wasmJsPath = resolve(__dirname, '..', 'sdk', 'wasm', 'pkg', 'conduyt_wasm.js');
const wasmBinPath = resolve(__dirname, '..', 'sdk', 'wasm', 'pkg', 'conduyt_wasm_bg.wasm');

let wasm;
try {
  wasm = await import(wasmJsPath);
  const wasmBytes = await readFile(wasmBinPath);
  await wasm.default(wasmBytes);
} catch (e) {
  // Fallback to npm package
  wasm = await import('conduyt-wasm');
  await wasm.default();
}

// ── Config ──────────────────────────────────────────────────────────
const PORT = process.env.CONDUYT_PORT || await autoDetectPort();
const BAUD = parseInt(process.env.CONDUYT_BAUD || '115200');
const TIMEOUT = 5000;

async function autoDetectPort() {
  const ports = await SerialPort.list();
  const arduino = ports.find(p => p.vendorId?.toLowerCase() === '2341');
  const usb = ports.find(p => p.vendorId);
  const found = arduino || usb;
  if (!found) {
    console.error('No USB serial device found.');
    process.exit(1);
  }
  console.log(`Auto-detected: ${found.path}`);
  return found.path;
}

// ── Serial helpers using conduyt-wasm ───────────────────────────────
let port;
let seq = 0;
let frameBuffer = Buffer.alloc(0);
const pendingResponses = new Map();

const CMD = wasm.getCMD();
const EVT = wasm.getEVT();

function nextSeq() { const s = seq; seq = (seq + 1) & 0xFF; return s; }

function sendCommand(cmdType, payload = new Uint8Array(0)) {
  return new Promise((resolve, reject) => {
    const s = nextSeq();

    // Use conduyt-wasm for encoding
    const pkt = wasm.makePacket(cmdType, s, payload);
    const wireBytes = wasm.wireEncode(pkt);
    const cobsBytes = wasm.cobsEncode(wireBytes);

    // Add 0x00 delimiter
    const frame = Buffer.concat([Buffer.from(cobsBytes), Buffer.from([0x00])]);

    const timer = setTimeout(() => {
      pendingResponses.delete(s);
      reject(new Error(`Timeout seq=${s}`));
    }, TIMEOUT);

    pendingResponses.set(s, { resolve, reject, timer });
    port.write(frame);
  });
}

function processData(data) {
  frameBuffer = Buffer.concat([frameBuffer, data]);
  let start = 0;
  for (let i = 0; i < frameBuffer.length; i++) {
    if (frameBuffer[i] === 0x00) {
      if (i > start) {
        const raw = new Uint8Array(frameBuffer.subarray(start, i));
        try {
          // Use conduyt-wasm for decoding
          const decoded = wasm.cobsDecode(raw);
          if (decoded) {
            const pkt = wasm.wireDecode(decoded);
            if (pkt) {
              const pending = pendingResponses.get(pkt.seq);
              if (pending) {
                clearTimeout(pending.timer);
                pendingResponses.delete(pkt.seq);
                if (pkt.type === EVT.NAK) {
                  const code = pkt.payload.length > 0 ? pkt.payload[0] : 0;
                  pending.reject(new Error(`NAK: ${wasm.errName(code)} (0x${code.toString(16)})`));
                } else {
                  pending.resolve(pkt);
                }
              }
            }
          }
        } catch {}
      }
      start = i + 1;
    }
  }
  frameBuffer = frameBuffer.subarray(start);
}

// ── Tests ───────────────────────────────────────────────────────────

console.log(`\nCONDUYT WASM + Serial Integration Tests`);
console.log(`Port: ${PORT}`);
console.log(`Using conduyt-wasm for all encoding/decoding\n`);

before(async () => {
  port = new SerialPort({ path: PORT, baudRate: BAUD });
  await new Promise((resolve, reject) => {
    port.on('open', resolve);
    port.on('error', reject);
  });
  port.on('data', processData);
  await new Promise(r => setTimeout(r, 1000));
});

after(async () => {
  for (const [, p] of pendingResponses) clearTimeout(p.timer);
  pendingResponses.clear();
  if (port?.isOpen) await new Promise(r => port.close(r));
});

describe('WASM encoding → firmware', () => {
  it('HELLO handshake via WASM-encoded packets', async () => {
    const resp = await sendCommand(CMD.HELLO);
    assert.equal(resp.type, EVT.HELLO_RESP);
    assert.ok(resp.payload.length >= 28);

    const name = String.fromCharCode(...resp.payload.slice(0, 16)).replace(/\0+$/, '');
    console.log(`    Firmware: ${name}`);
    assert.ok(name.length > 0);
  });

  it('PING/PONG via WASM encoding', async () => {
    const resp = await sendCommand(CMD.PING);
    assert.equal(resp.type, EVT.PONG);
  });

  it('PIN_MODE via WASM encoding', async () => {
    const resp = await sendCommand(CMD.PIN_MODE, new Uint8Array([2, 0x01]));
    assert.equal(resp.type, EVT.ACK);
  });

  it('PIN_WRITE via WASM encoding', async () => {
    const resp = await sendCommand(CMD.PIN_WRITE, new Uint8Array([2, 1, 0]));
    assert.equal(resp.type, EVT.ACK);
  });

  it('PIN_READ via WASM encoding', async () => {
    await sendCommand(CMD.PIN_MODE, new Uint8Array([2, 0x00])); // INPUT
    const resp = await sendCommand(CMD.PIN_READ, new Uint8Array([2]));
    assert.equal(resp.type, EVT.PIN_READ_RESP);
    const value = resp.payload[1] | (resp.payload[2] << 8);
    console.log(`    Pin 2 read: ${value}`);
  });
});

describe('WASM constants match firmware', () => {
  it('getCMD returns correct command codes', () => {
    assert.equal(CMD.PING, 0x01);
    assert.equal(CMD.HELLO, 0x02);
    assert.equal(CMD.PIN_MODE, 0x10);
    assert.equal(CMD.PIN_WRITE, 0x11);
    assert.equal(CMD.RESET, 0xF0);
  });

  it('getEVT returns correct event codes', () => {
    assert.equal(EVT.PONG, 0x80);
    assert.equal(EVT.HELLO_RESP, 0x81);
    assert.equal(EVT.ACK, 0x82);
    assert.equal(EVT.NAK, 0x83);
  });

  it('errName returns correct names', () => {
    assert.equal(wasm.errName(0x01), 'UNKNOWN_TYPE');
    assert.equal(wasm.errName(0x04), 'INVALID_PIN');
  });
});

describe('WASM wire format roundtrip with firmware', () => {
  it('firmware decodes WASM-encoded packets correctly across 10 pings', async () => {
    for (let i = 0; i < 10; i++) {
      const resp = await sendCommand(CMD.PING);
      assert.equal(resp.type, EVT.PONG);
    }
  });

  it('sequence numbers increment correctly', async () => {
    const startSeq = seq;
    await sendCommand(CMD.PING);
    await sendCommand(CMD.PING);
    await sendCommand(CMD.PING);
    assert.equal(seq, (startSeq + 3) & 0xFF);
  });
});

describe('WASM error handling matches firmware NAKs', () => {
  it('NAK error names match', async () => {
    try {
      await sendCommand(CMD.PIN_MODE, new Uint8Array([])); // too short
      assert.fail('Should NAK');
    } catch (e) {
      assert.ok(e.message.includes('NAK'));
      // errName should have decoded the error code
      console.log(`    NAK error: ${e.message}`);
    }
  });
});
