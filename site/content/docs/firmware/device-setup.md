---
title: Device Setup
description: Setting up GRAFT firmware on your microcontroller
---

# Device Setup

## Minimal Sketch

```cpp
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("MyBoard", "1.0.0", transport);

void setup() {
  device.begin();
}

void loop() {
  device.poll();  // non-blocking, one packet per call
}
```

That's it. The device will respond to HELLO, PING, and all pin control commands.

## Adding Modules

Enable modules with `#define` before including Graft.h:

```cpp
#define GRAFT_MODULE_SERVO
#define GRAFT_MODULE_NEOPIXEL
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("MyBoard", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.addModule(new GraftModuleNeoPixel());
  device.begin();
}
```

## Adding Datastreams

Datastreams are named, typed channels for higher-level data:

```cpp
device.addDatastream("temperature", GRAFT_FLOAT32, "celsius", false);  // read-only
device.addDatastream("setpoint",    GRAFT_FLOAT32, "celsius", true);   // writable

// Handle writes from the host
device.onDatastreamWrite("setpoint", [](GraftPayloadReader &payload, GraftContext &ctx) {
  float val = payload.readFloat32();
  pid.setTarget(val);
  ctx.ack();
});

// Push values from the device
void loop() {
  device.poll();
  if (tempTimer.elapsed()) {
    device.writeDatastream("temperature", sensor.readTemp());
  }
}
```

## Supported Boards

GRAFT auto-detects your board and configures memory limits:

| Board | RAM Budget | Max Modules | Max Subs | Max Datastreams |
|---|---|---|---|---|
| ATmega328 (Uno) | 128B buf | 4 | 8 | 4 |
| ESP32 | 512B buf | 8 | 16 | 16 |
| ESP8266 | 512B buf | 8 | 16 | 16 |
| nRF52 | 512B buf | 8 | 16 | 16 |
| RP2040 (Pico) | 512B buf | 8 | 16 | 16 |
| STM32 | 512B buf | 8 | 16 | 16 |
| SAMD | 256B buf | 6 | 12 | 8 |
| Teensy | 512B buf | 8 | 16 | 16 |

All limits are overridable via `#define` before including Graft.h.

## Transport Options

```cpp
// Serial (all boards)
GraftSerial transport(Serial, 115200);

// MQTT (ESP32/8266 with WiFi)
WiFiClient wifi;
GraftMQTT transport(wifi, "mqtt://broker.local", 1883, "my-device");

// BLE (ESP32)
GraftBLE transport("GRAFT-Device");

// TCP (any board with Client)
WiFiClient client;
GraftTCP transport(client, "192.168.1.100", 3000);
```

The transport accepts any Arduino `Client&` — your networking hardware is irrelevant to the protocol.
