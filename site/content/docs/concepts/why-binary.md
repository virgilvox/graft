---
title: "Why Binary"
description: "Why Conduyt uses a binary wire format instead of text"
---

# Why Binary

Text protocols impose real cost on microcontrollers. An ATmega328 running at 16MHz with 2KB of RAM cannot afford string parsing. JSON requires scanning for delimiters, handling escape sequences, and maintaining state machines for nested structures. Firmata encodes everything through MIDI's 7-bit format, wasting a bit per byte and requiring split/merge logic for any value above 127. MQTT payloads are often text by convention, pushing the parsing burden onto devices that have the least capacity for it.

## The overhead is measurable

A PIN_WRITE command in Conduyt is 10 bytes total: an 8-byte header plus a 2-byte payload containing the pin number and value. The equivalent JSON representation:

```json
{"cmd":"pinWrite","pin":13,"value":1}
```

That is 40+ bytes before any framing. On a serial link at 115200 baud, 40 bytes takes roughly 3.5ms to transmit. 10 bytes takes under 0.9ms. When a device is streaming sensor data at high frequency, this difference compounds.

## Fixed-size header, predictable parsing

Conduyt's 8-byte header has a fixed layout. The parser always knows where each field is. There is no scanning for colons, commas, or closing braces. No allocation for variable-length keys. No recursion for nested objects. The firmware reads 8 bytes, indexes directly into known offsets, and has every field it needs: protocol version, packet type, sequence number, payload length, and CRC.

This predictability matters for real-time behavior. Parsing time is constant regardless of packet content.

## Why COBS

Serial streams have no inherent message boundaries. Bytes arrive in a continuous flow with no indication of where one packet ends and the next begins. Conduyt uses COBS (Consistent Overhead Byte Stuffing) to solve this.

COBS eliminates 0x00 from encoded data, letting 0x00 serve as a packet delimiter. The encoding works by replacing each zero byte with a pointer to the next zero (or end of block), creating a chain that the decoder follows. Overhead is approximately 0.4% for typical packet sizes.

Alternative approaches have tradeoffs:

- **Length-prefix with escaping**: if the length field or data gets corrupted, the parser loses sync and has no reliable way to find the next packet boundary.
- **SLIP (Serial Line Internet Protocol)**: uses escape sequences that can double the size of pathological payloads (all 0xC0 or 0xDB bytes). Worst-case overhead is 100%.

COBS has a bounded worst case: one extra byte per 254 bytes of data. The decoder is stateless between packets, so a corrupted packet does not affect the next one.

## Why CRC8

An 8-bit CRC is cheap to compute: a 256-byte lookup table and one table lookup per byte of input. Conduyt uses the Dallas/Maxim polynomial 0x31, which has good Hamming distance properties for packets under 256 bytes. It detects all single-bit errors, all two-bit errors, and all odd numbers of bit errors within that range.

Full CRC16 or CRC32 costs more flash and more cycles on 8-bit MCUs. The lookup table doubles or quadruples in size. For packets that are at most a few hundred bytes, the additional error detection capability of larger CRCs provides minimal practical gain. The dominant corruption modes on serial links (noise bursts, dropped bytes) are caught reliably by CRC8 at these sizes.

## Message-oriented transports skip COBS

MQTT, WebSocket, and similar transports already deliver discrete messages with defined boundaries. When Conduyt runs over these transports, COBS framing is unnecessary and is not applied. The binary packet format stays identical; only the framing layer changes.

This separation keeps the protocol simple. The same packet structure works everywhere. The transport decides whether framing is needed, and the protocol layer does not care which transport is in use.
