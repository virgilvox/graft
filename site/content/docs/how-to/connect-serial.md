---
title: Connect over Serial
description: Set up serial communication between a host and a Conduyt device in JavaScript, Python, and Go
---

# Connect over Serial

Serial is the default transport for Conduyt. It works over USB with no network configuration.

## Firmware Setup

```cpp
#include <Conduyt.h>

ConduytSerial transport(Serial, 115200);
ConduytDevice device("MyDevice", "1.0.0", transport);

void setup() {
    Serial.begin(115200);
    device.begin();
}

void loop() {
    device.poll();
}
```

The default baud rate is 115200. Both sides must match.

## Find Your Serial Port

| OS | Typical Port |
|---|---|
| Linux | `/dev/ttyUSB0` or `/dev/ttyACM0` |
| macOS | `/dev/cu.usbmodem*` (e.g. `/dev/cu.usbmodem14101`) |
| Windows | `COM3`, `COM4`, etc. |

On Linux and macOS, list connected devices:

```bash
ls /dev/tty* | grep -i usb
# or
ls /dev/cu.*
```

On Windows, open Device Manager and check "Ports (COM & LPT)".

## JavaScript (Node.js)

Save as `connect.mjs` and run with `node connect.mjs`:

```js
import { SerialTransport } from 'conduyt-js/transports/serial'
import { ConduytDevice } from 'conduyt-js'

const transport = new SerialTransport({
    path: '/dev/ttyUSB0',
    baudRate: 115200
})

const device = new ConduytDevice(transport)
const hello = await device.connect()

console.log('Firmware:', hello.firmwareName)
console.log('Pin count:', hello.pinCount)

const pin13 = device.pin(13)
await pin13.mode('output')
await pin13.write(1)
console.log('Pin 13 set HIGH')

await device.disconnect()
console.log('Disconnected')
```

### Browser (WebSerial)

WebSerial requires a user gesture to trigger the port picker. Call `device.connect()` inside a click handler:

```js
import { WebSerialTransport } from 'conduyt-js/transports/web-serial'
import { ConduytDevice } from 'conduyt-js'

document.getElementById('connectBtn').addEventListener('click', async () => {
    const transport = new WebSerialTransport({ baudRate: 115200 })
    const device = new ConduytDevice(transport)
    const hello = await device.connect() // triggers browser port picker

    console.log('Firmware:', hello.firmwareName)
    console.log('Pin count:', hello.pinCount)

    await device.pin(13).mode('output')
    await device.pin(13).write(1)
    console.log('Pin 13 set HIGH')

    await device.disconnect()
})
```

## Python

Requires `pip install conduyt-py[serial]`.

```python
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.serial import SerialTransport


async def main():
    transport = SerialTransport('/dev/ttyUSB0', baudrate=115200)
    device = ConduytDevice(transport)
    hello = await device.connect()

    print(f"Firmware: {hello.firmware_name}")
    print(f"Pin count: {hello.pin_count}")

    await device.pin(13).mode("output")
    await device.pin(13).write(1)
    print("Pin 13 set HIGH")

    await device.disconnect()
    print("Disconnected")


asyncio.run(main())
```

## Go

```go
package main

import (
    "context"
    "fmt"
    "log"
    "time"

    "go.bug.org/serial"
    conduyt "github.com/virgilvox/conduyt/sdk/go"
)

func main() {
    port, err := serial.Open("/dev/ttyUSB0", &serial.Mode{BaudRate: 115200})
    if err != nil {
        log.Fatal(err)
    }

    ctx := context.Background()
    transport := conduyt.NewSerialTransport(port)
    device := conduyt.NewDevice(transport, 5*time.Second)

    hello, err := device.Connect(ctx)
    if err != nil {
        log.Fatal(err)
    }
    fmt.Println("Firmware:", hello.FirmwareName)
    fmt.Println("Pin count:", hello.PinCount)

    if err := device.Pin(13).Mode(ctx, conduyt.PinModeOutput); err != nil {
        log.Fatal(err)
    }
    if err := device.Pin(13).Write(ctx, 1); err != nil {
        log.Fatal(err)
    }
    fmt.Println("Pin 13 set HIGH")

    if err := device.Disconnect(ctx); err != nil {
        log.Fatal(err)
    }
    fmt.Println("Disconnected")
}
```

## Troubleshooting

### Permission denied (Linux)

Add your user to the `dialout` group:

```bash
sudo usermod -aG dialout $USER
```

Log out and back in for the change to take effect.

### Port busy

Another process has the port open. Close any serial monitors (Arduino IDE, screen, minicom) before connecting. Only one process can hold a serial port at a time.

### No data received

Verify both sides use the same baud rate. A mismatch produces garbled data or silence. The firmware and host transport must both specify 115200 (or whatever rate you choose).

### Device not detected

Check the USB cable. Some cables are charge-only and do not carry data. Try a different cable if the port does not appear in `/dev` or Device Manager.
