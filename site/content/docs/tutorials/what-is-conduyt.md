---
title: What is Conduyt?
description: An introduction to the Conduyt protocol, its architecture, and core concepts
---

# What is Conduyt?

Conduyt is a binary protocol for controlling microcontrollers from a host computer. You flash a small firmware library onto an Arduino, ESP32, or similar board, then write code in JavaScript, Python, Go, Rust, or Swift to control it. The protocol handles communication over USB serial, Bluetooth, MQTT, or any other transport.

## The architecture

A Conduyt system has two sides:

```
┌──────────────┐                    ┌──────────────┐
│  Host (PC)   │  binary packets    │   Device     │
│              │ ◄────────────────► │  (Arduino)   │
│  JS / Python │   over serial,     │              │
│  Go / Rust   │   BLE, MQTT, etc.  │  C++ firmware │
└──────────────┘                    └──────────────┘
```

**Device side**: An Arduino sketch includes the Conduyt library, creates a transport (serial, BLE, MQTT), and calls `device.begin()` and `device.poll()`. That is the entire firmware. The library handles all protocol logic, pin control, and module management.

**Host side**: A program on your computer (or server, or phone) imports an SDK, connects to the device, and sends commands. Toggle a pin, read a sensor, control a servo. The SDK handles packet encoding, CRC validation, and capability discovery.

**Between them**: A compact binary protocol. Fixed 8-byte headers, CRC8 checksums, COBS framing on serial links. A complete "turn on LED" command is 10 bytes.

## Core concepts

### Transports

A transport is the physical channel between host and device. Serial USB, Bluetooth Low Energy, MQTT over WiFi, WebSocket, TCP. The protocol is identical across all of them. Swap the transport, keep all your application code.

On the firmware side, pick a transport class:

```cpp
ConduytSerial transport(Serial, 115200);   // USB serial
ConduytBLE transport("MyDevice");          // Bluetooth
ConduytMQTT transport(wifi, "broker", 1883, "device-001");  // MQTT
```

On the host side, pick a matching transport:

```typescript
new SerialTransport({ path: '/dev/ttyUSB0' })  // USB serial
new BLETransport()                               // Bluetooth
new MQTTTransport({ broker: 'mqtt://...' })     // MQTT
```

### Capabilities

When the host connects, the device describes itself. It sends a HELLO_RESP packet containing: how many pins it has, what each pin supports (digital, analog, PWM, I2C), what modules are loaded, what datastreams are declared, maximum payload size, firmware version, and more.

The host SDK parses this into a capabilities object. From that point on, the SDK validates every operation before sending it. Request PWM on a pin that only supports digital? The SDK throws an error immediately, without wasting a round trip to the device.

### Modules

Modules are opt-in firmware plugins for specific hardware. Servo motors, NeoPixel LED strips, DHT temperature sensors, OLED displays, stepper motors, PID controllers. Enable a module with a compile flag, register it in your sketch, and the host SDK discovers it automatically.

```cpp
// Firmware: enable and register a servo module
#define CONDUYT_MODULE_SERVO
#include <Conduyt.h>

device.addModule(new ConduytModuleServo());
```

```typescript
// Host: use the discovered module
const servo = new ConduytServo(device)
await servo.attach(9)
await servo.write(90)
```

You can also write your own modules for custom hardware.

### Datastreams

Datastreams are named, typed data channels. A temperature sensor publishes a `float32` value called "temperature" with unit "celsius". A thermostat exposes a writable "setpoint" channel. The host reads, writes, and subscribes to these channels by name.

```cpp
// Firmware: declare a read-only temperature datastream
device.addDatastream("temperature", CONDUYT_TYPE_FLOAT32, "celsius", false);
device.writeDatastream("temperature", 22.5f);
```

```typescript
// Host: subscribe to temperature updates
for await (const value of device.datastream('temperature').subscribe()) {
    console.log('Temperature:', value)
}
```

Datastreams are a higher-level abstraction than raw pin control. Use pins for GPIO (on/off, PWM, analog reads). Use datastreams for application-level data (sensor readings, configuration values, status indicators).

## What Conduyt replaces

| Protocol | Year | Limitation |
|---|---|---|
| Firmata | 2006 | MIDI-based encoding, serial only, no real capability negotiation |
| Johnny-Five | 2012 | Node.js only, inherits all Firmata limits |
| Blynk 2.0 | 2021 | Requires proprietary cloud service |

Conduyt is binary (not MIDI), transport-agnostic (not serial-only), self-hostable (no cloud), and has SDKs for five languages.

## How these docs are organized

These docs follow the [Diataxis](https://diataxis.fr/) framework:

- **Tutorials** walk you through complete projects step by step. Start here.
- **How-To Guides** are targeted recipes for specific tasks: connecting over a particular transport, writing a custom module, configuring a broker.
- **Reference** is the complete technical specification: every packet type, every API method, every constant.
- **Concepts** explain the design decisions: why binary, how transports work, what the capability model does.

## Next steps

- [First Blink](/docs/tutorials/first-blink): Flash firmware, connect from JavaScript, toggle an LED, read a sensor. Takes 10 minutes.
- [Sensor Dashboard](/docs/tutorials/sensor-dashboard): Wire a DHT22, use the module system, read data from Python.
