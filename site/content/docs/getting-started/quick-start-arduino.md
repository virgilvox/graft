---
title: 'Quick Start: Arduino'
description: Get a GRAFT device running in under 5 minutes
---

# Quick Start: Arduino

## Install

Add GRAFT to your Arduino project via the Library Manager or PlatformIO:

```ini
lib_deps = graft
```

## Minimal Sketch

```cpp
#include <Graft.h>

GraftSerial transport(Serial, 115200);
GraftDevice device("MyBoard", "1.0.0", transport);

void setup() {
  device.begin();
}

void loop() {
  device.poll();
}
```

This gives you full pin control (digital, analog, PWM) from any GRAFT host SDK — no extra code needed.

## Adding Modules

Enable hardware modules with compile-time defines **before** including Graft.h:

```cpp
#define GRAFT_MODULE_SERVO
#define GRAFT_MODULE_NEOPIXEL
#include <Graft.h>

GraftSerial transport(Serial, 115200);
GraftDevice device("LedServo", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.addModule(new GraftModuleNeoPixel());
  device.begin();
}

void loop() {
  device.poll();
}
```

## Adding Datastreams

Push device-side values to the host:

```cpp
device.addDatastream("temperature", GRAFT_TYPE_FLOAT32, "celsius", false);
device.addDatastream("setpoint", GRAFT_TYPE_FLOAT32, "celsius", true);
device.onDatastreamWrite("setpoint", onSetpoint);
```

## Supported Boards

| Board | Packet Buffer | Modules | Notes |
|---|---|---|---|
| Arduino Uno | 128 B | 4 | Limited RAM — keep modules minimal |
| ESP32 | 512 B | 8 | WiFi + BLE transports available |
| ESP8266 | 512 B | 8 | WiFi transport, no BLE |
| nRF52840 | 512 B | 8 | BLE + USB CDC |
| Raspberry Pi Pico | 512 B | 8 | USB CDC |
| Teensy 4.1 | 512 B | 8 | USB CDC, high-speed |

## Next Steps

- [Transport Adapters](/docs/firmware/transport-adapters) — use BLE, MQTT, or TCP
- [Module System](/docs/firmware/module-system) — build custom modules
- [JavaScript SDK](/docs/getting-started/quick-start-js) — control from the host
