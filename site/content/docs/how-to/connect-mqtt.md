---
title: Connect over MQTT
description: Configure MQTT transport for Conduyt devices and host SDKs
---

# Connect over MQTT

MQTT transport lets Conduyt devices communicate over a network through a broker. This guide covers firmware configuration, host SDK setup, and the topic structure.

## Prerequisites

You need a running MQTT broker. See [Set Up MQTT Broker](/docs/how-to/broker-setup) for a Docker Compose setup with Mosquitto.

## Firmware Configuration

```cpp
#define CONDUYT_TRANSPORT_MQTT
#include <Conduyt.h>

WiFiClient wifiClient;
ConduytMQTT transport(wifiClient, "broker.local", 1883, "device-001");
ConduytDevice device("MQTTSensor", "1.0.0", transport);

void setup() {
    // Connect to WiFi first
    WiFi.begin("ssid", "password");
    while (WiFi.status() != WL_CONNECTED) { delay(500); }

    transport.setAuth("conduyt-device", "password");
    device.begin();
}

void loop() {
    device.poll();
}
```

The fourth argument to `ConduytMQTT` is the device ID. It determines the MQTT topic prefix for this device.

## JavaScript Host

Save as `mqtt-read.mjs` and run with `node mqtt-read.mjs`:

```js
import { MQTTTransport } from 'conduyt-js/transports/mqtt'
import { ConduytDevice } from 'conduyt-js'

const transport = new MQTTTransport({
    broker: 'mqtt://broker.local:1883',
    deviceId: 'device-001',
    username: 'conduyt-host',
    password: 'password'
})

const device = new ConduytDevice(transport)
const hello = await device.connect()

console.log('Firmware:', hello.firmwareName)

const value = await device.pin(0).read()
console.log('Pin 0 value:', value)

await device.disconnect()
console.log('Disconnected')
```

## Python Host

Requires `pip install conduyt-py[mqtt]`.

```python
import asyncio
from conduyt import ConduytDevice
from conduyt.transports.mqtt import MQTTTransport


async def main():
    transport = MQTTTransport(
        broker='broker.local',
        port=1883,
        device_id='device-001',
        username='conduyt-host',
        password='password'
    )

    device = ConduytDevice(transport)
    hello = await device.connect()

    print(f"Firmware: {hello.firmware_name}")

    value = await device.pin(0).read()
    print(f"Pin 0 value: {value}")

    await device.disconnect()
    print("Disconnected")


asyncio.run(main())
```

## Topic Structure

All topics are prefixed with `conduyt/{deviceId}/`.

| Topic | Direction | Purpose |
|---|---|---|
| `cmd/{typeHex}` | Host to Device | Commands (e.g. `cmd/11` for PIN_WRITE) |
| `evt/{typeHex}` | Device to Host | Events (e.g. `evt/90` for PIN_EVENT) |
| `hello` | Device to Broker | HELLO_RESP binary, retained |
| `status` | Broker (LWT) | `"online"` or `"offline"`, retained |
| `ds/{name}/cmd` | Host to Device | Write to a named datastream |
| `ds/{name}/evt` | Device to Host | Datastream value pushes |

## QoS Strategy

| Packet Type | QoS | Rationale |
|---|---|---|
| PIN_WRITE, PIN_MODE | 1 | Must arrive; writes are idempotent |
| PIN_EVENT, STREAM_DATA | 0 | High frequency; stale data is worthless |
| OTA_CHUNK | 2 | Firmware chunks must arrive exactly once |
| DS_EVENT | 1 | Important but idempotent |
| HELLO, HELLO_RESP | 1 | Connection setup must complete |

## Last Will and Testament

The device registers an LWT when it connects to the broker:

- On clean connect, the device publishes `"online"` to `conduyt/{deviceId}/status` with `retain=true`.
- If the device disconnects unexpectedly, the broker publishes `"offline"` to the same topic.

Host SDKs subscribe to the `status` topic and fire disconnect events automatically.

## Framing

MQTT transports do not use COBS framing. MQTT is message-oriented, so each publish is one complete Conduyt packet. This differs from serial and BLE transports, which require COBS to delimit packets on a byte stream.
