---
title: "ESP32"
description: "ESP32 board guide — WiFi, BLE, MQTT, OTA, and all CONDUYT transports."
---

# ESP32

The ESP32 is the most fully-featured CONDUYT target. It supports every transport (Serial, BLE, MQTT, TCP, WebSocket) and has WiFi for network-connected projects.

## Specs

| Property | Value |
|----------|-------|
| MCU | Xtensa LX6, dual-core, 240 MHz |
| Flash | 4 MB |
| RAM | 520 KB SRAM |
| GPIO | 34 pins |
| ADC | 18 channels (12-bit) |
| PWM | 16 channels |
| I2C | 2 buses |
| SPI | 3 buses |
| UART | 3 ports |
| WiFi | 802.11 b/g/n |
| BLE | 4.2 |
| OTA | Supported |
| PlatformIO env | `esp32dev` |

## Firmware setup

### PlatformIO

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = conduyt
build_flags = -DCONDUYT_OTA
```

### Arduino IDE

Select **ESP32 Dev Module** from **Tools → Board → ESP32 Arduino**. If you don't see ESP32 boards, add `https://espressif.github.io/arduino-esp32/package_esp32_index.json` to **File → Preferences → Additional Board Manager URLs**, then install the ESP32 package from **Tools → Board → Boards Manager**.

## Transports

The ESP32 supports all CONDUYT transports:

```cpp
// Serial (USB)
ConduytSerial transport(Serial, 115200);

// BLE
ConduytBLE transport("MyDevice");

// MQTT (requires WiFi)
#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient wifi;
PubSubClient mqtt(wifi);
ConduytMQTT transport(mqtt, "device-001");

// TCP (requires WiFi)
WiFiServer server(3333);
ConduytTCP transport(server);
```

## Built-in LED

The built-in LED is on **pin 2** on most ESP32 dev boards. Some boards (ESP32-S3) use a NeoPixel instead.

## Browser flashing

Go to the [Playground](/playground), click **Flash**, select **ESP32**, and click **Install CONDUYT Firmware**. The esp-web-tools installer auto-detects your chip family and handles bootloader, partitions, and firmware.

## OTA updates

Enable OTA in your build flags with `-DCONDUYT_OTA`. The ESP32 supports over-the-air firmware updates via the `OTA_BEGIN`, `OTA_CHUNK`, and `OTA_FINALIZE` commands.

## Common issues

- **Upload fails**: Hold the **BOOT** button while uploading. Some boards auto-reset, others don't.
- **WiFi won't connect**: Check that your SSID and password are correct. The ESP32 only supports 2.4 GHz networks.
- **BLE disconnects**: BLE connections can be unstable if the ESP32 is also running WiFi. Consider using one or the other.
- **ADC accuracy**: The ESP32's ADC is 12-bit but nonlinear. For precision measurements, use an external ADC.
