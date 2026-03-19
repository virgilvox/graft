---
title: Error Codes
description: Complete NAK error code reference
---

# Error Codes

When a command fails, the device responds with a NAK packet (`0x83`) containing a 1-byte error code. The NAK echoes the `seq` of the failed command.

## Error Table

| Code | Name | Constant | Description |
|---|---|---|---|
| `0x01` | UNKNOWN_TYPE | `CONDUYT_ERR_UNKNOWN_TYPE` | Unrecognized command type byte |
| `0x02` | CRC_MISMATCH | `CONDUYT_ERR_CRC_MISMATCH` | Packet CRC8 validation failed |
| `0x03` | PAYLOAD_TOO_LARGE | `CONDUYT_ERR_PAYLOAD_TOO_LARGE` | Payload exceeds device buffer |
| `0x04` | INVALID_PIN | `CONDUYT_ERR_INVALID_PIN` | Pin number out of range |
| `0x05` | PIN_MODE_UNSUPPORTED | `CONDUYT_ERR_PIN_MODE_UNSUPPORTED` | Pin does not support requested mode |
| `0x06` | I2C_NOT_AVAILABLE | `CONDUYT_ERR_I2C_NOT_AVAILABLE` | I2C bus not initialized or unavailable |
| `0x07` | I2C_NACK | `CONDUYT_ERR_I2C_NACK` | I2C device did not acknowledge |
| `0x08` | MODULE_NOT_LOADED | `CONDUYT_ERR_MODULE_NOT_LOADED` | Module ID not registered |
| `0x09` | UNKNOWN_MODULE_CMD | `CONDUYT_ERR_UNKNOWN_MODULE_CMD` | Module does not handle this command byte |
| `0x0A` | MODULE_BUSY | `CONDUYT_ERR_MODULE_BUSY` | Module is mid-operation (e.g., stepper moving) |
| `0x0B` | SUB_LIMIT_REACHED | `CONDUYT_ERR_SUB_LIMIT_REACHED` | Max subscriptions reached |
| `0x0C` | OUT_OF_MEMORY | `CONDUYT_ERR_OUT_OF_MEMORY` | Allocation failed |
| `0x0D` | UNKNOWN_DATASTREAM | `CONDUYT_ERR_UNKNOWN_DATASTREAM` | Datastream index not found |
| `0x0E` | DATASTREAM_READONLY | `CONDUYT_ERR_DATASTREAM_READONLY` | Write attempted on read-only datastream |
| `0x0F` | OTA_INVALID | `CONDUYT_ERR_OTA_INVALID` | OTA not supported or sequence error |
| `0x10` | VERSION_MISMATCH | `CONDUYT_ERR_VERSION_MISMATCH` | Protocol version incompatible |

## Common Scenarios

### INVALID_PIN (0x04)

You passed a pin number higher than the device supports. Check `device.capabilities.pins.length` before addressing a pin.

```typescript
const pinCount = device.capabilities.pins.length
if (pinIndex >= pinCount) {
  throw new Error(`Pin ${pinIndex} out of range, device has ${pinCount} pins`)
}
await device.pin(pinIndex).write(1)
```

### PIN_MODE_UNSUPPORTED (0x05)

The pin does not support the requested mode. Check the pin's capability bitmask in `device.capabilities.pins[n].capabilities`.

```typescript
const pin = device.capabilities.pins[9]
const supportsPWM = pin.capabilities & 0x04
if (!supportsPWM) {
  throw new Error(`Pin 9 does not support PWM`)
}
await device.pin(9).mode('pwm')
```

### MODULE_NOT_LOADED (0x08)

You sent a MOD_CMD for a module ID that the firmware did not register. Verify the module is registered with `device.addModule()` and that the module_id matches the registration order.

```cpp
// Firmware side: modules are assigned IDs in registration order
device.addModule(servoModule);   // module_id = 0
device.addModule(stepperModule); // module_id = 1
```

If the host sends a command to module_id 2 and only two modules are registered, the device returns MODULE_NOT_LOADED.

### PAYLOAD_TOO_LARGE (0x03)

Your payload exceeds the device's buffer. Check `device.capabilities.maxPayload` for the limit. On ATmega328 boards this is 128 bytes.

```typescript
const max = device.capabilities.maxPayload
if (data.length > max) {
  throw new Error(`Payload ${data.length} bytes exceeds limit of ${max}`)
}
```

## Host Handling

### JavaScript

```typescript
import { ConduytNAKError } from 'conduyt-js'

try {
  await device.pin(99).write(1)
} catch (e) {
  if (e instanceof ConduytNAKError) {
    console.log(e.errorName) // "INVALID_PIN"
    console.log(e.code)      // 0x04
  }
}
```

The `errorName` property maps the numeric code to its string name. The `code` property holds the raw byte value.

### Python

```python
from conduyt import ConduytNAKError

try:
    await device.pin(99).write(1)
except ConduytNAKError as e:
    print(e.error_name)  # "INVALID_PIN"
    print(e.code)        # 0x04
```

Python uses `error_name` (snake_case), not `errorName`.

## Retry Strategy

NAK errors are deterministic. Retrying the same command produces the same error. Fix the root cause (wrong pin number, unsupported mode, missing module) before retrying. Timeout errors, by contrast, may be transient and worth retrying once.
