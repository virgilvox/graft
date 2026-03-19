---
title: Python API
description: conduyt-py SDK reference
---

# Python API

The `conduyt-py` package. Requires Python 3.10+.

```bash
pip install conduyt-py
```

## ConduytDevice

Async API. All I/O methods are coroutines.

### Constructor

```python
ConduytDevice(transport: ConduytTransport, timeout_ms: int = 5000)
```

### Methods

| Method | Return | Description |
|---|---|---|
| `async connect()` | `HelloResp` | Connect and perform HELLO handshake |
| `async disconnect()` | `None` | Close the transport |
| `async ping()` | `None` | Send PING, wait for PONG |
| `async reset()` | `None` | Send RESET command |
| `pin(num)` | `PinProxy` | Get a pin proxy |

### Properties

| Property | Type | Description |
|---|---|---|
| `connected` | `bool` | Connection state |
| `capabilities` | `HelloResp \| None` | Parsed HELLO_RESP, None before connect |

### Usage

```python
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.serial import SerialTransport

async def main():
    transport = SerialTransport('/dev/ttyUSB0')
    device = ConduytDevice(transport, timeout_ms=5000)

    hello = await device.connect()
    print(hello.firmware_name)
    print(len(hello.pins), "pins")

    await device.ping()
    await device.disconnect()

asyncio.run(main())
```

## ConduytDeviceSync

Synchronous wrapper around `ConduytDevice`. Same interface minus `async`/`await`. Uses `close()` instead of `disconnect()`.

```python
from conduyt import ConduytDeviceSync
from conduyt.transports.mock import MockTransport

device = ConduytDeviceSync(MockTransport())
hello = device.connect()
device.ping()
device.close()
```

| Method | Return | Description |
|---|---|---|
| `connect()` | `HelloResp` | Connect and perform HELLO handshake |
| `close()` | `None` | Close the transport |
| `ping()` | `None` | Send PING, wait for PONG |
| `reset()` | `None` | Send RESET command |
| `pin(num)` | `PinProxy` | Get a pin proxy |

## PinProxy

Returned by `device.pin(num)`.

| Method | Return | Description |
|---|---|---|
| `async mode(mode)` | `PinProxy` (self, chainable) | Set pin mode |
| `async write(value)` | `None` | Write digital or PWM value |
| `async read()` | `int` | Read pin value |

Pin modes: `"input"`, `"output"`, `"pwm"`, `"analog"`, `"input_pullup"`.

```python
pin = device.pin(13)
await pin.mode('output')
await pin.write(1)

value = await device.pin(0).read()
```

## Module Wrappers

Module wrappers use raw `_send_command()` internally to communicate with firmware modules. Import from `conduyt.modules`.

| Class | Firmware Module |
|---|---|
| `ConduytServo` | servo |
| `ConduytNeoPixel` | neopixel |
| `ConduytEncoder` | encoder |
| `ConduytStepper` | stepper |
| `ConduytDHT` | dht |
| `ConduytOLED` | oled1306 |
| `ConduytPID` | pid |

```python
from conduyt.modules import ConduytServo, ConduytNeoPixel, ConduytDHT

servo = ConduytServo(device, module_id=0)
await servo.attach(9)
await servo.write(90)
await servo.detach()

neo = ConduytNeoPixel(device, module_id=1)
await neo.begin(pin=6, count=30)
await neo.fill(255, 0, 0)
await neo.show()

dht = ConduytDHT(device, module_id=2)
await dht.begin(pin=4, sensor_type=22)
reading = await dht.read()
print(reading.temperature, reading.humidity)
```

## Error Classes

### ConduytNAKError

Raised when the device responds with NAK.

| Attribute | Type | Description |
|---|---|---|
| `error_name` | `str` | Error name (e.g., "INVALID_PIN"). Snake_case attribute name. |
| `code` | `int` | Raw error code byte |

### ConduytTimeoutError

Raised when a command receives no response within the timeout period.

### ConduytDisconnectedError

Raised when a command is attempted on a disconnected device.

```python
from conduyt import ConduytNAKError, ConduytTimeoutError, ConduytDisconnectedError

try:
    await device.pin(99).write(1)
except ConduytNAKError as e:
    print(e.error_name)  # "INVALID_PIN"
    print(e.code)        # 0x04
except ConduytTimeoutError:
    print("Command timed out")
except ConduytDisconnectedError:
    print("Device disconnected")
```

## Transports

| Transport | Import | Extras |
|---|---|---|
| Mock | `conduyt.transports.mock` | (none) |
| Serial | `conduyt.transports.serial` | `pip install conduyt-py[serial]` |
| MQTT | `conduyt.transports.mqtt` | `pip install conduyt-py[mqtt]` |

Install all extras: `pip install conduyt-py[all]`.

All transports implement the `ConduytTransport` protocol:

```python
class ConduytTransport(Protocol):
    async def open(self) -> None: ...
    async def close(self) -> None: ...
    async def send(self, packet: bytes) -> None: ...
    def on_packet(self, handler: Callable[[bytes], None]) -> None: ...
    @property
    def needs_cobs(self) -> bool: ...
```

## Types

All types are dataclasses.

```python
@dataclass
class HelloResp:
    firmware_name: str
    firmware_version: tuple[int, int, int]
    mcu_id: bytes
    ota_capable: bool
    pins: list[PinCapability]
    i2c_buses: int
    spi_buses: int
    uart_count: int
    max_payload: int
    modules: list[ModuleDescriptor]
    datastreams: list[DatastreamDescriptor]

@dataclass
class PinCapability:
    pin: int
    capabilities: int  # bitmask of PIN_CAP values

@dataclass
class ModuleDescriptor:
    module_id: int
    name: str
    version_major: int
    version_minor: int
    pins: list[int]

@dataclass
class DatastreamDescriptor:
    index: int
    name: str
    type: int
    unit: str
    writable: bool
    pin_ref: int
    retain: bool

@dataclass
class ConduytPacket:
    version: int
    type: int
    seq: int
    payload: bytes
```
