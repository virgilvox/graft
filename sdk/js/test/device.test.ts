import { describe, it, expect, beforeEach } from 'vitest'
import { GraftDevice } from '../src/device.js'
import { MockTransport } from '../src/transports/mock.js'
import { wireEncode, wireDecode, makePacket } from '../src/core/wire.js'
import { CMD, EVT, PROTOCOL_VERSION, DS_TYPE } from '../src/core/constants.js'

/**
 * Build a HELLO_RESP packet that the mock device sends back.
 */
function makeHelloRespPayload(): Uint8Array {
  const parts: number[] = []
  // firmware_name: 16 bytes
  const name = new TextEncoder().encode('MockBoard')
  for (let i = 0; i < 16; i++) parts.push(i < name.length ? name[i] : 0)
  // version: 1.0.0
  parts.push(1, 0, 0)
  // mcu_id: 8 bytes
  for (let i = 0; i < 8; i++) parts.push(0)
  // ota_capable
  parts.push(0)
  // pin_count = 20
  parts.push(20)
  for (let i = 0; i < 20; i++) parts.push(0x0F) // DIN|DOUT|PWM|AIN
  // i2c, spi, uart
  parts.push(1, 1, 1)
  // max_payload: 512
  parts.push(0x00, 0x02)
  // module_count = 1 (servo)
  parts.push(1)
  parts.push(0) // module_id
  const modName = new TextEncoder().encode('servo')
  for (let i = 0; i < 8; i++) parts.push(i < modName.length ? modName[i] : 0)
  parts.push(1, 0) // version 1.0
  parts.push(0) // 0 pins claimed
  // datastream_count = 1
  parts.push(1)
  const dsName = new TextEncoder().encode('temperature')
  for (let i = 0; i < 16; i++) parts.push(i < dsName.length ? dsName[i] : 0)
  parts.push(DS_TYPE.FLOAT32)
  const unit = new TextEncoder().encode('celsius')
  for (let i = 0; i < 8; i++) parts.push(i < unit.length ? unit[i] : 0)
  parts.push(0) // not writable
  parts.push(0xFF) // no pin ref
  parts.push(0) // no retain
  return new Uint8Array(parts)
}

/**
 * Auto-respond mock: when the SDK sends a HELLO command,
 * respond with HELLO_RESP. When it sends any other command, respond ACK.
 */
function autoRespond(transport: MockTransport) {
  const origSend = transport.send.bind(transport)
  transport.send = async (packet: Uint8Array) => {
    await origSend(packet)
    // Decode the sent packet to get seq and type
    try {
      const decoded = wireDecode(packet)
      if (decoded.type === CMD.HELLO) {
        const resp = wireEncode(makePacket(EVT.HELLO_RESP, decoded.seq, makeHelloRespPayload()))
        transport.inject(resp)
      } else if (decoded.type === CMD.PING) {
        const resp = wireEncode(makePacket(EVT.PONG, decoded.seq))
        transport.inject(resp)
      } else {
        // ACK everything else
        const resp = wireEncode(makePacket(EVT.ACK, decoded.seq))
        transport.inject(resp)
      }
    } catch { /* ignore decode errors */ }
  }
}

