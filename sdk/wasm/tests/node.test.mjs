import { describe, it, before } from 'node:test';
import assert from 'node:assert/strict';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { dirname, resolve } from 'node:path';

const __dirname = dirname(fileURLToPath(import.meta.url));

// Load WASM directly for Node.js (no fetch available)
async function loadWasm() {
  const wasmPath = resolve(__dirname, '..', 'pkg', 'conduyt_wasm_bg.wasm');
  const jsPath = resolve(__dirname, '..', 'pkg', 'conduyt_wasm.js');

  const mod = await import(jsPath);
  const wasmBytes = await readFile(wasmPath);
  await mod.default(wasmBytes);
  return mod;
}

let wasm;

before(async () => {
  wasm = await loadWasm();
});

// ---------------------------------------------------------------------------
// CRC8
// ---------------------------------------------------------------------------

describe('crc8', () => {
  it('empty input returns 0', () => {
    assert.equal(wasm.crc8(new Uint8Array([])), 0);
  });

  it('single byte 0x00', () => {
    const val = wasm.crc8(new Uint8Array([0x00]));
    assert.equal(typeof val, 'number');
    assert.equal(val, wasm.crc8(new Uint8Array([0x00])));
  });

  it('single byte 0x01', () => {
    const val = wasm.crc8(new Uint8Array([0x01]));
    assert.equal(typeof val, 'number');
    assert.notEqual(val, wasm.crc8(new Uint8Array([0x00])));
  });

  it('is deterministic', () => {
    const data = new Uint8Array([0x01, 0x02, 0x03]);
    assert.equal(wasm.crc8(data), wasm.crc8(data));
  });

  it('different data produces different CRC', () => {
    const a = wasm.crc8(new Uint8Array([0x01, 0x02]));
    const b = wasm.crc8(new Uint8Array([0x03, 0x04]));
    assert.notEqual(a, b);
  });
});

// ---------------------------------------------------------------------------
// COBS
// ---------------------------------------------------------------------------

describe('cobs', () => {
  it('empty roundtrip', () => {
    const enc = wasm.cobsEncode(new Uint8Array([]));
    const dec = wasm.cobsDecode(enc);
    assert.deepEqual(dec, new Uint8Array([]));
  });

  it('single zero roundtrip', () => {
    const enc = wasm.cobsEncode(new Uint8Array([0x00]));
    const dec = wasm.cobsDecode(enc);
    assert.deepEqual(dec, new Uint8Array([0x00]));
  });

  it('mixed data roundtrip', () => {
    const data = new Uint8Array([0x01, 0x00, 0x02, 0x00, 0x03]);
    const enc = wasm.cobsEncode(data);
    const dec = wasm.cobsDecode(enc);
    assert.deepEqual(dec, data);
  });

  it('no zeros in encoded output', () => {
    const data = new Uint8Array([0x00, 0x00, 0x00]);
    const enc = wasm.cobsEncode(data);
    for (let i = 0; i < enc.length; i++) {
      assert.notEqual(enc[i], 0x00, `byte at index ${i} should not be 0x00`);
    }
  });

  it('decode invalid returns null', () => {
    assert.equal(wasm.cobsDecode(new Uint8Array([])), null);
  });
});

// ---------------------------------------------------------------------------
// Wire format
// ---------------------------------------------------------------------------

describe('wire', () => {
  it('ping encode/decode roundtrip', () => {
    const pkt = wasm.makePacket(0x01, 7);
    const enc = wasm.wireEncode(pkt);
    assert.equal(enc.length, 8); // HEADER_SIZE, no payload
    const dec = wasm.wireDecode(enc);
    assert.equal(dec.type, 0x01);
    assert.equal(dec.seq, 7);
    assert.equal(dec.payload.length, 0);
  });

  it('pin write roundtrip', () => {
    const pkt = wasm.makePacket(0x11, 255, new Uint8Array([5, 128]));
    const enc = wasm.wireEncode(pkt);
    const dec = wasm.wireDecode(enc);
    assert.equal(dec.type, 0x11);
    assert.equal(dec.seq, 255);
    assert.deepEqual(dec.payload, new Uint8Array([5, 128]));
  });

  it('wirePacketSize', () => {
    assert.equal(wasm.wirePacketSize(0), 8);
    assert.equal(wasm.wirePacketSize(10), 18);
  });

  it('decode throws on incomplete', () => {
    assert.throws(
      () => wasm.wireDecode(new Uint8Array([0x43, 0x44])),
      (err) => (err.message || err).includes('Incomplete')
    );
  });

  it('decode throws on invalid magic', () => {
    assert.throws(
      () => wasm.wireDecode(new Uint8Array([0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00])),
      (err) => (err.message || err).includes('Invalid magic')
    );
  });

  it('decode throws on CRC mismatch', () => {
    const pkt = wasm.makePacket(0x01, 0);
    const enc = wasm.wireEncode(pkt);
    enc[7] ^= 0xFF;
    assert.throws(
      () => wasm.wireDecode(enc),
      (err) => (err.message || err).includes('CRC mismatch')
    );
  });

  it('decode throws on version mismatch', () => {
    const pkt = wasm.makePacket(0x01, 0);
    const enc = wasm.wireEncode(pkt);
    enc[2] = 0xFF; // bad version
    // Recompute CRC to isolate version check
    const crcRegion = enc.subarray(2, 7);
    enc[7] = wasm.crc8(crcRegion);
    assert.throws(
      () => wasm.wireDecode(enc),
      (err) => (err.message || err).includes('Version mismatch')
    );
  });
});

