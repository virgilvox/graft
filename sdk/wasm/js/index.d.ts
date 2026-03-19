export interface ConduytPacket {
  version: number;
  type: number;
  seq: number;
  payload: Uint8Array;
}

/** Initialize the WASM module. Must be called before any other export. */
export function ensureInit(input?: RequestInfo | URL | Response | BufferSource | WebAssembly.Module): Promise<void>;

/** CRC8 Dallas/Maxim checksum. */
export function crc8(data: Uint8Array): number;

/** COBS-encode a byte buffer. */
export function cobsEncode(data: Uint8Array): Uint8Array;

/** COBS-decode a byte buffer. Returns null on invalid input. */
export function cobsDecode(data: Uint8Array): Uint8Array | null;

/** Encode a CONDUYT packet to wire bytes. */
export function wireEncode(packet: ConduytPacket): Uint8Array;

/** Decode wire bytes into a CONDUYT packet. Throws on error. */
export function wireDecode(data: Uint8Array): ConduytPacket;

/** Build a ConduytPacket with the current protocol version. */
export function makePacket(type: number, seq: number, payload?: Uint8Array): ConduytPacket;

/** Calculate total wire size for a given payload length. */
export function wirePacketSize(payloadLen: number): number;

/** Find the first complete packet in a byte buffer. Returns [packet, bytesConsumed] or null. */
export function wireFindPacket(buf: Uint8Array): [ConduytPacket, number] | null;

/** Get human-readable error name from a NAK error code. */
export function errName(code: number): string;

/** Host-to-device command codes. */
export const CMD: {
  readonly PING: 0x01;
  readonly HELLO: 0x02;
  readonly PIN_MODE: 0x10;
  readonly PIN_WRITE: 0x11;
  readonly PIN_READ: 0x12;
  readonly PIN_SUBSCRIBE: 0x13;
  readonly PIN_UNSUBSCRIBE: 0x14;
  readonly I2C_WRITE: 0x20;
  readonly I2C_READ: 0x21;
  readonly I2C_READ_REG: 0x22;
  readonly SPI_XFER: 0x30;
  readonly MOD_CMD: 0x40;
  readonly STREAM_START: 0x50;
  readonly STREAM_STOP: 0x51;
  readonly DS_WRITE: 0x60;
  readonly DS_READ: 0x61;
  readonly DS_SUBSCRIBE: 0x62;
  readonly OTA_BEGIN: 0x70;
  readonly OTA_CHUNK: 0x71;
  readonly OTA_FINALIZE: 0x72;
  readonly RESET: 0xf0;
};

/** Device-to-host event codes. */
export const EVT: {
  readonly PONG: 0x80;
  readonly HELLO_RESP: 0x81;
  readonly ACK: 0x82;
  readonly NAK: 0x83;
  readonly PIN_EVENT: 0x90;
  readonly PIN_READ_RESP: 0x91;
  readonly I2C_READ_RESP: 0xa0;
  readonly SPI_XFER_RESP: 0xb0;
  readonly MOD_EVENT: 0xc0;
  readonly MOD_RESP: 0xc1;
  readonly STREAM_DATA: 0xd0;
  readonly DS_EVENT: 0xd1;
  readonly DS_READ_RESP: 0xd2;
  readonly LOG: 0xe0;
  readonly FATAL: 0xff;
};

/** NAK error codes. */
export const ERR: {
  readonly UNKNOWN_TYPE: 0x01;
  readonly CRC_MISMATCH: 0x02;
  readonly PAYLOAD_TOO_LARGE: 0x03;
  readonly INVALID_PIN: 0x04;
  readonly PIN_MODE_UNSUPPORTED: 0x05;
  readonly I2C_NOT_AVAILABLE: 0x06;
  readonly I2C_NACK: 0x07;
  readonly MODULE_NOT_LOADED: 0x08;
  readonly UNKNOWN_MODULE_CMD: 0x09;
  readonly MODULE_BUSY: 0x0a;
  readonly SUB_LIMIT_REACHED: 0x0b;
  readonly OUT_OF_MEMORY: 0x0c;
  readonly UNKNOWN_DATASTREAM: 0x0d;
  readonly DATASTREAM_READONLY: 0x0e;
  readonly OTA_INVALID: 0x0f;
  readonly VERSION_MISMATCH: 0x10;
};

export const PROTOCOL_VERSION: number;
export const HEADER_SIZE: number;
export const MAGIC: Uint8Array;
