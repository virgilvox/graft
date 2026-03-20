---
title: "Arduino Uno R4 Minima"
description: "Uno R4 board guide — Renesas RA4M1, USB DFU flashing, and serial transport."
---

# Arduino Uno R4 Minima

The Uno R4 Minima uses the Renesas RA4M1 (ARM Cortex-M4) — a significant upgrade from the classic Uno's ATmega328P. It has more memory, a faster clock, and USB DFU flashing support for the browser [Playground](/playground).

## Specs

| Property | Value |
|----------|-------|
| MCU | Renesas RA4M1 (ARM Cortex-M4), 48 MHz |
| Flash | 256 KB |
| RAM | 32 KB SRAM |
| GPIO | 20 pins |
| ADC | 6 channels (14-bit) |
| PWM | 6 channels |
| I2C | 1 bus |
| SPI | 1 bus |
| UART | 1 port |
| WiFi | No (use Uno R4 WiFi variant) |
| BLE | No |
| OTA | No |
| PlatformIO env | `uno_r4_minima` |

## Firmware setup

### PlatformIO

```ini
[env:uno_r4_minima]
platform = renesas-ra
board = uno_r4_minima
framework = arduino
lib_deps = conduyt
```

### Arduino IDE

Select **Arduino Uno R4 Minima** from **Tools → Board → Arduino Renesas Boards**. If you don't see it, install the **Arduino Renesas fsp Boards** package from **Tools → Board → Boards Manager**.

## Transport

The Uno R4 connects over USB serial:

```cpp
#include <Conduyt.h>

ConduytSerial  transport(Serial, 115200);
ConduytDevice  device("MyDevice", "1.0.0", transport);

void setup() { device.begin(); }
void loop()  { device.poll(); }
```

## Browser flashing (DFU)

The Uno R4 has a USB DFU bootloader, which means it can be flashed directly from the browser using WebUSB.

1. **Double-tap the RESET button** — the "L" LED will start pulsing, indicating DFU mode
2. Go to the [Playground](/playground) and click **Flash**
3. Select **Arduino Uno R4** and click **Flash Arduino Uno R4**
4. Select the Arduino DFU device from the browser prompt
5. Wait for the progress bar to complete

The board will automatically reboot into the new firmware.

## Built-in LED

The built-in LED is on **pin 13**, same as the classic Uno.

## Differences from Uno R3

| Feature | Uno R3 | Uno R4 Minima |
|---------|--------|---------------|
| MCU | ATmega328P (AVR, 16 MHz) | RA4M1 (ARM, 48 MHz) |
| Flash | 32 KB | 256 KB |
| RAM | 2 KB | 32 KB |
| ADC | 6ch, 10-bit | 6ch, 14-bit |
| USB | FTDI/16U2 serial | Native USB + DFU |
| Browser flash | No (needs avrdude) | Yes (WebUSB DFU) |
| Flashing protocol | STK500v1 | USB DFU 1.1 |

## Common issues

- **DFU mode not entering**: Make sure you double-tap RESET quickly (within ~500ms). A single tap just resets the board.
- **WebUSB not finding device**: On Windows, you may need to install WinUSB drivers. On macOS and Linux, it should work out of the box.
- **Serial port disappears after DFU flash**: This is normal. After flashing, single-tap RESET to reboot into the new firmware. The serial port will reappear.
