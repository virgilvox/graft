---
title: JavaScript / TypeScript
description: graft-js SDK guide
---

# JavaScript / TypeScript SDK

## Install

```bash
npm install graft-js
```

## Connect

```typescript
import { GraftDevice } from 'graft-js'
import { SerialTransport } from 'graft-js/transports/serial'

const device = await GraftDevice.connect(
  new SerialTransport({ path: '/dev/ttyUSB0' })
)

console.log(device.capabilities.firmwareName)
console.log(device.capabilities.pins.length, 'pins')
console.log(device.capabilities.modules.map(m => m.name))
```

## Pin Control

```typescript
// Digital output
await device.pin(13).mode('output')
await device.pin(13).write(1)  // HIGH
await device.pin(13).write(0)  // LOW

// PWM
await device.pin(9).mode('pwm')
await device.pin(9).write(128)  // 50% duty

// Analog read
const value = await device.pin(0).read()  // 0-1023

// Subscription (AsyncIterator)
for await (const val of device.pin(0).subscribe({ intervalMs: 50, threshold: 5 })) {
  console.log('pot:', val)
}
```

## Datastreams

```typescript
// Subscribe to device-pushed values
for await (const temp of device.datastream('temperature').subscribe()) {
  console.log(`${temp}°C`)
}

// Write to a writable datastream
await device.datastream('setpoint').write(25.0)
```

## Modules

```typescript
import { GraftServo } from 'graft-js/modules/servo'

const servo = new GraftServo(device)
await servo.attach(9, 500, 2500)
await servo.write(90)
await servo.detach()
```

## Transports

| Transport | Import | Environment |
|---|---|---|
| Serial | `graft-js/transports/serial` | Node.js |
| WebSerial | `graft-js/transports/web-serial` | Browser |
| BLE | `graft-js/transports/ble` | Browser |
| MQTT | `graft-js/transports/mqtt` | Node.js + Browser |
| CLASP | `graft-js/transports/clasp` | Browser |
| WebSocket | `graft-js/transports/websocket` | Both |
| Mock | `graft-js/transports/mock` | Testing |

All transports implement the same `GraftTransport` interface. The `GraftDevice` class works identically regardless of which transport you use.

## Error Handling

```typescript
import { GraftNAKError, GraftTimeoutError } from 'graft-js'

try {
  await device.pin(99).mode('output')
} catch (err) {
  if (err instanceof GraftNAKError) {
    console.log(err.errorName)  // "INVALID_PIN"
    console.log(err.code)       // 0x04
  }
}
```
