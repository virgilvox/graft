---
title: "Arduino Uno R3"
description: "Classic Arduino Uno board guide — ATmega328P, serial transport, and getting started."
---

# Arduino Uno R3

The classic Arduino Uno with the ATmega328P. The most widely available Arduino board and a good choice for learning CONDUYT basics. Limited memory means you'll want to keep module usage minimal.

## Specs

| Property | Value |
|----------|-------|
| MCU | ATmega328P (AVR), 16 MHz |
| Flash | 32 KB (0.5 KB bootloader) |
| RAM | 2 KB SRAM |
| GPIO | 20 pins (14 digital + 6 analog) |
| ADC | 6 channels (10-bit) |
| PWM | 6 channels (pins 3, 5, 6, 9, 10, 11) |
| I2C | 1 bus (SDA=A4, SCL=A5) |
| SPI | 1 bus (pins 10-13) |
| UART | 1 port (pins 0, 1) |
| WiFi | No |
| BLE | No |
| OTA | No |
| PlatformIO env | `uno` |

## Firmware setup

### Arduino IDE

Select **Arduino Uno** from **Tools → Board → Arduino AVR Boards**. This comes pre-installed with the Arduino IDE.

### PlatformIO

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = conduyt
```

## Transport

Serial only (over USB):

```cpp
#include <Conduyt.h>

ConduytSerial  transport(Serial, 115200);
ConduytDevice  device("MyDevice", "1.0.0", transport);

void setup() { device.begin(); }
void loop()  { device.poll(); }
```

## Built-in LED

Pin **13**. This is the standard Arduino LED pin used in all Blink examples.

## Memory constraints

With only 2 KB of RAM, be mindful of:

- **Module count**: Each module uses RAM for its state. Stick to 1-2 modules on the Uno.
- **Datastream count**: Each datastream descriptor uses ~20 bytes. Keep to 3-4 max.
- **Payload size**: The default max payload is 256 bytes, which uses a receive buffer of that size.
- **String operations**: Avoid `String` objects in firmware — use fixed-size char arrays.

To reduce RAM usage, you can lower the max payload size:

```cpp
#define CONDUYT_MAX_PAYLOAD 128
```

## Common issues

- **Upload fails with "not in sync"**: Make sure the correct port is selected. Close any serial monitors before uploading.
- **Runs out of RAM**: The sketch compiles but behaves erratically. Reduce modules, datastreams, or payload size.
- **Pin 0/1 conflict**: Pins 0 and 1 are the hardware UART (Serial). Don't use them for GPIO while using serial transport.
