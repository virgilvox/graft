import { readFileSync } from 'fs'
import { wireEncode, wireDecode, makePacket } from '../sdk/js/src/core/wire.js'

interface Vector {
  name: string
  type: number
  seq: number
  payload_hex: string
}

interface VectorFile {
  description: string
  version: number
  vectors: Vector[]
}

const data: VectorFile = JSON.parse(
  readFileSync(new URL('./vectors/wire_vectors.json', import.meta.url), 'utf-8')
)

let passed = 0
let failed = 0

for (const v of data.vectors) {
  const payload = v.payload_hex
    ? new Uint8Array(v.payload_hex.match(/.{2}/g)!.map(h => parseInt(h, 16)))
    : new Uint8Array(0)

  try {
    // Encode
    const pkt = makePacket(v.type, v.seq, payload)
    const encoded = wireEncode(pkt)

    // Decode
    const decoded = wireDecode(encoded)

    // Verify roundtrip
    if (decoded.type !== v.type) throw new Error(`type mismatch: ${decoded.type} !== ${v.type}`)
    if (decoded.seq !== v.seq) throw new Error(`seq mismatch: ${decoded.seq} !== ${v.seq}`)

    const decodedHex = Array.from(decoded.payload).map(b => b.toString(16).padStart(2, '0')).join('')
    if (decodedHex !== v.payload_hex) {
      throw new Error(`payload mismatch: ${decodedHex} !== ${v.payload_hex}`)
    }

    console.log(`  PASS: ${v.name}`)
    passed++
  } catch (e) {
    console.log(`  FAIL: ${v.name} — ${(e as Error).message}`)
    failed++
  }
}

console.log(`\n${passed} passed, ${failed} failed, ${data.vectors.length} total`)
if (failed > 0) process.exit(1)
