# Changelog

## 0.3.0 — Capability model rebuild

### ⚠️ Breaking — protocol version bumped 1 → 2

Protocol v1 shipped with a corrupted CRC8 lookup table at indices 0xE0..0xFF (each
entry XOR'd with 0x20 vs the canonical poly 0x31 table). Firmware and SDKs were
internally consistent with each other but produced non-canonical CRC8 values that
no third-party CRC-8 library would interoperate with. v2 ships the canonical
table, computed from the polynomial at codegen time.

**Migration:** flash the v0.3 firmware build before connecting with a v0.3 SDK.
Old firmware on the wire (VER byte = 0x01) is silently dropped by v0.3 firmware
and times out cleanly under v0.3 hosts.

### New — board profile system

Per-board pin capabilities are now declared in YAML data files at
`protocol/boards/<id>.yml` (one per board) and `protocol/mcus/<family>.yml`
(one per silicon family). At codegen time, `protocol/generate.ts` reads them
and emits:

- `firmware/src/conduyt/boards/board_profiles_generated.h` — Firmata-style
  `#ifdef ARDUINO_*` matrix with per-board `conduyt_board_pin_caps[]`,
  `conduyt_board_adc_channel[]`, and bus counts.
- `protocol/board-profiles.json` — same data in machine-readable form for
  SDKs and the capability-audit tooling.

Adding a new board is one YAML file. No firmware code changes.

This release ships profiles for: Arduino Uno R3, Mega 2560, Nano,
Leonardo, Nano Every, Uno R4 Minima, Uno R4 WiFi, Nano ESP32, Raspberry Pi
Pico, ESP32 DevKit, ESP32-S2 Saola-1, ESP32-S3 DevKitC-1, ESP32-C3 DevKitM-1,
NodeMCU 1.0 (ESP-12E), nRF52840 DK, Teensy 3.6, Teensy 4.0, Teensy 4.1.

### New — sketch-level pin overrides

Three new public methods on `ConduytDevice` let sketches override the
generated profile for boards with custom shields or wiring:

```cpp
device.declarePinCaps(8, CONDUYT_PIN_CAP_DIGITAL_OUT | CONDUYT_PIN_CAP_PWM_OUT);
device.declareI2cBus(1, /*sda*/ 27, /*scl*/ 26);
device.declareSpiBus(0, /*cs*/ 10, /*copi*/ 11, /*cipo*/ 12, /*sck*/ 13);
```

Overrides merge into HELLO_RESP at handshake time, so host SDKs see them
exactly like profile data.

### Fix — `buildHelloResp` produced wrong capability bytes on every board

The previous implementation used preprocessor `#ifdef A0` checks that
silently failed on every modern Arduino core (R4 Renesas, RP2040, ESP32-Arduino),
because `A0` is a `static const uint8_t` on those cores rather than a `#define`.
Even on classic AVR, the same check failed because `A0` is also `static const`
in the variant's `pins_arduino.h`. Result: ANALOG_IN was never advertised on
any pin on any board ever shipped.

The new implementation walks the generated `conduyt_board_pin_caps[]`
table from the per-board YAML profile; the broken heuristics are gone.

### Fix — `analogRead(non-ADC pin)` no longer hangs the firmware

On the Renesas RA4M1 (Uno R4), calling `analogRead` on a pin that lacks
an ADC channel blocks indefinitely waiting on a conversion that never
completes. `handlePinRead` now validates the pin against the board profile's
ANALOG_IN capability bit and returns `PIN_MODE_UNSUPPORTED` NAK before
ever calling `analogRead`. Same guard applies on every supported MCU.

### Fix — `mcu_id` populated from per-MCU unique-ID register

Previously zero-filled across the board. New per-silicon implementations
(`firmware/src/conduyt/boards/mcu_id.h`):

- Renesas RA4M1: `R_BSP_UniqueIdGet()` (16-byte factory ID, take 8)
- RP2040: `pico_get_unique_board_id()` (8-byte flash JEDEC RUID)
- ESP32 family: `ESP.getEfuseMac()` (6-byte factory MAC, padded)
- ESP8266: `ESP.getChipId()` (24-bit chip ID, padded)
- nRF52: `NRF_FICR->DEVICEID[0..1]` (64-bit factory random)
- Teensy 3.x: `SIM_UIDH/MH/ML/L` (Kinetis 128-bit, top 64)
- Teensy 4.x: `HW_OCOTP_CFG0/CFG1` (NXP OCOTP fuses, 64-bit)
- AVR: signature bytes (or EEPROM if provisioned at byte 0..7)

### Tooling

- `firmware/test/capability-audit.mjs` — Node script that connects via WebSerial-equivalent
  serial, dumps HELLO_RESP, and validates the per-pin capability bytes against the
  generated profile. Live probes each pin to detect firmware regressions
  (e.g. analogRead hangs).
- `protocol/generate.ts` — single source of truth. Now also generates
  CRC8 modules for all six SDKs (firmware C, JS, Python, Go, Rust, Swift)
  from the polynomial.

### Internal

- Protocol generator extended from ~80 lines to ~400 lines and now owns
  the CRC8 table for all SDKs.
- Self-test in the generator catches non-linear CRC tables and asserts
  spot values against the polynomial.

## 0.2.0 — Initial public release

(See README and existing docs for what was in this version.)
