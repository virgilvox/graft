---
title: JavaScript API
description: conduyt-js SDK reference
---

# JavaScript API

The `conduyt-js` package. Works in Node.js and browsers depending on transport.

```bash
npm install conduyt-js
```

## Quick Example

```js
import { ConduytDevice } from 'conduyt-js'
import { SerialTransport } from 'conduyt-js/transports/serial'

// Connect and perform HELLO handshake
const device = await ConduytDevice.connect(
  new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })
)

// Inspect device capabilities
console.log(device.capabilities.firmwareName)
console.log(device.capabilities.pins.length, 'pins')

// Control a pin
await device.pin(13).mode('output')
await device.pin(13).write(1)

// Read a datastream
const temp = await device.datastream('temperature').read()

// Send a module command
const servo = device.module('servo')
await servo.cmd(0x02, new Uint8Array([90]))  // write angle 90

await device.disconnect()
```

This script uses pin control, datastream reads, and module commands. The full API for each is documented below.

## ConduytDevice

### Static Factory

```typescript
static async connect(transport: ConduytTransport, options?: ConnectOptions): Promise<ConduytDevice>
```

Creates a device, connects, and sends HELLO in one call. Returns a connected `ConduytDevice` with `capabilities` populated.

### Constructor and Connection

```typescript
const device = new ConduytDevice(transport, options?)
const hello: HelloResp = await device.connect()
```

### Methods

| Method | Return | Description |
|---|---|---|
| `async connect()` | `Promise<HelloResp>` | Connect and perform HELLO handshake |
| `async disconnect()` | `Promise<void>` | Close the transport |
| `async ping()` | `Promise<void>` | Send PING, wait for PONG |
| `async reset()` | `Promise<void>` | Send RESET command |
| `pin(num)` | `PinProxy` | Get a pin proxy |
| `datastream(name)` | `DatastreamProxy` | Get a datastream proxy |
| `module(name)` | `ModuleProxy` | Get a module proxy |
| `i2c(bus?)` | `I2CProxy` | Get an I2C proxy (default bus 0) |
| `on(eventType, handler)` | `void` | Listen for device events |

### Properties

| Property | Type | Description |
|---|---|---|
| `capabilities` | `HelloResp \| null` | Parsed HELLO_RESP, null before connect |
| `connected` | `boolean` | Connection state |

## PinProxy

Returned by `device.pin(num)`.

| Method | Return | Description |
|---|---|---|
| `async mode(mode)` | `Promise<void>` | Set pin mode: "input", "output", "pwm", "analog", "input_pullup" |
| `async write(value)` | `Promise<void>` | Write digital or PWM value (0-255) |
| `async read()` | `Promise<number>` | Read pin value (0-1023 for analog, 0-1 for digital) |
| `subscribe(opts?)` | `AsyncIterable<number>` | Subscribe to pin changes. Options: `intervalMs`, `threshold` |

## DatastreamProxy

Returned by `device.datastream(name)`.

| Method | Return | Description |
|---|---|---|
| `async read()` | `Promise<any>` | Read current value |
| `async write(value)` | `Promise<void>` | Write a value (writable datastreams only) |
| `subscribe(opts?)` | `AsyncIterable<any>` | Subscribe to value updates. Options: `intervalMs` |

## ModuleProxy

Returned by `device.module(name)`.

| Method | Return | Description |
|---|---|---|
| `async cmd(cmdByte, data?)` | `Promise<Uint8Array \| void>` | Send a module command. Returns MOD_RESP data if the module responds. |
| `onEvent(handler)` | `void` | Register handler for unsolicited MOD_EVENT packets |

## I2CProxy

Returned by `device.i2c(bus?)`.

| Method | Return | Description |
|---|---|---|
| `async write(addr, data)` | `Promise<void>` | Write bytes to I2C address |
| `async read(addr, count)` | `Promise<Uint8Array>` | Read N bytes from I2C address |
| `async readReg(addr, reg, count)` | `Promise<Uint8Array>` | Read N bytes from register at I2C address |

## Transports

| Transport | Import | Environment |
|---|---|---|
| Serial | `conduyt-js/transports/serial` | Node.js |
| WebSerial | `conduyt-js/transports/web-serial` | Browser |
| BLE | `conduyt-js/transports/ble` | Browser |
| MQTT | `conduyt-js/transports/mqtt` | Node.js + Browser |
| CLASP | `conduyt-js/transports/clasp` | Browser |
| WebSocket | `conduyt-js/transports/websocket` | Both |
| Mock | `conduyt-js/transports/mock` | Testing |

All transports implement the `ConduytTransport` interface:

```typescript
interface ConduytTransport {
  open(): Promise<void>
  close(): Promise<void>
  send(packet: Uint8Array): Promise<void>
  onPacket(handler: (packet: Uint8Array) => void): void
  readonly needsCOBS: boolean
}
```

## Error Classes

### ConduytNAKError

Thrown when the device responds with NAK.

| Property | Type | Description |
|---|---|---|
| `errorName` | `string` | Error name (e.g., "INVALID_PIN") |
| `code` | `number` | Raw error code byte |

### ConduytTimeoutError

Thrown when a command receives no response within the timeout period.

### ConduytDisconnectedError

Thrown when a command is attempted on a disconnected device.

### ConduytCapabilityError

Thrown when an operation targets hardware the device does not support (e.g., I2C on a board with no I2C bus).

## Module Wrappers

High-level classes for built-in firmware modules. Each wraps `ModuleProxy` with typed methods.

| Class | Import | Firmware Module |
|---|---|---|
| `ConduytServo` | `conduyt-js/modules/servo` | servo |
| `ConduytNeoPixel` | `conduyt-js/modules/neopixel` | neopixel |
| `ConduytEncoder` | `conduyt-js/modules/encoder` | encoder |
| `ConduytStepper` | `conduyt-js/modules/stepper` | stepper |
| `ConduytDHT` | `conduyt-js/modules/dht` | dht |
| `ConduytOLED` | `conduyt-js/modules/oled` | oled1306 |
| `ConduytPID` | `conduyt-js/modules/pid` | pid |

Usage:

```typescript
import { ConduytServo } from 'conduyt-js/modules/servo'

const servo = new ConduytServo(device)
await servo.attach(9, 500, 2500)
await servo.write(90)
await servo.detach()
```

## Types

```typescript
interface HelloResp {
  firmwareName: string
  firmwareVersion: [number, number, number]
  mcuId: Uint8Array
  otaCapable: boolean
  pins: PinCapability[]
  i2cBuses: number
  spiBuses: number
  uartCount: number
  maxPayload: number
  modules: ModuleDescriptor[]
  datastreams: DatastreamDescriptor[]
}

interface PinCapability {
  pin: number
  capabilities: number  // bitmask of PIN_CAP values
}
```

Use the `PIN_CAP` constants to test individual bits:

```typescript
import { PIN_CAP } from 'conduyt-js'

const pin = device.capabilities.pins[13]
const hasPWM = (pin.capabilities & PIN_CAP.PWM_OUT) !== 0
```

```typescript
interface ModuleDescriptor {
  moduleId: number
  name: string
  versionMajor: number
  versionMinor: number
  pins: number[]
}

interface DatastreamDescriptor {
  index: number
  name: string
  type: number   // DS_TYPE value
  unit: string
  writable: boolean
  pinRef: number  // 0xFF if none
  retain: boolean
}

interface ConduytPacket {
  version: number
  type: number
  seq: number
  payload: Uint8Array
}
```
