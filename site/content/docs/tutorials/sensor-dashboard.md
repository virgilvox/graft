---
title: "Sensor Dashboard: Temperature Readings from Python"
description: "Wire a DHT22 sensor, flash Conduyt firmware with the DHT module, and read sensor data from a Python script."
---

# Sensor Dashboard: Temperature Readings from Python

This tutorial wires a DHT22 sensor to a microcontroller, flashes Conduyt firmware with the DHT module, and reads temperature and humidity from a Python script over serial. You will learn how Conduyt modules bridge hardware-specific firmware code to high-level host SDK calls.

## What you need

- Arduino Uno or ESP32 dev board
- DHT22 sensor (four-pin version or three-pin breakout)
- 10k ohm pull-up resistor (not needed if using a breakout with built-in resistor)
- Breadboard and jumper wires
- USB cable for your board
- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/index.html) installed
- Python 3.10 or later

## 1. Wire the DHT22

Connect the DHT22 to the board:

| DHT22 Pin | Board Pin |
|-----------|-----------|
| VCC       | 5V (or 3.3V on ESP32) |
| DATA      | D4        |
| NC        | not connected |
| GND       | GND       |

If you are using the bare four-pin sensor, connect the 10k resistor between VCC and DATA.

## How modules work

A module is a C++ plugin that handles specific hardware on the firmware side. When firmware registers a module with `addModule`, that module appears in the device's capability list. On connect, the host SDK discovers registered modules through the HELLO response and provides a wrapper class to send commands and receive responses.

For the DHT22, the module handles sensor initialization and reading on the device. The host side sends `begin` and `read` commands through the wrapper. The firmware module translates those commands into hardware calls and returns results. No sensor-specific code runs on the host.

## 2. Flash the firmware

Create a PlatformIO project. This example targets the Arduino Uno; change the `board` line for ESP32.

```ini
; platformio.ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = conduyt
build_flags = -DCONDUYT_MODULE_DHT
```

The `-DCONDUYT_MODULE_DHT` flag tells the Conduyt library to compile the DHT module code into the firmware binary.

Write the firmware sketch:

```cpp
// src/main.cpp
#include <Arduino.h>
#include <Conduyt.h>

ConduytSerial transport(Serial, 115200);
ConduytDevice device("WeatherStation", "1.0.0", transport);

void setup() {
  Serial.begin(115200);

  device.addModule(new ConduytModuleDHT());
  device.begin();
}

void loop() {
  device.poll();
}
```

`addModule` registers the DHT module so it appears in the HELLO response and can receive commands from the host. The `poll()` loop processes all incoming packets, including module commands. No other firmware code is needed -- the module handles DHT-specific logic internally.

Upload:

```bash
pio run --target upload
```

Expected output (last few lines):

```
Writing | ################################################## | 100%
avrdude done.  Thank you.
======== [SUCCESS] Took 5.41 seconds ========
```

## 3. Install the Python SDK

Create a virtual environment and install the SDK with serial support:

```bash
python -m venv .venv
source .venv/bin/activate
pip install conduyt-py[serial]
```

Expected output (trimmed):

```
Successfully installed conduyt-py-0.1.0 pyserial-3.5
```

## 4. Connect and check capabilities

Create `dashboard.py`:

```python
# dashboard.py
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.serial import SerialTransport

async def main():
    device = ConduytDevice(SerialTransport('/dev/ttyUSB0'))
    hello = await device.connect()

    print(f"Firmware: {hello.firmware_name}")
    print(f"Pins: {len(hello.pins)}")
    print(f"Modules: {[m.name for m in hello.modules]}")

    await device.disconnect()

asyncio.run(main())
```

Replace `/dev/ttyUSB0` with your board's serial port.

Run it:

```bash
python dashboard.py
```

Expected output:

```
Firmware: WeatherStation
Pins: 20
Modules: ['dht']
```

The `modules` list contains `dht` because the firmware registered a `ConduytModuleDHT` instance. The HELLO response includes all registered modules so the host knows what the device supports before sending commands.

## 5. Read the DHT sensor via the module wrapper

The Python SDK includes `ConduytDHT`, a wrapper that sends module commands to the DHT firmware module. Replace the contents of `dashboard.py`:

```python
# dashboard.py
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.serial import SerialTransport
from conduyt.modules import ConduytDHT

async def main():
    device = ConduytDevice(SerialTransport('/dev/ttyUSB0'))
    await device.connect()

    dht = ConduytDHT(device, module_id=0)
    await dht.begin(pin=4, sensor_type=22)

    reading = await dht.read()
    print(f"Temperature: {reading.temperature} C")
    print(f"Humidity: {reading.humidity} %")

    await device.disconnect()

asyncio.run(main())
```

`module_id=0` is the index of the first registered module. If you registered multiple modules, the second would be `module_id=1`.

`begin(pin=4, sensor_type=22)` sends a MOD_CMD packet to the device telling the DHT module which GPIO pin the sensor is on and which sensor type to use (22 for DHT22, 11 for DHT11). The module initializes the sensor hardware on the device side.

`read()` sends another MOD_CMD to the device. The DHT module reads the sensor and responds with temperature and humidity values.

Run it:

```bash
python dashboard.py
```

Expected output (values depend on ambient conditions):

```
Temperature: 23.4 C
Humidity: 51.2 %
```

## 6. Print continuous readings in a loop

Replace the contents of `dashboard.py` to read and display data every two seconds:

```python
# dashboard.py
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.serial import SerialTransport
from conduyt.modules import ConduytDHT

async def main():
    device = ConduytDevice(SerialTransport('/dev/ttyUSB0'))
    await device.connect()

    dht = ConduytDHT(device, module_id=0)
    await dht.begin(pin=4, sensor_type=22)

    print("Reading sensor data. Press Ctrl+C to stop.\n")
    try:
        while True:
            reading = await dht.read()
            print(f"Temp: {reading.temperature:.1f} C  |  Humidity: {reading.humidity:.1f} %")
            await asyncio.sleep(2)
    except KeyboardInterrupt:
        pass

    await device.disconnect()
    print("\nDisconnected.")

asyncio.run(main())
```

Run it:

```bash
python dashboard.py
```

Expected output (runs until you press Ctrl+C):

```
Reading sensor data. Press Ctrl+C to stop.

Temp: 23.4 C  |  Humidity: 51.2 %
Temp: 23.5 C  |  Humidity: 51.0 %
Temp: 23.5 C  |  Humidity: 50.8 %
^C
Disconnected.
```

Each iteration sends a MOD_CMD to the board and waits for the response. The two-second interval matches the DHT22's minimum sampling period.

## What's next

This tutorial used a module wrapper, where the host sends a command and the device responds with data. Conduyt also supports datastreams for continuous data push from device to host without polling. See [Use Datastreams](/docs/how-to/use-datastreams) for that approach.

For the full wire format specification, see [Packet Structure](/docs/reference/packet-structure).
