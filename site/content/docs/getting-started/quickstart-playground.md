---
title: "Quick Start: Browser Playground"
description: "Flash firmware and control a board from your browser — no installs required."
---

# Quick Start: Browser Playground

The [CONDUYT Playground](/playground) lets you flash firmware, write code, and control hardware entirely from your browser. No IDE, no CLI, no drivers.

## What you need

- A supported board (ESP32, Arduino Uno R4, or Raspberry Pi Pico)
- USB cable
- Chrome or Edge (WebSerial and WebUSB require Chromium-based browsers)

## 1. Open the Playground

Go to [conduyt.io/playground](/playground). You'll see three panels:

- **Code Editor** (left) — Monaco editor with CONDUYT API autocomplete
- **Console** (top right) — log output from your code
- **Device** (bottom right) — connection status and board info

## 2. Flash firmware

Click **Flash** in the toolbar. Select your board:

### ESP32
Click **Install CONDUYT Firmware**. Chrome will prompt you to select a serial port — pick your ESP32. The installer handles partitions, bootloader, and firmware automatically.

### Arduino Uno R4
**Double-tap the RESET button** on your board — the "L" LED should start pulsing. Click **Flash Arduino Uno R4**. Chrome will prompt for a USB device — select the Arduino DFU device. The progress bar shows flash progress.

### Raspberry Pi Pico
Hold the **BOOTSEL** button while plugging in USB. Click **Flash Raspberry Pi Pico**.

## 3. Connect

Close the flash panel. Click **Connect** in the toolbar. Select your board's serial port from the browser prompt. The Device panel should show "Connected" with your firmware name and pin count.

## 4. Run code

The editor comes pre-loaded with a Blink example. Click **Run** (▶). You should see:

```
[runner] Connected: BasicBlink
LED ON
LED OFF
LED ON
...
[runner] Finished
```

And pin 13's LED blinking on your board.

## 5. Try other examples

Use the **Examples** dropdown in the toolbar to load different sketches:

- **Blink** — toggle LED on/off
- **Read Sensor** — read analog values
- **Ping** — test the connection
- **PWM Fade** — smooth LED brightness control
- **Raw Packets** — low-level protocol demo (no board needed)

## Available APIs

In the editor, you have access to:

### `device` — High-level board control
```javascript
await device.connect()
await device.pin(13).mode('output')
await device.pin(13).write(1)
const val = await device.pin(36).read()
await device.ping()
await device.reset()
```

### `conduyt` — Low-level WASM protocol functions
```javascript
const CMD = conduyt.getCMD()
const pkt = conduyt.makePacket(CMD.PING, 0)
const wire = conduyt.wireEncode(pkt)
const cobs = conduyt.cobsEncode(wire)
```

### `log(...)` — Print to the console panel
### `sleep(ms)` — Async delay

Type `device.` or `conduyt.` in the editor to see full autocomplete.

## Limitations

- WebSerial requires Chrome or Edge (not Firefox or Safari)
- The board must be connected via USB (no wireless from the playground — use the native SDKs for BLE/MQTT)
- Code runs in the main browser thread — long-running loops should use `await sleep()` to stay responsive
- Click **Stop** (■) to halt running code

## Next steps

- [Arduino IDE Quick Start](/docs/getting-started/quickstart-arduino-ide) — develop locally with full toolchain
- [PlatformIO Quick Start](/docs/tutorials/first-blink) — advanced build system with multi-board support
- [WASM SDK](/docs/sdks/wasm) — use the WASM module in your own web apps
