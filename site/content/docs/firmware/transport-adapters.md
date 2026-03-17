---
title: Transport Adapters
description: Available transport layers for GRAFT firmware
---

# Transport Adapters

GRAFT is transport-agnostic. The same binary protocol works over any of these:

## Available Transports

| Transport | Class | COBS | Platform | Include Guard |
|---|---|---|---|---|
| Serial (UART/USB) | `GraftSerial` | Yes | All | — (default) |
| USB CDC | `GraftUSBSerial` | Yes | RP2040, SAMD, nRF52 | — |
| MQTT | `GraftMQTT` | No | ESP32, ESP8266 | `GRAFT_TRANSPORT_MQTT` |
| BLE (NUS) | `GraftBLE` | Yes | ESP32, nRF52 | `GRAFT_TRANSPORT_BLE` |
| TCP Server | `GraftTCP` | No | ESP32, ESP8266 | `GRAFT_TRANSPORT_TCP` |
| CLASP | `GraftCLASP` | Yes | ESP32 | `GRAFT_TRANSPORT_CLASP` |

## Serial (Default)

```cpp
GraftSerial transport(Serial, 115200);
GraftDevice device("MyBoard", "1.0.0", transport);
```

## MQTT

```cpp
#define GRAFT_TRANSPORT_MQTT
#include <Graft.h>

WiFiClient wifiClient;
GraftMQTT transport(wifiClient, "broker.local", 1883, "device-001");
GraftDevice device("MQTTBoard", "1.0.0", transport);
```

Topics follow the pattern `graft/{deviceId}/cmd` and `graft/{deviceId}/evt/{typeHex}`.

## BLE

```cpp
#define GRAFT_TRANSPORT_BLE
#include <Graft.h>

GraftBLE transport("MyGraftDevice");
GraftDevice device("BLEBoard", "1.0.0", transport);
```

Uses Nordic UART Service (NUS) UUIDs. COBS framing handles packet boundaries within BLE characteristic writes.

## COBS Framing

Serial and BLE transports use COBS (Consistent Overhead Byte Stuffing) to frame packets. COBS ensures 0x00 never appears in the encoded data, so 0x00 serves as a reliable packet delimiter.

TCP and MQTT transports don't need COBS — TCP has stream semantics with length-prefixed reads, and MQTT delivers discrete messages.

The `needsCOBS` flag on each transport tells `GraftDevice` whether to apply COBS encoding/decoding.
