---
title: Module System
description: Extending GRAFT firmware with custom hardware modules
---

# Module System

Modules are opt-in C++ classes that add hardware capabilities to a GRAFT device. They handle MOD_CMD packets and can emit MOD_EVENT responses.

## Module Interface

```cpp
class GraftModuleBase {
public:
    virtual const char* name() = 0;        // max 8 chars
    virtual uint8_t versionMajor() { return 1; }
    virtual uint8_t versionMinor() { return 0; }
    virtual void begin() {}                // called after device.begin()
    virtual void handle(uint8_t cmd, GraftPayloadReader &payload, GraftContext &ctx) = 0;
    virtual void poll() {}                 // called every loop cycle
    virtual uint8_t pinCount() { return 0; }
    virtual const uint8_t* pins() { return nullptr; }
};
```

## Quick Module (Macro Pattern)

```cpp
#define GRAFT_MODULE_MYMODULE
// In your .h file:

GRAFT_MODULE(MyModule) {
public:
    const char* name() override { return "mymod"; }

    void handle(uint8_t cmd, GraftPayloadReader &payload, GraftContext &ctx) override {
        GRAFT_ON_CMD(0x01) {
            uint8_t value = payload.readUInt8();
            // Do something with value
            ctx.ack();
        }
        GRAFT_ON_CMD(0x02) {
            uint8_t buf[4];
            GraftPayloadWriter w(buf, sizeof(buf));
            w.writeFloat32(42.0f);
            ctx.sendModResp(0, buf, w.length());
        }
    }
};
```

## Registration

```cpp
void setup() {
    device.addModule(new MyModule());
    device.begin();
}
```

Modules are indexed by registration order. The first module added is ID 0, second is ID 1, etc. Module IDs appear in HELLO_RESP.

## Context API

The `GraftContext` passed to `handle()` provides:

| Method | Description |
|---|---|
| `ctx.ack()` | Send ACK response |
| `ctx.nak(errorCode)` | Send NAK response |
| `ctx.sendModResp(id, data, len)` | Send typed response data |
| `ctx.emitModEvent(id, code, data, len)` | Emit unsolicited event |

If the handler doesn't call any of these, GraftDevice auto-sends ACK.

## Poll

Modules with continuous behavior (stepper pulse generation, PID loop) implement `poll()`. It's called every `device.poll()` cycle — keep it fast and non-blocking.

## Compile Guards

Each module is gated by a compile-time define:

```cpp
#define GRAFT_MODULE_SERVO
#define GRAFT_MODULE_NEOPIXEL
#define GRAFT_MODULE_ENCODER
#include <Graft.h>
```

This keeps binary size minimal on constrained MCUs like ATmega328P.
