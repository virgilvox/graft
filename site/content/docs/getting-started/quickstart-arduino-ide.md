---
title: "Quick Start: Arduino IDE"
description: "Install the Conduyt library, flash firmware, and blink an LED — all from the Arduino IDE."
---

# Quick Start: Arduino IDE

Get a CONDUYT device up and running in under 5 minutes using the Arduino IDE.

## What you need

- An Arduino board (Uno R3, Uno R4, ESP32, or any supported board)
- USB cable for your board
- [Arduino IDE 2](https://www.arduino.cc/en/software) installed
- Node.js 20+ (for the host script)

## 1. Install the Conduyt library

Open the Arduino IDE, go to **Sketch → Include Library → Manage Libraries**, and search for **Conduyt**. Click **Install**.

Alternatively, download the latest `Conduyt-x.x.x.zip` from the [GitHub Releases](https://github.com/virgilvox/conduyt/releases) page and install via **Sketch → Include Library → Add .ZIP Library**.

## 2. Flash the BasicBlink sketch

Go to **File → Examples → Conduyt → BasicBlink**. This opens a minimal sketch:

```cpp
#include <Conduyt.h>

ConduytSerial  transport(Serial, 115200);
ConduytDevice  device("BasicBlink", "1.0.0", transport);

void setup() {
  device.begin();
}

void loop() {
  device.poll();
}
```

Select your board from **Tools → Board**, pick the correct port from **Tools → Port**, and click **Upload** (→ arrow button).

Once uploaded, the device is running CONDUYT firmware and waiting for a host to connect.

## 3. Install the JavaScript SDK

Open a terminal and create a project:

```bash
mkdir my-conduyt-project && cd my-conduyt-project
npm init -y
npm install conduyt-js serialport
```

## 4. Blink the LED

Create `blink.mjs`:

```javascript
import { ConduytDevice } from 'conduyt-js';
import { SerialTransport } from 'conduyt-js/transports/serial';

const device = await ConduytDevice.connect(
  new SerialTransport({ path: '/dev/cu.usbmodem101' }) // adjust for your port
);

console.log('Connected to:', device.capabilities.firmwareName);

await device.pin(13).mode('output');

for (let i = 0; i < 10; i++) {
  await device.pin(13).write(1);
  console.log('LED ON');
  await new Promise(r => setTimeout(r, 500));

  await device.pin(13).write(0);
  console.log('LED OFF');
  await new Promise(r => setTimeout(r, 500));
}

await device.disconnect();
```

Run it:

```bash
node blink.mjs
```

You should see the LED on pin 13 blink 10 times.

## Finding your serial port

- **macOS**: `/dev/cu.usbmodem*` or `/dev/cu.usbserial-*`
- **Linux**: `/dev/ttyACM0` or `/dev/ttyUSB0`
- **Windows**: `COM3`, `COM4`, etc. (check Device Manager)

Or list ports programmatically:

```javascript
import { SerialPort } from 'serialport';
const ports = await SerialPort.list();
ports.forEach(p => console.log(p.path, p.manufacturer));
```

## Next steps

- [Sensor Dashboard Tutorial](/docs/tutorials/sensor-dashboard) — read analog sensors and use datastreams
- [Connect over BLE](/docs/how-to/connect-ble) — wireless control from a browser
- [Playground](/playground) — write and run code directly in the browser
