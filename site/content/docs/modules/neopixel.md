---
title: "NeoPixel Module"
description: "Control WS2812/SK6812 addressable RGB LED strips and rings."
---

# NeoPixel Module

Drive WS2812, SK6812, and compatible addressable LED strips with per-pixel color control.

## Firmware setup

```cpp
#include <Conduyt.h>

#define CONDUYT_MODULE_NEOPIXEL

ConduytSerial transport(Serial, 115200);
ConduytDevice device("LEDStrip", "1.0.0", transport);
ConduytNeoPixel neopixel;

void setup() {
  device.addModule(&neopixel);
  device.begin();
}

void loop() {
  device.poll();
}
```

## Host SDK usage

### JavaScript

```javascript
import { NeoPixel } from 'conduyt-js/modules/neopixel';

const strip = new NeoPixel(device, 0);

await strip.begin(6, 30);         // pin 6, 30 LEDs
await strip.setPixel(0, 255, 0, 0);  // pixel 0 = red
await strip.setPixel(1, 0, 255, 0);  // pixel 1 = green
await strip.show();                   // push to strip

await strip.fill(0, 0, 255);     // all blue
await strip.show();

await strip.setBrightness(128);  // 50% brightness
await strip.show();
```

### Python

```python
from conduyt.modules import NeoPixel

strip = NeoPixel(device, 0)
await strip.begin(6, 30)
await strip.set_pixel(0, 255, 0, 0)
await strip.show()
await strip.fill(0, 0, 255)
await strip.show()
```

## Commands

| Command | Byte | Payload | Description |
|---------|------|---------|-------------|
| Begin | `0x01` | `pin(1) + count(2)` | Initialize strip |
| SetPixel | `0x02` | `index(2) + r(1) + g(1) + b(1)` | Set pixel color |
| SetPixelW | `0x03` | `index(2) + r(1) + g(1) + b(1) + w(1)` | Set RGBW pixel |
| Fill | `0x04` | `r(1) + g(1) + b(1)` | Fill all pixels |
| Show | `0x05` | (none) | Push buffer to strip |
| Brightness | `0x06` | `level(1)` | Set brightness (0-255) |

## Wiring

Connect the data line to a digital pin (pin 6 is common). Power the strip from a 5V supply — do not power more than a few LEDs from the Arduino's 5V pin. Connect all grounds together. A 300-500 ohm resistor on the data line and a 1000 uF capacitor across the power supply are recommended for longer strips.

## Notes

- Call `show()` after setting pixels — changes are buffered until show is called.
- Each pixel uses ~3 bytes of RAM on the MCU. A 300-pixel strip uses ~900 bytes.
- On the Uno R3 (2 KB RAM), keep strips under ~50 pixels. ESP32 and Pico can handle hundreds.
- RGBW strips (SK6812) use `setPixelW` instead of `setPixel`.
