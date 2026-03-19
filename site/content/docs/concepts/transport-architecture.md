---
title: "Transport Architecture"
description: "How Conduyt separates protocol from transport"
---

# Transport Architecture

Conduyt separates the protocol (what packets mean) from the transport (how packets move). The protocol layer handles packet encoding, CRC computation, and sequence tracking. The transport layer handles delivery. This separation is the reason application code does not change when the physical connection changes.

## Two categories of transport

**Direct transports** connect host to device with no intermediary. Serial, USB CDC, BLE, and TCP are direct transports. The host opens a connection to a specific device and exchanges packets over that connection.

**Brokered transports** route through a message broker. MQTT is the primary brokered transport. The host publishes commands to a topic, the broker delivers them to the subscribed device, and events flow back through the broker.

The protocol layer is identical in both categories. A PIN_WRITE packet has the same binary structure whether it travels over a USB cable or through an MQTT broker.

## Framing depends on the transport

Direct transports that use byte streams need COBS framing to delineate packets. Serial, USB CDC, and BLE all deliver raw byte streams with no inherent message boundaries. COBS encoding eliminates 0x00 from the packet data, and 0x00 serves as the delimiter between packets.

Message-oriented transports deliver discrete messages. Each message contains exactly one packet. MQTT publishes are messages. WebSocket frames are messages. No COBS needed.

TCP uses a 2-byte length prefix (little-endian uint16) before each packet. The receiver reads the length, then reads that many bytes. No COBS.

The firmware's transport class exposes a `needsCOBS` flag. `ConduytDevice` checks this flag and applies or skips COBS framing accordingly.

## MQTT topic structure

MQTT topics follow this pattern:

```
conduyt/{deviceId}/cmd/{typeHex}   # commands to device
conduyt/{deviceId}/evt/{typeHex}   # events from device
```

The device ID in the topic path makes multi-device setups natural. A host managing several devices subscribes to each device's event topics independently. The hex-encoded packet type in the topic allows selective subscription: a dashboard that only cares about datastream events subscribes to `conduyt/+/evt/d1` without receiving pin state changes or other traffic.

## Why transport-agnostic matters

Firmware authors choose a transport at compile time. The choice is a single line: instantiate `ConduytSerial`, `ConduytMQTT`, `ConduytTCP`, or another transport class. Pass it to `ConduytDevice`. All application code (pin control, module registration, datastream declarations) remains unchanged.

Host SDK users choose a transport at runtime. Connect over serial for bench testing, switch to MQTT for deployment. The application code stays the same in both cases. Swapping from serial to MQTT requires changing one line in the firmware and one line in the host.

This is not an abstraction for its own sake. Hardware projects go through phases: USB-tethered development, wireless testing, production deployment. The transport layer changes at each phase. The application logic should not.

## The ConduytTransport interface

On the firmware side, any transport implements five functions:

```cpp
void begin();
int available();
int read(uint8_t* buf, size_t len);
int write(const uint8_t* buf, size_t len);
bool needsCOBS();
```

`begin()` initializes the transport. `available()` returns how many bytes are ready to read. `read()` and `write()` move data. `needsCOBS()` tells `ConduytDevice` whether to apply COBS framing.

Any new transport that implements this interface works with the existing `ConduytDevice` class. No changes to the protocol layer, no changes to registered modules, no changes to datastream handling. The transport is a plug-in point with a deliberately small surface area.
