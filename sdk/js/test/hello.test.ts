import { describe, it, expect } from 'vitest'
import { parseHelloResp } from '../src/hello.js'
import { DS_TYPE } from '../src/core/constants.js'

/**
 * Build a minimal HELLO_RESP binary payload for testing.
 */
function buildHelloPayload(opts: {
  name?: string
  version?: [number, number, number]
  pinCount?: number
  modules?: Array<{ name: string; pins?: number[] }>
  datastreams?: Array<{ name: string; type: number; unit: string; writable: boolean }>
} = {}): Uint8Array {
  const parts: number[] = []

  // firmware_name: 16 bytes
  const name = opts.name ?? 'TestBoard'
  const nameBytes = new TextEncoder().encode(name)
  for (let i = 0; i < 16; i++) parts.push(i < nameBytes.length ? nameBytes[i] : 0)

  // firmware_version: 3 bytes
  const ver = opts.version ?? [1, 2, 3]
  parts.push(ver[0], ver[1], ver[2])

  // mcu_id: 8 bytes
  for (let i = 0; i < 8; i++) parts.push(0)

  // ota_capable: 1 byte
  parts.push(0)

  // pin_count + caps
  const pinCount = opts.pinCount ?? 20
  parts.push(pinCount)
  for (let i = 0; i < pinCount; i++) parts.push(0x03) // DIGITAL_IN | DIGITAL_OUT

  // i2c_buses, spi_buses, uart_count
  parts.push(1, 1, 1)

  // max_payload: uint16 LE
  parts.push(0x00, 0x02) // 512

  // modules
  const modules = opts.modules ?? []
  parts.push(modules.length)
  for (let i = 0; i < modules.length; i++) {
    const mod = modules[i]
    parts.push(i) // module_id
    const modName = new TextEncoder().encode(mod.name)
    for (let j = 0; j < 8; j++) parts.push(j < modName.length ? modName[j] : 0)
    parts.push(1, 0) // version 1.0
    const pins = mod.pins ?? []
    parts.push(pins.length)
    for (const p of pins) parts.push(p)
  }

  // datastreams
  const datastreams = opts.datastreams ?? []
  parts.push(datastreams.length)
  for (const ds of datastreams) {
    const dsName = new TextEncoder().encode(ds.name)
    for (let j = 0; j < 16; j++) parts.push(j < dsName.length ? dsName[j] : 0)
    parts.push(ds.type)
    const unitBytes = new TextEncoder().encode(ds.unit)
    for (let j = 0; j < 8; j++) parts.push(j < unitBytes.length ? unitBytes[j] : 0)
    parts.push(ds.writable ? 1 : 0)
    parts.push(0xFF) // pin_ref: none
    parts.push(ds.writable ? 1 : 0) // retain = writable
  }

  return new Uint8Array(parts)
}

describe('parseHelloResp', () => {
  it('parses firmware name and version', () => {
    const payload = buildHelloPayload({ name: 'MyBoard', version: [2, 5, 1] })
    const hello = parseHelloResp(payload)

    expect(hello.firmwareName).toBe('MyBoard')
    expect(hello.firmwareVersion).toEqual([2, 5, 1])
  })

  it('parses pin count and capabilities', () => {
    const payload = buildHelloPayload({ pinCount: 14 })
    const hello = parseHelloResp(payload)

    expect(hello.pins.length).toBe(14)
    expect(hello.pins[0].pin).toBe(0)
    expect(hello.pins[0].capabilities).toBe(0x03) // DIN | DOUT
  })

  it('parses bus counts and max payload', () => {
    const payload = buildHelloPayload()
    const hello = parseHelloResp(payload)

    expect(hello.i2cBuses).toBe(1)
    expect(hello.spiBuses).toBe(1)
    expect(hello.uartCount).toBe(1)
    expect(hello.maxPayload).toBe(512)
  })

  it('parses modules', () => {
    const payload = buildHelloPayload({
      modules: [
        { name: 'servo', pins: [9] },
        { name: 'neopixel', pins: [6] },
      ]
    })
    const hello = parseHelloResp(payload)

    expect(hello.modules.length).toBe(2)
    expect(hello.modules[0].name).toBe('servo')
    expect(hello.modules[0].moduleId).toBe(0)
    expect(hello.modules[0].pins).toEqual([9])
    expect(hello.modules[1].name).toBe('neopixel')
    expect(hello.modules[1].pins).toEqual([6])
  })

  it('parses datastreams', () => {
    const payload = buildHelloPayload({
      datastreams: [
        { name: 'temperature', type: DS_TYPE.FLOAT32, unit: 'celsius', writable: false },
        { name: 'setpoint', type: DS_TYPE.FLOAT32, unit: 'celsius', writable: true },
        { name: 'relay', type: DS_TYPE.BOOL, unit: '', writable: true },
      ]
    })
    const hello = parseHelloResp(payload)

    expect(hello.datastreams.length).toBe(3)

    expect(hello.datastreams[0].name).toBe('temperature')
    expect(hello.datastreams[0].type).toBe(DS_TYPE.FLOAT32)
    expect(hello.datastreams[0].unit).toBe('celsius')
    expect(hello.datastreams[0].writable).toBe(false)

    expect(hello.datastreams[1].name).toBe('setpoint')
    expect(hello.datastreams[1].writable).toBe(true)
    expect(hello.datastreams[1].retain).toBe(true)

    expect(hello.datastreams[2].name).toBe('relay')
    expect(hello.datastreams[2].type).toBe(DS_TYPE.BOOL)
  })

  it('handles zero modules and zero datastreams', () => {
    const payload = buildHelloPayload({ modules: [], datastreams: [] })
    const hello = parseHelloResp(payload)

    expect(hello.modules.length).toBe(0)
    expect(hello.datastreams.length).toBe(0)
  })

  it('parses OTA capability', () => {
    const payload = buildHelloPayload()
    const hello = parseHelloResp(payload)
    expect(hello.otaCapable).toBe(false)
  })
})
