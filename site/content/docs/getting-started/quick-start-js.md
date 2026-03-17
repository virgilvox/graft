---
title: 'Quick Start: JavaScript'
description: Control a GRAFT device from Node.js or the browser
---

# Quick Start: JavaScript

## Install

```bash
npm install graft-js
```

## Connect and Blink

```typescript
import { GraftDevice } from 'graft-js'
import { SerialTransport } from 'graft-js/transports/serial'

const transport = new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })
const device = new GraftDevice(transport)

await device.connect()

// Blink pin 13
await device.pin(13).mode('output')
await device.pin(13).write(1)
await new Promise(r => setTimeout(r, 1000))
await device.pin(13).write(0)

await device.disconnect()
```

## Read Analog

```typescript
const value = await device.pin(0).read('analog')
console.log('Analog:', value) // 0–4095
```

## Use a Module

```typescript
import { GraftServo } from 'graft-js/modules/servo'

const servo = new GraftServo(device)
await servo.attach(9)
await servo.write(90) // center position
```

## Transports

| Transport | Install | Use Case |
|---|---|---|
| Serial | `graft-js/transports/serial` | USB / UART (Node.js) |
| WebSerial | `graft-js/transports/web-serial` | Browser USB |
| BLE | `graft-js/transports/ble` | Browser Bluetooth |
| MQTT | `graft-js/transports/mqtt` | IoT / remote |
| WebSocket | `graft-js/transports/websocket` | Browser / bridge |
| Mock | `graft-js/transports/mock` | Unit testing |

## Error Handling

```typescript
import { GraftNAKError, GraftTimeoutError } from 'graft-js'

try {
  await device.pin(99).write(1)
} catch (e) {
  if (e instanceof GraftNAKError) {
    console.error('Device error:', e.errorName) // INVALID_PIN
  }
}
```
