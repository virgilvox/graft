---
title: "Troubleshooting"
description: "Common CONDUYT issues and how to fix them."
---

# Troubleshooting

## Connection issues

### "Timeout waiting for response"

The host sent a command but didn't receive a response within the timeout window.

**Causes:**
- Wrong serial port selected
- Wrong baud rate (must match firmware, default 115200)
- Firmware not running (board not flashed, or stuck in bootloader)
- Serial monitor or another process has the port open

**Fix:** Close any serial monitors, verify the port path, and ensure the firmware is uploaded and running. Try pressing the board's RESET button.

### "Not connected" error

You tried to send a command before calling `device.connect()` or before the HELLO handshake completed.

**Fix:** Always `await device.connect()` before sending commands.

### Serial port not found

**macOS:** Look for `/dev/cu.usbmodem*` or `/dev/cu.usbserial-*`. If nothing appears, check System Information → USB.

**Linux:** Look for `/dev/ttyACM0` or `/dev/ttyUSB0`. You may need to add your user to the `dialout` group:
```bash
sudo usermod -a -G dialout $USER
# log out and back in
```

**Windows:** Check Device Manager → Ports (COM & LPT). Install drivers if needed (FTDI, CH340, or CDC).

## Protocol errors

### "NAK: INVALID_PIN"

The pin number you specified doesn't exist on the board, or the pin doesn't support the requested mode.

**Fix:** Check the board's pin map. Not all pins support all modes (e.g., analog input is only on specific pins).

### "NAK: PIN_MODE_UNSUPPORTED"

The firmware doesn't support the requested pin mode on that pin (e.g., PWM on a non-PWM pin).

**Fix:** Check which pins support PWM, analog, and interrupt on your board.

### "NAK: MODULE_NOT_LOADED"

You sent a module command but the firmware doesn't have that module enabled.

**Fix:** Make sure you've `#define`d the module flag and called `device.addModule()` in your firmware sketch.

### "CRC mismatch"

The packet's CRC8 checksum doesn't match. The data was corrupted in transit.

**Causes:**
- Electrical noise on the serial line
- Baud rate mismatch between host and firmware
- Buffer overflow (payload too large)

**Fix:** Verify baud rates match. Use shorter cables. Reduce payload size if near the limit.

## Firmware issues

### Sketch compiles but board doesn't respond

The firmware might be crashing or stuck. Common causes:

- Calling blocking functions in `loop()` (use `device.poll()` which is non-blocking)
- Running out of RAM (especially on Uno R3 with 2 KB)
- I2C or SPI bus locked (sensor not responding)

**Fix:** Add `Serial.println("alive")` in `loop()` to verify the firmware is running. Strip modules to the minimum and add them back one at a time.

### "not in sync" during upload (AVR boards)

The Arduino IDE can't communicate with the bootloader.

**Fix:** Select the correct board and port. Hold RESET, release it, and immediately click Upload. On some clones, you need CH340 drivers.

## Playground issues

### "WebSerial not supported"

WebSerial requires Chrome or Edge on desktop. Firefox and Safari don't support it.

### "No DFU device found" (Uno R4)

The board isn't in DFU mode. Double-tap RESET quickly — the "L" LED should pulse.

### Code runs but nothing happens on the board

Make sure you called `await device.connect()` in your playground code. Check the Console panel for error messages.

## Performance

### Slow response times

- **Serial**: 115200 baud gives ~11 KB/s. For faster throughput, increase baud rate (230400, 460800, 921600).
- **BLE**: Limited to ~20 bytes per packet (BLE MTU). Larger payloads are fragmented.
- **MQTT**: Add latency from the broker hop. Use direct serial or TCP for low-latency control.

### PIN_SUBSCRIBE flooding

If you subscribe to a pin with a very short interval (e.g., 10ms), the device will send events rapidly. This can overwhelm the serial buffer.

**Fix:** Use a reasonable interval (100ms+) and set a threshold to only send events when the value changes significantly.
