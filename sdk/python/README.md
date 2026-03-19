# graft-py

GRAFT protocol SDK for Python. Async and sync APIs for host-side hardware control.

## Install

```bash
pip install graft-py

# With serial transport
pip install graft-py[serial]

# With MQTT transport
pip install graft-py[mqtt]

# All transports
pip install graft-py[all]
```

## Quick Start

### Async

```python
from graft import GraftDevice
from graft.transports.serial import SerialTransport

device = GraftDevice(SerialTransport("/dev/ttyUSB0"))
hello = await device.connect()

await device.pin(13).mode("output")
await device.pin(13).write(1)
value = await device.pin(0).read("analog")

await device.disconnect()
```

### Sync

```python
from graft import GraftDeviceSync
from graft.transports.serial import SerialTransport

device = GraftDeviceSync(SerialTransport("/dev/ttyUSB0"))
device.connect()

device.pin(13).mode("output")
device.pin(13).write(1)
value = device.pin(0).read("analog")

device.disconnect()
```

### Module Usage

```python
from graft.modules.servo import GraftServo

servo = GraftServo(device)
await servo.attach(9)
await servo.write(90)
```

## API Reference

### GraftDevice (async)

| Method | Returns | Description |
|--------|---------|-------------|
| `connect()` | `HelloResp` | Connect and run HELLO handshake |
| `disconnect()` | `None` | Close connection |
| `ping()` | `None` | Ping/pong roundtrip |
| `reset()` | `None` | Reset device state |
| `pin(num)` | `PinProxy` | Pin control object |

### GraftDeviceSync

Synchronous wrapper around `GraftDevice`. Same methods, no `await`.

### PinProxy

| Method | Returns | Description |
|--------|---------|-------------|
| `mode(mode)` | `None` | Set pin mode: `"input"`, `"output"`, `"pwm"`, `"analog"`, `"input_pullup"` |
| `write(value)` | `None` | Digital or PWM write |
| `read(mode?)` | `int` | Read pin value |

### Errors

| Class | When |
|-------|------|
| `GraftNAKError` | Device rejected a command |
| `GraftTimeoutError` | No response within timeout |
| `GraftDisconnectedError` | Transport disconnected |

## Transports

| Transport | Module | Dependency |
|-----------|--------|------------|
| `SerialTransport` | `graft.transports.serial` | `pyserial-asyncio` |
| `MQTTTransport` | `graft.transports.mqtt` | `aiomqtt` |
| `MockTransport` | `graft.transports.mock` | None |

## Modules

| Module | Import | Hardware |
|--------|--------|----------|
| `GraftServo` | `graft.modules.servo` | Hobby servos |
| `GraftNeoPixel` | `graft.modules.neopixel` | WS2812/SK6812 LEDs |
| `GraftDHT` | `graft.modules.dht` | DHT11/DHT22 sensors |
| `GraftOLED` | `graft.modules.oled` | SSD1306 OLED displays |
| `GraftStepper` | `graft.modules.stepper` | Stepper motors |
| `GraftEncoder` | `graft.modules.encoder` | Rotary encoders |
| `GraftPID` | `graft.modules.pid` | PID controller |

## Requirements

- Python >= 3.10
- `pyserial-asyncio >= 0.6` (for serial transport)
- `aiomqtt >= 2.0` (for MQTT transport)

## License

MIT. Copyright (c) 2026 LumenCanvas.
