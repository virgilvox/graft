---
title: "First Blink: Arduino LED Control from JavaScript"
description: "Flash Conduyt firmware on an Arduino, connect from a Node.js script, toggle an LED, and read an analog pin."
---

# First Blink: Arduino LED Control from JavaScript

This tutorial walks through flashing Conduyt firmware onto an Arduino, connecting to it from Node.js over serial, toggling the onboard LED, and reading an analog sensor. By the end, you will understand how the Conduyt protocol negotiates a connection, sets pin modes, and transfers pin state between a microcontroller and a host program.

## What you will learn

1. How to set up a PlatformIO project and flash Conduyt firmware
2. How the firmware's transport, device, and poll loop work together
3. How to connect from JavaScript using `ConduytDevice.connect()`
4. How to control digital output pins and read analog input channels
5. What protocol messages (HELLO, PIN_MODE, PIN_WRITE) are exchanged behind the scenes

## What you need

- Arduino Uno (or compatible board with a built-in LED on pin 13)
- USB cable (Type-A to Type-B for Uno)
- Potentiometer (any value; 10k ohm works well)
- Three jumper wires
- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/index.html) installed
- Node.js 18 or later

## 1. Create a PlatformIO project

PlatformIO is a build system for embedded development. It handles toolchains, board definitions, and library management so you do not need to install the Arduino IDE or manage compiler versions yourself. Install it with `pip install platformio` or follow the [official installation guide](https://docs.platformio.org/en/latest/core/installation/index.html).

Create and enter a new project directory, then initialize it for Arduino Uno:

```bash
mkdir conduyt-firmware && cd conduyt-firmware
pio project init --board uno --project-option "framework=arduino"
```

Expected output:

```
Project has been initialized!
```

This creates a `platformio.ini` config file, a `src/` directory for your code, and a `lib/` directory for local libraries.

Open `platformio.ini` and add the Conduyt library as a dependency:

```ini
; platformio.ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = conduyt
```

PlatformIO will download the Conduyt library automatically on the next build.

## 2. Flash the firmware

Create the file `src/main.cpp` with the following code:

```cpp
// src/main.cpp
#include <Arduino.h>
#include <Conduyt.h>

ConduytSerial transport(Serial, 115200);
ConduytDevice device("MyBoard", "1.0.0", transport);

void setup() {
  Serial.begin(115200);
  device.begin();
}

void loop() {
  device.poll();
}
```

Here is what each part does:

- `#include <Conduyt.h>` pulls in the Conduyt firmware library, which implements the protocol's device side.
- `ConduytSerial transport(Serial, 115200)` creates a serial transport layer. This object handles framing and encoding Conduyt protocol messages over the hardware UART at 115200 baud.
- `ConduytDevice device("MyBoard", "1.0.0", transport)` creates the device instance. The first argument is the firmware name reported during the handshake. The second is a version string. The third binds the device to the serial transport.
- `device.begin()` in `setup()` initializes internal state: it registers the board's pin map and prepares to respond to host connections.
- `device.poll()` in `loop()` checks for incoming protocol messages and processes them. Every iteration of the Arduino loop calls `poll()` so the device can respond to commands with minimal latency.

Upload the firmware to the board:

```bash
pio run --target upload
```

Expected output (last few lines):

```
Writing | ################################################## | 100%
avrdude done.  Thank you.
======== [SUCCESS] Took 5.23 seconds ========
```

The board is now running Conduyt firmware and waiting for a host connection on serial.

## 3. Install the host SDK

Open a separate terminal and create a working directory for the Node.js scripts:

```bash
mkdir conduyt-blink && cd conduyt-blink
npm init -y
npm install conduyt-js
```

Expected output (trimmed):

```
added 3 packages in 1.2s
```

The `conduyt-js` package contains both the core SDK and transport modules.

## 4. Connect and inspect capabilities

Create `blink.mjs`:

```js
// blink.mjs
import { ConduytDevice } from 'conduyt-js'
import { SerialTransport } from 'conduyt-js/transports/serial'

const transport = new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })

let device
try {
  device = await ConduytDevice.connect(transport)
} catch (err) {
  console.error('Connection failed:', err.message)
  process.exit(1)
}

console.log('Firmware:', device.capabilities.firmwareName)
console.log('Pins:', device.capabilities.pins.length)
console.log('Modules:', device.capabilities.modules)

await device.disconnect()
```

Replace `/dev/ttyUSB0` with your board's serial port. On macOS this is often `/dev/cu.usbmodemXXXX`. On Windows it is `COM3` or similar. You can list available ports with `pio device list`.

`ConduytDevice.connect()` is a static factory method. It opens the transport, performs the protocol handshake, and returns a device instance with its `capabilities` object already populated. If the board does not respond or the handshake fails, the promise rejects.

Run the script:

```bash
node blink.mjs
```

Expected output:

```
Firmware: MyBoard
Pins: 20
Modules: []
```

The `firmwareName` matches the string you passed to the `ConduytDevice` constructor in the firmware. The `pins` array lists every pin the board exposes. The `modules` array is empty because this sketch has no custom modules registered.

## 5. Toggle the LED

Replace the contents of `blink.mjs`:

```js
// blink.mjs
import { ConduytDevice } from 'conduyt-js'
import { SerialTransport } from 'conduyt-js/transports/serial'

const transport = new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })

let device
try {
  device = await ConduytDevice.connect(transport)
} catch (err) {
  console.error('Connection failed:', err.message)
  process.exit(1)
}

try {
  await device.pin(13).mode('output')

  console.log('LED on')
  await device.pin(13).write(1)

  await new Promise(r => setTimeout(r, 1000))

  console.log('LED off')
  await device.pin(13).write(0)
} finally {
  await device.disconnect()
}
```

Run it:

```bash
node blink.mjs
```

Expected output:

```
LED on
LED off
```

The onboard LED turns on for one second, then turns off. `pin(13).mode('output')` configures pin 13 as a digital output. `write(1)` sets the pin high (LED on) and `write(0)` sets it low (LED off). The `try/finally` block ensures the serial connection is closed even if a pin command fails.

## 6. Read an analog pin

Wire the potentiometer to the Arduino:

- Left leg to **GND**
- Center leg (wiper) to **A0**
- Right leg to **5V**

Replace the contents of `blink.mjs`:

```js
// blink.mjs
import { ConduytDevice } from 'conduyt-js'
import { SerialTransport } from 'conduyt-js/transports/serial'

const transport = new SerialTransport({ path: '/dev/ttyUSB0', baudRate: 115200 })

let device
try {
  device = await ConduytDevice.connect(transport)
} catch (err) {
  console.error('Connection failed:', err.message)
  process.exit(1)
}

try {
  await device.pin(0).mode('analog')

  for (let i = 0; i < 5; i++) {
    const value = await device.pin(0).read()
    console.log(`A0 = ${value}`)
    await new Promise(r => setTimeout(r, 500))
  }
} finally {
  await device.disconnect()
}
```

Run it while turning the potentiometer knob:

```bash
node blink.mjs
```

Expected output (values vary with knob position):

```
A0 = 512
A0 = 612
A0 = 780
A0 = 340
A0 = 100
```

**How analog pin addressing works in Conduyt:** When you call `pin(0).mode('analog')`, the firmware maps pin 0 to analog channel 0, which corresponds to the physical pin labeled A0 on the Arduino. The `mode('analog')` call is what triggers analog channel mapping. Calling `pin(0).read()` then performs an analog-to-digital conversion on channel A0 and returns the raw ADC value (0 to 1023 on a 10-bit Arduino). Turning the potentiometer changes the voltage divider output, which changes the reading.

## 7. Clean up

The `finally` block in each script calls `device.disconnect()`, which closes the serial connection and releases the port. If your script exits without disconnecting (for example, due to an unhandled exception outside the try/finally), the port may stay locked until you unplug the board or kill the process.

## What happened at the protocol level

Each step above triggered specific Conduyt protocol messages over the serial link:

1. **HELLO handshake** -- `ConduytDevice.connect()` sent a HELLO request to the board. The firmware replied with its name (`"MyBoard"`), version (`"1.0.0"`), pin count, and module list. This is how the `capabilities` object got populated.
2. **PIN_MODE command** -- `pin(13).mode('output')` sent a PIN_MODE message telling the firmware to configure pin 13 as a digital output. The same message type was used for `pin(0).mode('analog')`, with the mode field set to analog.
3. **PIN_WRITE command** -- `pin(13).write(1)` and `pin(13).write(0)` each sent a PIN_WRITE message with the target pin and value. The firmware set the hardware pin state accordingly.
4. **PIN_READ** -- `pin(0).read()` sent a read request for pin 0. The firmware performed the ADC conversion and returned the result in the response.

All of these messages are binary-encoded, framed by the transport layer, and handled automatically by the SDK and firmware library. You do not need to construct or parse them yourself.

## Next steps

- [Sensor Dashboard tutorial](/docs/tutorials/sensor-dashboard) -- Wire a DHT22 temperature sensor, use the module system and datastreams, and read live sensor data.
- [Connect over Serial](/docs/how-to/connect-serial) -- Configure serial communication across JavaScript, Python, and Go host SDKs.