describe('GraftDevice', () => {
  let transport: MockTransport

  beforeEach(() => {
    transport = new MockTransport()
    autoRespond(transport)
  })

  it('connects and parses HELLO_RESP', async () => {
    const device = await GraftDevice.connect(transport)
    expect(device.connected).toBe(true)
    expect(device.capabilities).not.toBeNull()
    expect(device.capabilities!.firmwareName).toBe('MockBoard')
    expect(device.capabilities!.firmwareVersion).toEqual([1, 0, 0])
    expect(device.capabilities!.pins.length).toBe(20)
    expect(device.capabilities!.modules.length).toBe(1)
    expect(device.capabilities!.modules[0].name).toBe('servo')
    expect(device.capabilities!.datastreams.length).toBe(1)
    expect(device.capabilities!.datastreams[0].name).toBe('temperature')
  })

  it('sends PING and receives PONG', async () => {
    const device = await GraftDevice.connect(transport)
    await device.ping() // should not throw
  })

  it('sets pin mode', async () => {
    const device = await GraftDevice.connect(transport)
    await device.pin(13).mode('output')

    // Verify a PIN_MODE command was sent
    const sent = transport.sentPackets
    const pinModePkt = sent.find(p => {
      try { return wireDecode(p).type === CMD.PIN_MODE } catch { return false }
    })
    expect(pinModePkt).toBeDefined()
    const decoded = wireDecode(pinModePkt!)
    expect(decoded.payload[0]).toBe(13) // pin
    expect(decoded.payload[1]).toBe(0x01) // OUTPUT
  })

  it('writes to pin', async () => {
    const device = await GraftDevice.connect(transport)
    await device.pin(13).write(1)

    const sent = transport.sentPackets
    const pinWritePkt = sent.find(p => {
      try { return wireDecode(p).type === CMD.PIN_WRITE } catch { return false }
    })
    expect(pinWritePkt).toBeDefined()
    const decoded = wireDecode(pinWritePkt!)
    expect(decoded.payload[0]).toBe(13)
    expect(decoded.payload[1]).toBe(1)
  })

  it('reads from pin', async () => {
    // Override auto-respond for PIN_READ to return a value
    transport.send = async (packet: Uint8Array) => {
      transport.sentPackets.push(new Uint8Array(packet))
      try {
        const decoded = wireDecode(packet)
        if (decoded.type === CMD.HELLO) {
          transport.inject(wireEncode(makePacket(EVT.HELLO_RESP, decoded.seq, makeHelloRespPayload())))
        } else if (decoded.type === CMD.PIN_READ) {
          // Return pin 0, value 512 (little-endian)
          const resp = wireEncode(makePacket(EVT.PIN_READ_RESP, decoded.seq, new Uint8Array([0, 0x00, 0x02])))
          transport.inject(resp)
        } else {
          transport.inject(wireEncode(makePacket(EVT.ACK, decoded.seq)))
        }
      } catch { /* ignore */ }
    }

    const device = await GraftDevice.connect(transport)
    const value = await device.pin(0).read()
    expect(value).toBe(512)
  })

  it('sends module command', async () => {
    const device = await GraftDevice.connect(transport)
    const servo = device.module('servo')
    await servo.cmd(0x02, new Uint8Array([90])) // write angle 90

    const sent = transport.sentPackets
    const modPkt = sent.find(p => {
      try { return wireDecode(p).type === CMD.MOD_CMD } catch { return false }
    })
    expect(modPkt).toBeDefined()
    const decoded = wireDecode(modPkt!)
    expect(decoded.payload[0]).toBe(0) // module_id
    expect(decoded.payload[1]).toBe(0x02) // cmd: write angle
    expect(decoded.payload[2]).toBe(90)
  })

  it('throws on unknown module', async () => {
    const device = await GraftDevice.connect(transport)
    expect(() => device.module('nonexistent')).toThrow('not found')
  })

  it('sends reset', async () => {
    const device = await GraftDevice.connect(transport)
    await device.reset()

    const sent = transport.sentPackets
    const resetPkt = sent.find(p => {
      try { return wireDecode(p).type === CMD.RESET } catch { return false }
    })
    expect(resetPkt).toBeDefined()
  })

  it('disconnects cleanly', async () => {
    const device = await GraftDevice.connect(transport)
    await device.disconnect()
    expect(device.connected).toBe(false)
    expect(device.capabilities).toBeNull()
  })

  it('handles NAK responses', async () => {
    transport.send = async (packet: Uint8Array) => {
      transport.sentPackets.push(new Uint8Array(packet))
      try {
        const decoded = wireDecode(packet)
        if (decoded.type === CMD.HELLO) {
          transport.inject(wireEncode(makePacket(EVT.HELLO_RESP, decoded.seq, makeHelloRespPayload())))
        } else {
          // NAK with INVALID_PIN error
          transport.inject(wireEncode(makePacket(EVT.NAK, decoded.seq, new Uint8Array([0x04]))))
        }
      } catch { /* ignore */ }
    }

    const device = await GraftDevice.connect(transport)
    await expect(device.pin(99).mode('output')).rejects.toThrow('NAK')
  })

  it('increments SEQ per command', async () => {
    const device = await GraftDevice.connect(transport)

    // HELLO was seq 0, so next commands start from 1
    await device.pin(13).mode('output')
    await device.pin(13).write(1)
    await device.pin(13).write(0)

    // Check SEQ values are incrementing
    const seqs = transport.sentPackets.map(p => {
      try { return wireDecode(p).seq } catch { return -1 }
    }).filter(s => s >= 0)

    // Should be sequential
    for (let i = 1; i < seqs.length; i++) {
      expect(seqs[i]).toBe((seqs[i - 1] + 1) & 0xFF)
    }
  })
})
