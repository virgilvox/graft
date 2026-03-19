---
title: Packet Types
description: Complete table of CONDUYT command and event packet types
---

# Packet Types

Every CONDUYT packet carries a 1-byte type code in the header. Commands flow host to device. Events flow device to host.

Payload fields use the notation `u8` (unsigned byte), `u16` (unsigned 16-bit little-endian), `i32` (signed 32-bit little-endian), `f32` (32-bit IEEE 754 float little-endian), `N` (variable length remaining bytes). Fields in `[brackets]` are optional.

## Commands (Host to Device)

| Code | Name | Payload | Response |
|---|---|---|---|
| `0x01` | PING | (none) | PONG |
| `0x02` | HELLO | (none) | HELLO_RESP |
| `0x10` | PIN_MODE | pin(u8) + mode(u8) | ACK |
| `0x11` | PIN_WRITE | pin(u8) + value(u8) [+ mode(u8)] | ACK |
| `0x12` | PIN_READ | pin(u8) [+ mode(u8)] | PIN_READ_RESP |
| `0x13` | PIN_SUBSCRIBE | pin(u8) + mode(u8) + interval_ms(u16) [+ threshold(u16)] | ACK |
| `0x14` | PIN_UNSUBSCRIBE | pin(u8) | ACK |
| `0x20` | I2C_WRITE | addr(u8) + data(N) | ACK |
| `0x21` | I2C_READ | addr(u8) + count(u8) | I2C_READ_RESP |
| `0x22` | I2C_READ_REG | addr(u8) + reg(u8) + count(u8) | I2C_READ_RESP |
| `0x30` | SPI_XFER | cs_pin(u8) + data(N) | SPI_XFER_RESP |
| `0x40` | MOD_CMD | module_id(u8) + cmd(u8) + payload(N) | ACK or MOD_RESP |
| `0x50` | STREAM_START | pin_mask(u16) + rate_hz(u16) | ACK |
| `0x51` | STREAM_STOP | (none) | ACK |
| `0x60` | DS_WRITE | ds_index(u8) + value(N) | ACK |
| `0x61` | DS_READ | ds_index(u8) | DS_READ_RESP |
| `0x62` | DS_SUBSCRIBE | ds_index(u8) + interval_ms(u16) | ACK |
| `0x70` | OTA_BEGIN | total_bytes(u32) + sha256(32) | ACK |
| `0x71` | OTA_CHUNK | offset(u32) + data(N) | ACK |
| `0x72` | OTA_FINALIZE | (none) | ACK (then reboot) |
| `0xF0` | RESET | (none) | ACK |

## Events (Device to Host)

| Code | Name | Payload | Trigger |
|---|---|---|---|
| `0x80` | PONG | (none) | Response to PING |
| `0x81` | HELLO_RESP | capability binary | Response to HELLO |
| `0x82` | ACK | (none) | Command success |
| `0x83` | NAK | error_code(u8) | Command failure |
| `0x90` | PIN_EVENT | pin(u8) + value(u16) | Subscription trigger |
| `0x91` | PIN_READ_RESP | pin(u8) + value(u16) | Response to PIN_READ |
| `0xA0` | I2C_READ_RESP | data(N) | Response to I2C_READ |
| `0xB0` | SPI_XFER_RESP | data(N) | Response to SPI_XFER |
| `0xC0` | MOD_EVENT | module_id(u8) + event_code(u8) + data(N) | Unsolicited module event |
| `0xC1` | MOD_RESP | module_id(u8) + data(N) | Response to MOD_CMD |
| `0xD0` | STREAM_DATA | values(N x u16) | Streaming sample |
| `0xD1` | DS_EVENT | ds_index(u8) + value(N) | Datastream push |
| `0xD2` | DS_READ_RESP | ds_index(u8) + value(N) | Response to DS_READ |
| `0xE0` | LOG | string(N) | Debug log message |
| `0xFF` | FATAL | string(N) | Unrecoverable error |

## Common Payload Examples

Actual payload bytes for frequently used packets:

**PIN_MODE** -- set pin 13 to output mode:
```
0D 01
```
`0D` = pin 13, `01` = DIGITAL_OUT mode.

**PIN_WRITE** -- set pin 13 high:
```
0D 01
```
`0D` = pin 13, `01` = value HIGH.

**PIN_READ** -- read pin 0:
```
00
```
`00` = pin 0. The device responds with PIN_READ_RESP containing the current value.

**MOD_CMD** -- send command 0x02 to module 0:
```
00 02
```
`00` = module_id 0, `02` = command byte. Additional payload bytes follow if the command requires data.

## Sequence Numbers

Every command carries a `seq` byte (0 to 255, wrapping). The device echoes `seq` in its response (ACK, NAK, or typed response). This correlates multiple in-flight commands to their responses.

Unsolicited events (PIN_EVENT, MOD_EVENT, DS_EVENT, STREAM_DATA, LOG, FATAL) use `seq = 0`.
