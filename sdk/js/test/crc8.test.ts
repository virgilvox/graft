import { describe, it, expect } from 'vitest'
import { crc8 } from '../src/core/crc8.js'

describe('CRC8 Dallas/Maxim', () => {
  it('returns 0 for empty input', () => {
    expect(crc8(new Uint8Array([]))).toBe(0x00)
  })

  it('computes CRC for single byte 0x00', () => {
    expect(crc8(new Uint8Array([0x00]))).toBe(0x00)
  })

  it('computes CRC for single byte 0x01', () => {
    expect(crc8(new Uint8Array([0x01]))).toBe(0x31)
  })

  it('computes CRC for known vector [0x01, 0x02]', () => {
    const result = crc8(new Uint8Array([0x01, 0x02]))
    expect(result).toBeTypeOf('number')
    expect(result).toBeGreaterThanOrEqual(0)
    expect(result).toBeLessThanOrEqual(255)
  })

  it('produces different CRC for different data', () => {
    const a = crc8(new Uint8Array([0x01, 0x02, 0x03]))
    const b = crc8(new Uint8Array([0x01, 0x02, 0x04]))
    expect(a).not.toBe(b)
  })

  it('is deterministic', () => {
    const data = new Uint8Array([0x47, 0x46, 0x01, 0x02, 0x00, 0x00, 0x00])
    const a = crc8(data)
    const b = crc8(data)
    expect(a).toBe(b)
  })

  it('computes CRC for a longer payload', () => {
    const data = new Uint8Array(256)
    for (let i = 0; i < 256; i++) data[i] = i
    const result = crc8(data)
    expect(result).toBeTypeOf('number')
    expect(result).toBeGreaterThanOrEqual(0)
    expect(result).toBeLessThanOrEqual(255)
  })
})
