---
title: "Raspberry Pi Pico"
description: "RP2040 Pico board guide — dual-core, PIO, and USB serial transport."
---

# Raspberry Pi Pico

The Raspberry Pi Pico features the RP2040 — a dual-core ARM Cortex-M0+ with generous RAM and the unique PIO (Programmable I/O) subsystem. Great for projects needing precise timing or multiple serial buses.

## Specs

| Property | Value |
|----------|-------|
| MCU | RP2040, dual-core ARM Cortex-M0+, 133 MHz |
| Flash | 2 MB |
| RAM | 264 KB SRAM |
| GPIO | 26 pins |
| ADC | 3 channels (12-bit) |
| PWM | 16 channels |
| I2C | 2 buses |
| SPI | 2 buses |
| UART | 2 ports |
| WiFi | No (use Pico W variant) |
| BLE | No (use Pico W variant) |
| OTA | No |
| PlatformIO env | `pico` |

## Firmware setup

### PlatformIO

```ini
[env:pico]
platform = raspberrypi
board = pico
framework = arduino
lib_deps = conduyt
```

### Arduino IDE

Install the **Raspberry Pi Pico/RP2040** boards package from **Tools → Board → Boards Manager** (search "pico"). Select **Raspberry Pi Pico** from the board list.

## Transport

USB serial:

```cpp
#include <Conduyt.h>

ConduytSerial  transport(Serial, 115200);
ConduytDevice  device("PicoDevice", "1.0.0", transport);

void setup() { device.begin(); }
void loop()  { device.poll(); }
```

## Browser flashing

1. Hold the **BOOTSEL** button while plugging in USB
2. The Pico appears as a USB mass storage device
3. Go to the [Playground](/playground), click **Flash**, select **Raspberry Pi Pico**

## Built-in LED

Pin **25** (directly wired, no PWM). On the Pico W, the LED is connected via the WiFi chip and requires `LED_BUILTIN`.

## Common issues

- **No serial port appearing**: The Pico's USB serial only appears after firmware that uses `Serial` is running. Flash via BOOTSEL first.
- **Upload fails in Arduino IDE**: Hold BOOTSEL, plug in, release BOOTSEL, then upload. Some IDE versions auto-detect BOOTSEL mode.
