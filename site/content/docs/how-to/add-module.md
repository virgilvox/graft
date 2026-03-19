---
title: Add a Firmware Module
description: Write and register a custom firmware module for a Conduyt device
---

# Add a Firmware Module

Modules extend a Conduyt device with custom hardware capabilities. Each module handles commands, emits events, and runs a poll loop.

Write a custom module when built-in pin control and datastreams are not enough for your hardware. Common cases: devices that need initialization sequences (e.g., display controllers), multi-step communication protocols (e.g., 1-Wire sensors), or continuous background processing (e.g., PID loops, stepper pulse generation).

## Module Interface

Every module implements `ConduytModuleBase`:

```cpp
class ConduytModuleBase {
public:
    virtual const char* name() = 0;        // max 8 characters
    virtual uint8_t versionMajor() { return 1; }
    virtual uint8_t versionMinor() { return 0; }
    virtual void begin() {}                // called after device.begin()
    virtual void handle(uint8_t cmd, ConduytPayloadReader &payload, ConduytContext &ctx) = 0;
    virtual void poll() {}                 // called every loop cycle
    virtual uint8_t pinCount() { return 0; }
    virtual const uint8_t* pins() { return nullptr; }
};
```

## Write a Module

Use the `CONDUYT_MODULE` macro and `CONDUYT_ON_CMD` for command dispatch:

```cpp
#define CONDUYT_MODULE_RELAY
#include <Conduyt.h>

CONDUYT_MODULE(RelayModule) {
public:
    const char* name() override { return "relay"; }
    uint8_t versionMajor() override { return 1; }
    uint8_t versionMinor() override { return 0; }

    void begin() override {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
    }

    void handle(uint8_t cmd, ConduytPayloadReader &payload, ConduytContext &ctx) override {
        CONDUYT_ON_CMD(0x01) {
            // Set relay state
            uint8_t state = payload.readUInt8();
            digitalWrite(_pin, state ? HIGH : LOW);
            ctx.ack();
        }

        CONDUYT_ON_CMD(0x02) {
            // Report relay state
            uint8_t buf[1];
            ConduytPayloadWriter w(buf, sizeof(buf));
            w.writeUInt8(digitalRead(_pin));
            ctx.sendModResp(0x02, buf, w.length());
        }
    }

    void poll() override {
        // Nothing continuous for a relay
    }

private:
    uint8_t _pin = 4;
};
```

## Register the Module

Add modules before calling `device.begin()`:

```cpp
void setup() {
    device.addModule(new RelayModule());
    device.addModule(new TempSensorModule());
    device.begin();
}
```

Module IDs are assigned by registration order. The first module added gets ID 0, the second gets ID 1, and so on. These IDs appear in the HELLO_RESP packet so the host knows what modules are available.

## Context API

The `ConduytContext` object passed to `handle()` controls responses:

| Method | Purpose |
|---|---|
| `ctx.ack()` | Send ACK to confirm the command succeeded |
| `ctx.nak(errorCode)` | Send NAK with an error code |
| `ctx.sendModResp(id, data, len)` | Send a typed response with payload data |
| `ctx.emitModEvent(id, code, data, len)` | Emit an unsolicited event to the host |

If the handler does not call `ack()`, `nak()`, or `sendModResp()`, ConduytDevice auto-sends ACK.

## Reading Payloads

`ConduytPayloadReader` reads typed values from the command payload:

```cpp
uint8_t  val8  = payload.readUInt8();
uint16_t val16 = payload.readUInt16();
int32_t  val32 = payload.readInt32();
float    valf  = payload.readFloat32();
```

All multi-byte values are little-endian.

## Writing Payloads

`ConduytPayloadWriter` builds response data:

```cpp
uint8_t buf[8];
ConduytPayloadWriter w(buf, sizeof(buf));
w.writeUInt8(0x01);
w.writeFloat32(23.5f);
ctx.sendModResp(0x01, buf, w.length());
```

## Poll Loop

The `poll()` method runs every `device.poll()` cycle. Use it for continuous behavior like stepper pulse generation or PID control loops. Keep `poll()` fast and non-blocking. Do not call `delay()` inside it.

```cpp
void poll() override {
    unsigned long now = millis();
    if (now - _lastRead >= 1000) {
        _lastRead = now;
        _latestTemp = readSensor();
        _hasNewReading = true;
    }
}
```

The `poll()` method does not receive a `ConduytContext`, so it cannot send responses directly. Store results and return them when the host sends a command via `handle()`. For unsolicited events, use the device's `sendPacket()` method through a stored device reference.

## Control the Module from the Host

After flashing firmware with your module, use the host SDK to send commands. The module's ID is its registration index (first module added = 0).

### JavaScript

```js
// Get the module proxy by name (matches the name() return value)
const relay = device.module('relay')

// Send command 0x01 (set state) with payload [1] (on)
await relay.cmd(0x01, new Uint8Array([1]))

// Send command 0x02 (read state), receive response
const resp = await relay.cmd(0x02)
console.log('Relay state:', resp[0])
```

### Python

The Python SDK does not have a `.module()` proxy method. You send raw `MOD_CMD` packets. Built-in modules in `conduyt.modules` have pre-built wrappers that handle this for you, but for a custom module you write a thin wrapper class:

```python
from conduyt.protocol import CMD_MOD_CMD


class RelayModule:
    """Wrapper for the relay firmware module."""

    def __init__(self, device, module_id: int):
        self._device = device
        self._module_id = module_id

    async def set_state(self, on: bool):
        payload = bytes([self._module_id, 0x01, int(on)])
        await self._device._send_command(CMD_MOD_CMD, payload)

    async def get_state(self) -> int:
        payload = bytes([self._module_id, 0x02])
        resp = await self._device._send_command(CMD_MOD_CMD, payload)
        return resp[0]
```

Usage:

```python
relay = RelayModule(device, module_id=0)
await relay.set_state(True)

state = await relay.get_state()
print(f"Relay state: {state}")
```

This calls `_send_command()` directly, which is a private method. The module wrappers in `conduyt.modules` for built-in modules use the same pattern internally. A public module API for Python is planned.

## Compile Guards

Gate each module with a compile-time define to keep binary size small on constrained MCUs:

```cpp
#define CONDUYT_MODULE_RELAY
#define CONDUYT_MODULE_TEMPSENSOR
#include <Conduyt.h>
```

Only the modules you define get compiled into the firmware.
