---
title: Use Datastreams
description: Declare, push, and subscribe to named typed values between device and host
---

# Use Datastreams

Datastreams are named, typed values that flow between a Conduyt device and host. Use datastreams when you need structured, application-level data: sensor readings with units, configuration values, status indicators. For direct hardware control (toggling pins, reading raw ADC values), use the pin API instead.

## Declare Datastreams on the Device

Register datastreams before calling `device.begin()`:

```cpp
// read-only: device pushes, host reads
device.addDatastream("temperature", CONDUYT_TYPE_FLOAT32, "celsius", false);

// writable: host writes, device receives
device.addDatastream("setpoint", CONDUYT_TYPE_FLOAT32, "celsius", true);

device.begin();
```

The fourth argument controls write permission. Set `false` for read-only (device to host) and `true` for writable (host to device).

## Push Values from the Device

Use non-blocking timing to push values at a fixed interval:

```cpp
unsigned long lastPush = 0;

void loop() {
    device.poll();

    if (millis() - lastPush >= 1000) {
        lastPush = millis();
        device.writeDatastream("temperature", readSensor());
    }
}
```

Each `writeDatastream` call sends a DS_EVENT packet to the host. Avoid calling it every loop iteration; the host and serial link cannot keep up with tens of thousands of updates per second.

## Receive Writes on the Device

```cpp
device.onDatastreamWrite("setpoint", [](ConduytPayloadReader &payload, ConduytContext &ctx) {
    float value = payload.readFloat32();
    applySetpoint(value);
    ctx.ack();
});
```

## Type Codes

| Code | Constant | Size |
|---|---|---|
| `0x01` | `CONDUYT_TYPE_BOOL` | 1 byte |
| `0x02` | `CONDUYT_TYPE_INT8` | 1 byte |
| `0x03` | `CONDUYT_TYPE_UINT8` | 1 byte |
| `0x04` | `CONDUYT_TYPE_INT16` | 2 bytes |
| `0x05` | `CONDUYT_TYPE_UINT16` | 2 bytes |
| `0x06` | `CONDUYT_TYPE_INT32` | 4 bytes |
| `0x07` | `CONDUYT_TYPE_FLOAT32` | 4 bytes |
| `0x08` | `CONDUYT_TYPE_STRING` | variable (UTF-8, null-terminated) |
| `0x09` | `CONDUYT_TYPE_BYTES` | variable (raw binary) |

## Read from the Host (JavaScript)

```typescript
const temp = await device.datastream('temperature').read()
console.log('Temperature:', temp)
```

## Write from the Host (JavaScript)

```typescript
await device.datastream('setpoint').write(25.0)
```

## Subscribe to Updates (JavaScript)

```typescript
for await (const value of device.datastream('temperature').subscribe()) {
    console.log('Temperature:', value)
}
```

The async iterator yields a new value each time the device pushes an update.

## Host Python

The Python SDK does not have a datastream proxy like the JavaScript SDK's `.datastream()` method. Datastreams are firmware-to-host push via DS_EVENT packets. To read the latest value from Python, send a DS_READ command (`0x61`) using `_send_command`. To subscribe to push updates, send DS_SUBSCRIBE (`0x62`). The JavaScript SDK's `.datastream()` method handles this protocol internally and is the recommended path for host-side datastream interaction.

## Full Example

Firmware:

```cpp
#include <Conduyt.h>

ConduytSerial transport(Serial, 115200);
ConduytDevice device("TempNode", "1.0.0", transport);

void setup() {
    Serial.begin(115200);

    device.addDatastream("temperature", CONDUYT_TYPE_FLOAT32, "celsius", false);
    device.addDatastream("setpoint", CONDUYT_TYPE_FLOAT32, "celsius", true);

    device.onDatastreamWrite("setpoint", [](ConduytPayloadReader &payload, ConduytContext &ctx) {
        float sp = payload.readFloat32();
        Serial.print("New setpoint: ");
        Serial.println(sp);
        ctx.ack();
    });

    device.begin();
}

unsigned long lastPush = 0;

void loop() {
    device.poll();

    if (millis() - lastPush >= 1000) {
        lastPush = millis();
        device.writeDatastream("temperature", analogRead(A0) * 0.48828125f);
    }
}
```

Host (JavaScript):

```typescript
import { ConduytDevice } from 'conduyt-js'
import { SerialTransport } from 'conduyt-js/transports/serial'

const device = await ConduytDevice.connect(
  new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })
)

// Read current value
const temp = await device.datastream('temperature').read()
console.log('Current:', temp)

// Set target
await device.datastream('setpoint').write(22.0)

// Stream updates
for await (const value of device.datastream('temperature').subscribe()) {
    console.log('Temperature:', value)
}
```

For wire format details and type encoding, see [Datastream Types](/docs/reference/datastream-types).
