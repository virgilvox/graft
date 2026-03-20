---
title: "Servo Module"
description: "Control hobby servos — attach, set angle, microseconds, and detach."
---

# Servo Module

Control standard hobby servos (SG90, MG996R, etc.) with angle or microsecond precision.

## Firmware setup

Enable the servo module with the `CONDUYT_MODULE_SERVO` flag and attach it to the device:

```cpp
#include <Conduyt.h>

#define CONDUYT_MODULE_SERVO

ConduytSerial transport(Serial, 115200);
ConduytDevice device("ServoBot", "1.0.0", transport);
ConduytServo servo;

void setup() {
  device.addModule(&servo);
  device.begin();
}

void loop() {
  device.poll();
}
```

## Host SDK usage

### JavaScript

```javascript
import { ConduytDevice } from 'conduyt-js';
import { Servo } from 'conduyt-js/modules/servo';

const device = await ConduytDevice.connect(transport);
const servo = new Servo(device, 0); // module index 0

await servo.attach(9, 544, 2400);   // pin 9, min/max microseconds
await servo.write(90);              // move to 90 degrees
await servo.writeMicroseconds(1500); // precise positioning
await servo.detach();
```

### Python

```python
from conduyt import ConduytDevice, SerialTransport
from conduyt.modules import Servo

device = ConduytDevice(SerialTransport("/dev/ttyUSB0"))
await device.connect()

servo = Servo(device, 0)
await servo.attach(9, 544, 2400)
await servo.write(90)
await servo.write_microseconds(1500)
await servo.detach()
```

### Go

```go
servo := modules.NewServo(device, 0)
servo.Attach(9, 544, 2400)
servo.Write(90)
servo.Detach()
```

## Commands

| Command | Byte | Payload | Description |
|---------|------|---------|-------------|
| Attach | `0x01` | `pin(1) + minUs(2) + maxUs(2)` | Attach servo to pin |
| Write | `0x02` | `angle(1)` | Set angle (0-180) |
| WriteMicroseconds | `0x03` | `us(2)` | Set pulse width in microseconds |
| Detach | `0x04` | (none) | Detach from pin |

## Wiring

Connect the servo signal wire to a PWM-capable pin (e.g., pin 9). Power the servo from 5V (not the Arduino's 5V pin for high-torque servos — use an external supply). Connect all grounds together.

## Notes

- Most servos accept 544-2400 microseconds. The `attach` command lets you set custom min/max.
- Sending angles outside 0-180 is clamped by the firmware.
- On ESP32, any GPIO pin can be used for servo (LEDC peripheral). On AVR, only PWM pins work.
