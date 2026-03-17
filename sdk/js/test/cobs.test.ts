import { describe, it, expect } from 'vitest'
import { cobsEncode, cobsDecode } from '../src/core/cobs.js'

describe('COBS', () => {
  describe('encode', () => {
    it('encodes empty input', () => {
      const result = cobsEncode(new Uint8Array([]))
      expect(result).toEqual(new Uint8Array([0x01]))
    })

    it('encodes single zero byte', () => {
      const result = cobsEncode(new Uint8Array([0x00]))
      expect(result).toEqual(new Uint8Array([0x01, 0x01]))
    })

    it('encodes single non-zero byte', () => {
      const result = cobsEncode(new Uint8Array([0x42]))
      expect(result).toEqual(new Uint8Array([0x02, 0x42]))
    })

    it('encodes [0x00, 0x00]', () => {
      const result = cobsEncode(new Uint8Array([0x00, 0x00]))
      expect(result).toEqual(new Uint8Array([0x01, 0x01, 0x01]))
    })

    it('encodes [0x11, 0x22, 0x00, 0x33]', () => {
      const result = cobsEncode(new Uint8Array([0x11, 0x22, 0x00, 0x33]))
      expect(result).toEqual(new Uint8Array([0x03, 0x11, 0x22, 0x02, 0x33]))
    })

    it('encodes [0x11, 0x22, 0x33, 0x44]', () => {
      const result = cobsEncode(new Uint8Array([0x11, 0x22, 0x33, 0x44]))
      expect(result).toEqual(new Uint8Array([0x05, 0x11, 0x22, 0x33, 0x44]))
    })

    it('never produces zero bytes in output', () => {
      // Random-ish data with zeros
      const data = new Uint8Array([0, 1, 2, 0, 3, 0, 0, 4, 5, 6, 7, 0, 8])
      const encoded = cobsEncode(data)
      for (let i = 0; i < encoded.length; i++) {
        expect(encoded[i]).not.toBe(0)
      }
    })
  })

  describe('decode', () => {
    it('decodes empty input returns null', () => {
      expect(cobsDecode(new Uint8Array([]))).toBeNull()
    })

    it('decodes [0x01] to empty', () => {
      const result = cobsDecode(new Uint8Array([0x01]))
      expect(result).toEqual(new Uint8Array([]))
    })

    it('rejects input with zero byte', () => {
      expect(cobsDecode(new Uint8Array([0x00, 0x01]))).toBeNull()
    })
  })

  describe('round-trip', () => {
    const vectors = [
      new Uint8Array([]),
      new Uint8Array([0x00]),
      new Uint8Array([0x00, 0x00]),
      new Uint8Array([0x42]),
      new Uint8Array([0x11, 0x22, 0x00, 0x33]),
      new Uint8Array([0x11, 0x22, 0x33, 0x44]),
      new Uint8Array([0, 1, 2, 0, 3, 0, 0, 4, 5, 6, 7, 0, 8]),
    ]

    for (const data of vectors) {
      it(`round-trips [${Array.from(data).map(b => '0x' + b.toString(16).padStart(2, '0')).join(', ')}]`, () => {
        const encoded = cobsEncode(data)
        const decoded = cobsDecode(encoded)
        expect(decoded).toEqual(data)
      })
    }

    it('round-trips 254 non-zero bytes (max block)', () => {
      const data = new Uint8Array(254)
      for (let i = 0; i < 254; i++) data[i] = i + 1
      const encoded = cobsEncode(data)
      const decoded = cobsDecode(encoded)
      expect(decoded).toEqual(data)
    })

    it('round-trips 255 non-zero bytes (block split)', () => {
      const data = new Uint8Array(255)
      for (let i = 0; i < 255; i++) data[i] = (i % 254) + 1
      const encoded = cobsEncode(data)
      const decoded = cobsDecode(encoded)
      expect(decoded).toEqual(data)
    })

    it('round-trips all zeros', () => {
      const data = new Uint8Array(10)
      const encoded = cobsEncode(data)
      const decoded = cobsDecode(encoded)
      expect(decoded).toEqual(data)
    })
  })
})
