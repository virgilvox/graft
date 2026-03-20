---
title: "DHT Module"
description: "Read temperature and humidity from DHT11/DHT22 sensors."
---

# DHT Module

Read temperature (Celsius) and humidity (%) from DHT11 and DHT22 sensors.

## Firmware setup

```cpp
#include <Conduyt.h>

#define CONDUYT_MODULE_DHT

ConduytSerial transport(Serial, 115200);
ConduytDevice device("WeatherStation", "1.0.0", transport);
ConduytDHT dht;

void setup() {
  device.addModule(&dht);
  device.begin();
}

void loop() {
  device.poll();
}
```

## Host SDK usage

### JavaScript

```javascript
import { DHT } from 'conduyt-js/modules/dht';

const dht = new DHT(device, 0);

await dht.begin(4, 22);  // pin 4, DHT22 (use 11 for DHT11)

const { temperature, humidity } = await dht.read();
console.log(`${temperature}°C, ${humidity}%`);
```

### Python

```python
from conduyt.modules import DHT

dht = DHT(device, 0)
await dht.begin(4, 22)

temp, hum = await dht.read()
print(f"{temp}°C, {hum}%")
```

## Commands

| Command | Byte | Payload | Description |
|---------|------|---------|-------------|
| Begin | `0x01` | `pin(1) + type(1)` | Initialize (type: 11 or 22) |
| Read | `0x02` | (none) | Read sensor, response contains temp + humidity |

## Response

The `read` command returns a MOD_RESP with payload:

```
temperature(4 bytes, float32 LE) + humidity(4 bytes, float32 LE)
```

## Wiring

Connect the DHT data pin to a digital GPIO (pin 4 is common). Add a 10K pull-up resistor between the data pin and 3.3V. Power from 3.3V (DHT22) or 5V (DHT11).

## Notes

- DHT sensors are slow — allow at least 2 seconds between reads (DHT22) or 1 second (DHT11).
- DHT22 is more accurate (0.1°C resolution) than DHT11 (1°C resolution).
- If reads return 0/0 or NaN, check your wiring and pull-up resistor.