// ---------------------------------------------------------------------------
// wireFindPacket
// ---------------------------------------------------------------------------

describe('wireFindPacket', () => {
  it('finds packet at start of buffer', () => {
    const pkt = wasm.makePacket(0x01, 5);
    const enc = wasm.wireEncode(pkt);
    const result = wasm.wireFindPacket(enc);
    assert.notEqual(result, null);
    assert.equal(result[0].type, 0x01);
    assert.equal(result[0].seq, 5);
    assert.equal(result[1], enc.length);
  });

  it('finds packet with leading garbage', () => {
    const pkt = wasm.makePacket(0x01, 3, new Uint8Array([0xAA]));
    const enc = wasm.wireEncode(pkt);
    const buf = new Uint8Array(3 + enc.length);
    buf[0] = 0xFF; buf[1] = 0xFE; buf[2] = 0xFD;
    buf.set(enc, 3);
    const result = wasm.wireFindPacket(buf);
    assert.notEqual(result, null);
    assert.equal(result[0].type, 0x01);
    assert.equal(result[0].seq, 3);
    assert.deepEqual(result[0].payload, new Uint8Array([0xAA]));
    assert.equal(result[1], 3 + enc.length);
  });

  it('returns null for incomplete packet', () => {
    const pkt = wasm.makePacket(0x01, 0, new Uint8Array([1, 2, 3]));
    const enc = wasm.wireEncode(pkt);
    assert.equal(wasm.wireFindPacket(enc.subarray(0, enc.length - 1)), null);
  });

  it('returns null when no magic found', () => {
    assert.equal(wasm.wireFindPacket(new Uint8Array([0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])), null);
  });
});

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

describe('constants', () => {
  it('CMD constants match expected values', () => {
    const cmd = wasm.getCMD();
    assert.equal(cmd.PING, 0x01);
    assert.equal(cmd.HELLO, 0x02);
    assert.equal(cmd.PIN_MODE, 0x10);
    assert.equal(cmd.PIN_WRITE, 0x11);
    assert.equal(cmd.RESET, 0xF0);
  });

  it('EVT constants match expected values', () => {
    const evt = wasm.getEVT();
    assert.equal(evt.PONG, 0x80);
    assert.equal(evt.HELLO_RESP, 0x81);
    assert.equal(evt.ACK, 0x82);
    assert.equal(evt.NAK, 0x83);
    assert.equal(evt.FATAL, 0xFF);
  });

  it('ERR constants match expected values', () => {
    const err = wasm.getERR();
    assert.equal(err.UNKNOWN_TYPE, 0x01);
    assert.equal(err.CRC_MISMATCH, 0x02);
    assert.equal(err.VERSION_MISMATCH, 0x10);
  });

  it('errName returns correct names', () => {
    assert.equal(wasm.errName(0x01), 'UNKNOWN_TYPE');
    assert.equal(wasm.errName(0x02), 'CRC_MISMATCH');
    assert.equal(wasm.errName(0xFF), 'UNKNOWN');
  });

  it('PROTOCOL_VERSION and HEADER_SIZE', () => {
    assert.equal(wasm.PROTOCOL_VERSION(), 0x01);
    assert.equal(wasm.HEADER_SIZE(), 8);
  });
});

// ---------------------------------------------------------------------------
// Conformance vectors
// ---------------------------------------------------------------------------

describe('conformance', () => {
  it('wire vectors roundtrip', async () => {
    const vectorsPath = resolve(__dirname, '..', '..', '..', 'conformance', 'vectors', 'wire_vectors.json');
    const vectors = JSON.parse(await readFile(vectorsPath, 'utf-8'));

    for (const v of vectors.vectors) {
      const payloadBytes = v.payload_hex
        ? new Uint8Array(v.payload_hex.match(/.{2}/g).map(h => parseInt(h, 16)))
        : new Uint8Array(0);
      const pkt = wasm.makePacket(v.type, v.seq, payloadBytes);
      const enc = wasm.wireEncode(pkt);
      const dec = wasm.wireDecode(enc);
      assert.equal(dec.type, v.type, `type mismatch for vector: ${v.name}`);
      assert.equal(dec.seq, v.seq, `seq mismatch for vector: ${v.name}`);
      assert.deepEqual(dec.payload, payloadBytes, `payload mismatch for vector: ${v.name}`);
    }
  });
});
