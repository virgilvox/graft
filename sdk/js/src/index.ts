/**
 * graft-js — GRAFT Protocol SDK for JavaScript/TypeScript
 *
 * Reference host-side SDK for the GRAFT protocol.
 * Transport-agnostic, capability-first hardware control.
 */

// Core
export { PROTOCOL_VERSION, MAGIC, HEADER_SIZE, CMD, EVT, ERR, ERR_NAMES, DS_TYPE, DS_TYPE_SIZE, PIN_CAP, PIN_MODE, SUB_MODE } from './core/constants.js'
export { crc8 } from './core/crc8.js'
export { cobsEncode, cobsDecode } from './core/cobs.js'
export { wireEncode, wireDecode, makePacket, wirePacketSize, wireFindPacket } from './core/wire.js'

// Types
export type { GraftPacket, HelloResp, PinCapability, ModuleDescriptor, DatastreamDescriptor, GraftTransport, PinSubscribeOptions, DatastreamSubscribeOptions, DatastreamValue } from './core/types.js'

// Errors
export { GraftNAKError, GraftTimeoutError, GraftDisconnectedError, GraftCapabilityError, GraftWireError } from './core/errors.js'

// Device
export { GraftDevice } from './device.js'

// HELLO parser
export { parseHelloResp } from './hello.js'

// SEQ tracker
export { SeqTracker } from './seq.js'

// Reconnect wrapper
export { ReconnectTransport } from './reconnect.js'
export type { ReconnectOptions } from './reconnect.js'

// Transport interface (re-export from types for convenience)
export type { GraftTransport as GraftTransportInterface } from './transports/transport.js'
