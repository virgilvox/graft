/**
 * CONDUYT MCU Unique-ID Reader
 *
 * Per-silicon-family implementations of `conduyt_mcu_id_get(buf, len)`.
 * The `CONDUYT_MCU_ID_SOURCE_*` macro is set by board_profiles_generated.h
 * (which derives it from the board's referenced MCU profile in protocol/mcus/).
 *
 * If no source is defined, the default zero-fill is used and HELLO_RESP's
 * mcu_id field is all zeros — same behavior as v0.2.x.
 */

#ifndef CONDUYT_MCU_ID_H
#define CONDUYT_MCU_ID_H

#include <stdint.h>
#include <string.h>
#include "board_profiles_generated.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

/* ── Per-MCU vendor headers (file-scope, never inside the function body) ── */

#if defined(CONDUYT_MCU_ID_SOURCE_RA4M1) && __has_include(<bsp_api.h>)
  #include <bsp_api.h>
  #define CONDUYT_MCU_ID_HAVE_RA4M1 1
#endif

#if defined(CONDUYT_MCU_ID_SOURCE_RP2040) && __has_include(<pico/unique_id.h>)
  #include <pico/unique_id.h>
  #define CONDUYT_MCU_ID_HAVE_RP2040 1
#endif

#if (defined(CONDUYT_MCU_ID_SOURCE_ESP32) || defined(CONDUYT_MCU_ID_SOURCE_ESP8266)) && __has_include(<Esp.h>)
  #include <Esp.h>
  #define CONDUYT_MCU_ID_HAVE_ESP 1
#endif

#if defined(CONDUYT_MCU_ID_SOURCE_NRF52) && __has_include(<nrf.h>)
  #include <nrf.h>
  #define CONDUYT_MCU_ID_HAVE_NRF52 1
#endif

#if defined(CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE) && __has_include(<avr/eeprom.h>)
  #include <avr/eeprom.h>
  #define CONDUYT_MCU_ID_HAVE_AVR_EEPROM 1
  #if defined(SIGRD) && __has_include(<avr/boot.h>)
    #include <avr/boot.h>
    #define CONDUYT_MCU_ID_HAVE_AVR_BOOT 1
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fill `out` with up to `len` bytes of factory-immutable MCU identifier.
 * Implementation selected by CONDUYT_MCU_ID_SOURCE_* macro at compile time.
 * Bytes beyond what the silicon provides are zero-padded.
 */
static inline void conduyt_mcu_id_get(uint8_t *out, uint8_t len) {
    if (out == NULL || len == 0) return;
    memset(out, 0, len);

#if defined(CONDUYT_MCU_ID_HAVE_RA4M1)
    /* Renesas RA4M1 (Uno R4 family) — 16-byte unique ID via FSP. */
    bsp_unique_id_t const *uid = R_BSP_UniqueIdGet();
    uint8_t n = len < 16 ? len : 16;
    memcpy(out, uid->unique_id_bytes, n);

#elif defined(CONDUYT_MCU_ID_HAVE_RP2040)
    /* RP2040 — 8-byte ID from the external flash JEDEC RUID. */
    pico_unique_board_id_t id;
    pico_get_unique_board_id(&id);
    uint8_t n = len < 8 ? len : 8;
    memcpy(out, id.id, n);

#elif defined(CONDUYT_MCU_ID_SOURCE_ESP32) && defined(CONDUYT_MCU_ID_HAVE_ESP)
    /* ESP32 family — eFuse MAC (6 bytes). */
    uint64_t mac = ESP.getEfuseMac();
    uint8_t n = len < 6 ? len : 6;
    memcpy(out, &mac, n);

#elif defined(CONDUYT_MCU_ID_SOURCE_ESP8266) && defined(CONDUYT_MCU_ID_HAVE_ESP)
    /* ESP8266 — 24-bit chip ID, padded. */
    uint32_t id = ESP.getChipId();
    if (len >= 4) memcpy(out, &id, 4);

#elif defined(CONDUYT_MCU_ID_HAVE_NRF52)
    /* nRF52 — FICR DEVICEID (8 bytes). */
    uint32_t lo = NRF_FICR->DEVICEID[0];
    uint32_t hi = NRF_FICR->DEVICEID[1];
    if (len >= 4) memcpy(out, &lo, 4);
    if (len >= 8) memcpy(out + 4, &hi, 4);

#elif defined(CONDUYT_MCU_ID_SOURCE_TEENSY3) && defined(SIM_UIDMH) && defined(SIM_UIDL)
    /* Kinetis K20/K66 — SIM_UID 128-bit, take upper 64. */
    uint32_t hi = SIM_UIDMH;
    uint32_t lo = SIM_UIDL;
    if (len >= 4) memcpy(out, &hi, 4);
    if (len >= 8) memcpy(out + 4, &lo, 4);

#elif defined(CONDUYT_MCU_ID_SOURCE_TEENSY4) && defined(HW_OCOTP_CFG0) && defined(HW_OCOTP_CFG1)
    /* i.MX RT1062 — OCOTP CFG0/CFG1 (64-bit). */
    uint32_t lo = HW_OCOTP_CFG0;
    uint32_t hi = HW_OCOTP_CFG1;
    if (len >= 4) memcpy(out, &lo, 4);
    if (len >= 8) memcpy(out + 4, &hi, 4);

#elif defined(CONDUYT_MCU_ID_HAVE_AVR_EEPROM)
    /* AVR — no factory unique ID. Prefer EEPROM-provisioned serial.
     * Fall back to the device signature on classic AVR (megaAVR/4809
     * doesn't define SIGRD; we just zero-fill there). */
    uint8_t e0 = eeprom_read_byte((const uint8_t *)0);
    if (e0 != 0xFF) {
        for (uint8_t i = 0; i < len && i < 8; i++) {
            out[i] = eeprom_read_byte((const uint8_t *)(uintptr_t)i);
        }
    }
    #if defined(CONDUYT_MCU_ID_HAVE_AVR_BOOT)
    else {
        if (len >= 1) out[0] = boot_signature_byte_get(0x0000);
        if (len >= 2) out[1] = boot_signature_byte_get(0x0002);
        if (len >= 3) out[2] = boot_signature_byte_get(0x0004);
    }
    #endif

#endif
    /* No source matched → already zero-filled by memset above. */
}

#ifdef __cplusplus
}
#endif

#endif /* CONDUYT_MCU_ID_H */
