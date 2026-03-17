---
title: Python
description: Python SDK API guide for graft-py
---

# Python SDK Guide

## Installation

```bash
pip install graft-py

# With transport extras:
pip install graft-py[serial]   # pyserial-asyncio
pip install graft-py[mqtt]     # aiomqtt
pip install graft-py[all]      # everything
```

Requires Python 3.10+.

## Async API

```python
import asyncio
from graft import GraftDevice
from graft.transports.serial import SerialTransport

async def main():
    transport = SerialTransport('/dev/ttyUSB0')
    device = GraftDevice(transport, timeout_ms=5000)

    hello = await device.connect()
    print(f"Firmware: {hello.firmware_name}")
    print(f"Version: {hello.firmware_version}")
    print(f"Pins: {len(hello.pins)}")
    print(f"Modules: {[m.name for m in hello.modules]}")

    await device.ping()
    await device.disconnect()

asyncio.run(main())
```

## Synchronous Wrapper

```python
from graft import GraftDeviceSync
from graft.transports.mock import MockTransport

device = GraftDeviceSync(MockTransport())
device.connect()
device.ping()
device.close()
```

## Pin Control

```python
pin = device.pin(13)
await pin.mode('output')
await pin.write(255)        # PWM
value = await pin.read()    # 16-bit
```

Pin modes: `input`, `output`, `pwm`, `analog`, `input_pullup`.

## Modules

```python
from graft.modules import GraftServo, GraftNeoPixel, GraftDHT

# Servo
servo = GraftServo(device, module_id=0)
await servo.attach(9)
await servo.write(90)
await servo.detach()

# NeoPixel
neo = GraftNeoPixel(device, module_id=1)
await neo.begin(pin=6, count=30)
await neo.fill(255, 0, 0)
await neo.show()

# DHT
dht = GraftDHT(device, module_id=2)
await dht.begin(pin=4, sensor_type=22)
reading = await dht.read()
print(f"Temp: {reading.temperature}°C, Humidity: {reading.humidity}%")
```

## Error Handling

```python
from graft import GraftNAKError, GraftTimeoutError, GraftDisconnectedError

try:
    await device.pin(99).write(1)
except GraftNAKError as e:
    print(f"Device error: {e.error_name} (0x{e.code:02x})")
except GraftTimeoutError:
    print("Command timed out")
except GraftDisconnectedError:
    print("Device disconnected")
```

## Transports

| Transport | Import | Requirements |
|---|---|---|
| Mock | `graft.transports.mock` | — |
| Serial | `graft.transports.serial` | `pip install graft-py[serial]` |
| MQTT | `graft.transports.mqtt` | `pip install graft-py[mqtt]` |
