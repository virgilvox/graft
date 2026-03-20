# conduyt-wasm

CONDUYT protocol core compiled to WebAssembly. Use the same Rust implementation that powers the native SDK — in the browser or Node.js.

[![npm](https://img.shields.io/npm/v/conduyt-wasm)](https://www.npmjs.com/package/conduyt-wasm)
[![WASM CI](https://github.com/virgilvox/conduyt/actions/workflows/wasm-ci.yml/badge.svg)](https://github.com/virgilvox/conduyt/actions/workflows/wasm-ci.yml)

## Install

```bash
npm install conduyt-wasm
```

## Quick Start (Browser)

```javascript
import init, {
  makePacket, wireEncode, wireDecode,
  cobsEncode, cobsDecode, crc8,
  getCMD, getEVT, errName
} from 'conduyt-wasm';

// Initialize the WASM module (required once before use)
await init();

const CMD = getCMD();
const EVT = getEVT();

// Build a PING packet
const pkt = makePacket(CMD.PING, 0);

// Encode to wire format
const wire = wireEncode(pkt);
console.log('Wire bytes:', wire);

// COBS-encode for serial framing
const frame = cobsEncode(wire);
console.log('COBS frame:', frame);

// Decode it back
const decoded = wireDecode(cobsDecode(frame));
console.log('Decoded:', decoded);
// { version: 1, type: 1, seq: 0, payload: Uint8Array([]) }
```

## Quick Start (Node.js)

```javascript
import init, * as conduyt from 'conduyt-wasm';
import { readFile } from 'fs/promises';

// Node.js needs the WASM binary loaded manually
const wasmBytes = await readFile('node_modules/conduyt-wasm/conduyt_wasm_bg.wasm');
await init(wasmBytes);

const CMD = conduyt.getCMD();
const pkt = conduyt.makePacket(CMD.PING, 0);
const wire = conduyt.wireEncode(pkt);
console.log('CRC8:', conduyt.crc8(wire.subarray(2, 7)));
```

## API Reference

### Packet Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `makePacket` | `(type: number, seq: number, payload?: Uint8Array) → ConduytPacket` | Build a packet with current protocol version |
| `wireEncode` | `(packet: ConduytPacket) → Uint8Array` | Encode packet to wire bytes |
| `wireDecode` | `(data: Uint8Array) → ConduytPacket` | Decode wire bytes (throws on error) |
| `wireFindPacket` | `(buf: Uint8Array) → [ConduytPacket, number] \| null` | Find first valid packet in buffer |
| `wirePacketSize` | `(payloadLen: number) → number` | Calculate total wire size |

### Framing Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `cobsEncode` | `(data: Uint8Array) → Uint8Array` | COBS-encode (eliminates 0x00 bytes) |
| `cobsDecode` | `(data: Uint8Array) → Uint8Array \| null` | COBS-decode (returns null on invalid) |
| `crc8` | `(data: Uint8Array) → number` | CRC8 Dallas/Maxim checksum |

### Constants

| Function | Returns | Description |
|----------|---------|-------------|
| `getCMD()` | `Record<string, number>` | Command codes (PING, HELLO, PIN_MODE, ...) |
| `getEVT()` | `Record<string, number>` | Event codes (PONG, HELLO_RESP, ACK, ...) |
| `getERR()` | `Record<string, number>` | Error codes (UNKNOWN_TYPE, CRC_MISMATCH, ...) |
| `errName(code)` | `string` | Human-readable error name |
| `PROTOCOL_VERSION()` | `number` | Protocol version (1) |
| `HEADER_SIZE()` | `number` | Packet header size (8) |

### ConduytPacket

```typescript
interface ConduytPacket {
  version: number;   // Protocol version
  type: number;      // Command/event type
  seq: number;       // Sequence number (0-255)
  payload: Uint8Array; // Payload bytes
}
```

## Size

~28KB raw, ~12KB gzipped. Uses `lol_alloc` for minimal allocator overhead.

## How It Works

This package compiles the Rust `conduyt` crate's `no_std` core (COBS, CRC8, wire format) to WebAssembly using `wasm-pack` + `wasm-bindgen`. The same code that runs natively in the Rust SDK runs byte-for-byte identically in the browser.

## License

MIT
