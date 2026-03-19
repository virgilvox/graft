# GRAFT

[![Firmware CI](https://github.com/virgilvox/graft/actions/workflows/firmware-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/firmware-ci.yml)
[![JS CI](https://github.com/virgilvox/graft/actions/workflows/js-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/js-ci.yml)
[![Python CI](https://github.com/virgilvox/graft/actions/workflows/py-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/py-ci.yml)
[![Go CI](https://github.com/virgilvox/graft/actions/workflows/go-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/go-ci.yml)
[![Rust CI](https://github.com/virgilvox/graft/actions/workflows/rust-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/rust-ci.yml)
[![Swift CI](https://github.com/virgilvox/graft/actions/workflows/swift-ci.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/swift-ci.yml)
[![Conformance](https://github.com/virgilvox/graft/actions/workflows/conformance.yml/badge.svg)](https://github.com/virgilvox/graft/actions/workflows/conformance.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

Open binary protocol for host-to-device hardware control.

GRAFT replaces ad-hoc serial text commands with a structured, transport-agnostic binary protocol. One firmware library handles framing, dispatch, and capabilities. Host SDKs in five languages connect over Serial, BLE, MQTT, WebSocket, or TCP. Flash a sketch, connect from any language, control pins, read sensors, drive modules.

## Quick Start

### Arduino Firmware

```cpp
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("MyDevice", "1.0.0", transport);

void setup() { device.begin(); }
void loop()  { device.poll(); }
```

### JavaScript Host

```js
import { GraftDevice } from 'graft-js'
import { SerialTransport } from 'graft-js/transports/serial'

const device = await GraftDevice.connect(
  new SerialTransport({ path: '/dev/ttyUSB0' })
)
await device.pin(13).mode('output')
await device.pin(13).write(1)
const value = await device.pin(A0).read('analog')
await device.disconnect()
```

### Python Host

```python
from graft import GraftDevice
from graft.transports.serial import SerialTransport

device = GraftDevice(SerialTransport("/dev/ttyUSB0"))
await device.connect()
await device.pin(13).mode("output")
await device.pin(13).write(1)
value = await device.pin(0).read("analog")
await device.disconnect()
```

## What GRAFT Does

- Binary packet protocol with CRC8, COBS framing, and sequence tracking
- Pin control (digital, analog, PWM), pin subscriptions with configurable interval and threshold
- I2C, SPI passthrough for direct bus access from the host
- Module system for reusable hardware drivers (Servo, NeoPixel, DHT, OLED, Stepper, Encoder, PID)
- Datastream abstraction for named, typed, subscribable device telemetry
- OTA firmware updates over any transport
- High-speed analog streaming with configurable sample rate

## Supported Hardware

| Board | Platform | OTA |
|-------|----------|-----|
| Arduino Uno | AVR (ATmega328P) | No |
| ESP32 DevKit | Espressif32 | Yes |
| NodeMCU v2 | ESP8266 | No |
| nRF52840 DK | Nordic nRF52 | No |
| Raspberry Pi Pico | RP2040 | No |
| Teensy 4.1 | Teensy | No |

## SDKs

| Language | Package | Install | Status |
|----------|---------|---------|--------|
| JavaScript/TypeScript | `graft-js` | `npm install graft-js` | v0.1.0 |
| Python | `graft-py` | `pip install graft-py` | v0.1.0 |
| Go | `graft-go` | `go get github.com/graft-io/graft-go` | v0.1.0 |
| Rust | `graft` | `cargo add graft` | v0.1.0 |
| Swift | `GraftKit` | Swift Package Manager | v0.1.0 |

## Wire Format

Every GRAFT packet is 8 bytes of header followed by a variable-length payload.

```
 0      1      2      3      4      5      6      7
+------+------+------+------+------+------+------+------+----------+
| 0x47 | 0x46 | VER  | TYPE | SEQ  | LEN (u16le) | CRC8 | PAYLOAD  |
+------+------+------+------+------+------+------+------+----------+
  magic  magic   1     cmd/   0-255  payload len   header   0..N
  'G'    'F'           evt                         crc      bytes
```

CRC8 covers bytes 0 through 6. Transports that need framing (Serial, BLE) wrap each packet in COBS encoding with a 0x00 delimiter.

## Project Structure

```
graft/
  firmware/         Arduino/PlatformIO library (C++)
    src/graft/      Device, transports, modules, core wire/cobs/crc
    examples/       Ready-to-flash sketches
  sdk/
    js/             JavaScript/TypeScript SDK
    python/         Python SDK (async + sync)
    go/             Go SDK
    rust/           Rust SDK (no_std core + std device)
    swift/          Swift SDK (GraftKit)
  broker/           MQTT broker (Mosquitto, Docker)
  conformance/      Cross-SDK test vectors
  protocol/         constants.json (source of truth)
  site/             Documentation site (Nuxt)
```

## Documentation

Full documentation at [github.com/virgilvox/graft](https://github.com/virgilvox/graft).

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT. Copyright (c) 2026 LumenCanvas.
