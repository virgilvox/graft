---
title: 'Quick Start: Python'
description: Control a GRAFT device from Python 3.10+
---

# Quick Start: Python

## Install

```bash
pip install graft-py

# With serial support:
pip install graft-py[serial]
```

## Connect and Blink

```python
import asyncio
from graft import GraftDevice
from graft.transports.serial import SerialTransport

async def main():
    transport = SerialTransport('/dev/ttyUSB0')
    device = GraftDevice(transport)

    hello = await device.connect()
    print(f"Connected: {hello.firmware_name}")

    pin = device.pin(13)
    await pin.mode('output')
    await pin.write(1)

    await asyncio.sleep(1)
    await pin.write(0)

    await device.disconnect()

asyncio.run(main())
```

## Synchronous API

```python
from graft import GraftDeviceSync
from graft.transports.mock import MockTransport

device = GraftDeviceSync(MockTransport())
device.connect()
device.pin(13).mode('output')
device.pin(13).write(1)
device.close()
```

## Module Wrappers

```python
from graft.modules import GraftServo

servo = GraftServo(device, module_id=0)
await servo.attach(pin=9)
await servo.write(90)
```

Available: `GraftServo`, `GraftNeoPixel`, `GraftEncoder`, `GraftStepper`, `GraftDHT`, `GraftOLED`, `GraftPID`.

## Next Steps

- [Python SDK Guide](/docs/sdk-guides/python) — full API reference
- [MQTT Transport](/docs/iot/broker-setup) — remote device control
