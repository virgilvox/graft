# GRAFT Firmware

Arduino/PlatformIO library that turns any microcontroller into a GRAFT device. Handles packet framing, command dispatch, capability reporting, and module loading. The host controls everything; the firmware runs the protocol.

## Install

### PlatformIO

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
lib_deps = lumencanvas/Graft
```

### Arduino IDE

Install "Graft" from the Library Manager.

## Minimal Sketch

```cpp
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("MyDevice", "1.0.0", transport);

void setup() { device.begin(); }
void loop()  { device.poll(); }
```

This gives the host full pin control, I2C/SPI passthrough, and capability reporting.

## Adding Modules

Modules are compile-time opt-in. Define the feature flag before including `Graft.h`.

```cpp
#define GRAFT_MODULE_SERVO
#define GRAFT_MODULE_NEOPIXEL
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("ModuleDemo", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.addModule(new GraftModuleNeoPixel(6, 30)); // pin 6, 30 LEDs
  device.begin();
}

void loop() { device.poll(); }
```

## Adding Datastreams

Datastreams are named, typed values that the device pushes to the host.

```cpp
GraftSerial  transport(Serial, 115200);
GraftDevice  device("Thermostat", "1.0.0", transport);

void setup() {
  device.addDatastream("temperature", GRAFT_FLOAT32, "C", false);
  device.addDatastream("setpoint", GRAFT_FLOAT32, "C", true);
  device.onDatastreamWrite("setpoint", [](GraftPayloadReader &r, GraftContext &ctx) {
    float sp = r.readFloat32();
    // handle new setpoint
    ctx.ack();
  });
  device.begin();
}

void loop() {
  device.poll();
  // periodically:
  device.writeDatastream("temperature", 22.5f);
}
```

## Supported Boards

| Board | Platform | Buffer Size | Max Modules | Max Subscriptions |
|-------|----------|-------------|-------------|-------------------|
| Arduino Uno | AVR (328P) | 128 | 4 | 8 |
| Arduino Mega | AVR (2560) | 128 | 4 | 8 |
| ESP32 | Espressif32 | 512 | 8 | 16 |
| ESP8266 (NodeMCU) | Espressif8266 | 512 | 8 | 16 |
| nRF52840 DK | Nordic nRF52 | 512 | 8 | 16 |
| Raspberry Pi Pico | RP2040 | 512 | 8 | 16 |
| Teensy 4.1 | Teensy | 512 | 8 | 16 |
| SAMD21/51 | SAMD | 256 | 6 | 12 |

## Transports

| Transport | Class | Use Case |
|-----------|-------|----------|
| Serial (UART/USB) | `GraftSerial` | Wired connection, most common |
| USB Serial | `GraftUSBSerial` | Native USB CDC |
| BLE | `GraftBLE` | Bluetooth Low Energy (ESP32, nRF52) |
| MQTT | `GraftMQTT` | WiFi via MQTT broker |
| TCP | `GraftTCP` | Direct WiFi socket |
| CLASP | `GraftCLASP` | CLASP tunnel protocol |

## Modules

| Module | Flag | Class | Hardware |
|--------|------|-------|----------|
| Servo | `GRAFT_MODULE_SERVO` | `GraftModuleServo` | Hobby servos |
| NeoPixel | `GRAFT_MODULE_NEOPIXEL` | `GraftModuleNeoPixel` | WS2812/SK6812 LEDs |
| DHT | `GRAFT_MODULE_DHT` | `GraftModuleDHT` | DHT11/DHT22 sensors |
| OLED | `GRAFT_MODULE_OLED` | `GraftModuleOLED` | SSD1306 displays |
| Stepper | `GRAFT_MODULE_STEPPER` | `GraftModuleStepper` | Stepper motors |
| Encoder | `GRAFT_MODULE_ENCODER` | `GraftModuleEncoder` | Rotary encoders |
| PID | `GRAFT_MODULE_PID` | `GraftModulePID` | PID controller |
| I2C Passthrough | `GRAFT_MODULE_I2C_PASSTHROUGH` | `GraftModuleI2CPassthrough` | Raw I2C forwarding |

## Configuration

These constants are set per-platform in `GraftBoard.h`. Override them before including `Graft.h` if needed.

| Constant | Default | Description |
|----------|---------|-------------|
| `GRAFT_PACKET_BUF_SIZE` | 128-512 | Max payload size in bytes |
| `GRAFT_MAX_MODULES` | 4-8 | Module slot count |
| `GRAFT_MAX_SUBSCRIPTIONS` | 8-16 | Active pin subscription limit |
| `GRAFT_MAX_DATASTREAMS` | 4-16 | Datastream slot count |

## Running Tests

```bash
pio test -e native
```

Runs the Unity-based test suite against the core C library and device logic on the host machine. No hardware required.

## Examples

| Example | Description |
|---------|-------------|
| `BasicBlink` | Minimal sketch, pin control only |
| `ServoControl` | Servo module with serial transport |
| `DatastreamThermostat` | Datastreams with write callbacks |
| `MQTTSensor` | WiFi sensor over MQTT |
| `BLEDevice` | BLE transport on ESP32 |
| `FullKitchen` | Multiple modules and datastreams |

## License

MIT. Copyright (c) 2026 LumenCanvas.
