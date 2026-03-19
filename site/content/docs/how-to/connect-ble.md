---
title: Connect over BLE
description: Set up Bluetooth Low Energy communication between a host and a Conduyt device
---

# Connect over BLE

BLE transport enables wireless communication without a network or broker. It works with ESP32 and nRF52 boards on the firmware side, and WebBluetooth or native BLE APIs on the host side.

## Firmware Setup

```cpp
#define CONDUYT_TRANSPORT_BLE
#include <Conduyt.h>

ConduytBLE transport("MyDevice");
ConduytDevice device("BLEDevice", "1.0.0", transport);

void setup() {
    device.begin();
}

void loop() {
    device.poll();
}
```

The string passed to `ConduytBLE` is the advertised device name. Keep it short; BLE advertisement packets have limited space.

### Supported Boards

- **ESP32** (all variants: ESP32, ESP32-S3, ESP32-C3)
- **nRF52** (nRF52832, nRF52840)

## BLE Service Details

Conduyt uses the Nordic UART Service (NUS) for data transfer. BLE is a byte stream, so Conduyt applies COBS framing to delimit packets. The transport layer handles this; you do not need to frame packets yourself.

## JavaScript (Browser via WebBluetooth)

```typescript
import { BLETransport } from 'conduyt-js/transports/ble'
import { ConduytDevice } from 'conduyt-js'

const transport = new BLETransport()

const device = new ConduytDevice(transport)
await device.connect()  // triggers browser device picker
```

`device.connect()` calls `navigator.bluetooth.requestDevice()` under the hood. This requires a user gesture (button click). The browser displays a picker filtered to devices advertising the NUS service UUID.

### Full Browser Example

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Conduyt BLE Demo</title>
</head>
<body>
  <button id="connect">Connect</button>
  <button id="toggle" disabled>Toggle LED</button>
  <pre id="log"></pre>

  <script type="module">
    import { BLETransport } from 'conduyt-js/transports/ble'
    import { ConduytDevice } from 'conduyt-js'

    const connectBtn = document.getElementById('connect')
    const toggleBtn = document.getElementById('toggle')
    const log = document.getElementById('log')

    let device = null
    let ledOn = false

    function print(msg) {
      log.textContent += msg + '\n'
    }

    connectBtn.addEventListener('click', async () => {
      try {
        const transport = new BLETransport()
        device = new ConduytDevice(transport)
        await device.connect()

        print('Connected to: ' + device.capabilities.firmwareName)
        await device.pin(13).mode('output')
        print('Pin 13 set to output')

        toggleBtn.disabled = false
        connectBtn.disabled = true

        device.on('disconnect', () => {
          print('Disconnected')
          toggleBtn.disabled = true
          connectBtn.disabled = false
          device = null
          ledOn = false
        })
      } catch (err) {
        print('Connection failed: ' + err.message)
      }
    })

    toggleBtn.addEventListener('click', async () => {
      if (!device) return

      ledOn = !ledOn
      await device.pin(13).write(ledOn ? 1 : 0)
      print('LED ' + (ledOn ? 'ON' : 'OFF'))
    })
  </script>
</body>
</html>
```

## Swift (iOS/macOS)

ConduytKit wraps CoreBluetooth and handles NUS service discovery, COBS framing, and packet parsing.

```swift
import ConduytKit

let device = ConduytBLEDevice()

// Scans for Conduyt devices and connects to the first one found.
// On iOS, this triggers the system Bluetooth permission prompt on first use.
let capabilities = try await device.connect()
print("Connected: \(capabilities.firmwareName)")

// Pin control
try await device.pin(13).mode(.output)
try await device.pin(13).write(1)
let value = try await device.pin(0).read()

try await device.disconnect()
```

If you are using CoreBluetooth directly without ConduytKit, scan for the NUS service UUID, subscribe to notifications on the TX characteristic (`6E400003-...`), and write commands to the RX characteristic (`6E400002-...`). All data must pass through COBS decode/encode since BLE is a byte stream transport.

### NUS UUIDs

| Characteristic | UUID |
|---|---|
| Service | `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` |
| TX (device to host) | `6E400003-B5A3-F393-E0A9-E50E24DCCA9E` |
| RX (host to device) | `6E400002-B5A3-F393-E0A9-E50E24DCCA9E` |

## Troubleshooting

### Device not found in picker

Confirm the firmware is advertising. On ESP32, BLE advertising starts after `device.begin()`. If the device was previously bonded to another host, clear the bond on both sides.

### Connection drops frequently

BLE connections are sensitive to distance and interference. Keep the device within 5 meters for reliable communication. Reduce the connection interval if your board supports it.

### MTU size

The default BLE MTU is 20 bytes. Conduyt packets larger than the MTU are split across multiple BLE writes automatically by the transport. For better throughput, negotiate a larger MTU (up to 512 bytes) if both sides support it.
