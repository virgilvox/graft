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

#if defined(CONDUYT_MCU_ID_SOURCE_RA4M1)
    /* Renesas RA4M1 (Uno R4 family).
     * 16-byte unique ID via R_BSP_UniqueIdGet() (FSP).
     * Reference: ra/fsp/src/bsp/mcu/all/bsp_common.h. */
    #if __has_include(<bsp_api.h>)
        #include <bsp_api.h>
        bsp_unique_id_t const *uid = R_BSP_UniqueIdGet();
        uint8_t n = len < 16 ? len : 16;
        memcpy(out, uid->unique_id_bytes, n);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_RP2040)
    /* Raspberry Pi RP2040.
     * 8-byte unique ID from external flash JEDEC RUID. */
    #if __has_include(<pico/unique_id.h>)
        #include <pico/unique_id.h>
        pico_unique_board_id_t id;
        pico_get_unique_board_id(&id);
        uint8_t n = len < 8 ? len : 8;
        memcpy(out, id.id, n);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_ESP32)
    /* ESP32 family — eFuse MAC (6 bytes). */
    #if __has_include(<Esp.h>)
        #include <Esp.h>
        uint64_t mac = ESP.getEfuseMac();
        uint8_t n = len < 6 ? len : 6;
        memcpy(out, &mac, n);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_NRF52)
    /* nRF52 — FICR DEVICEID (8 bytes). */
    #if __has_include(<nrf.h>)
        #include <nrf.h>
        uint32_t lo = NRF_FICR->DEVICEID[0];
        uint32_t hi = NRF_FICR->DEVICEID[1];
        if (len >= 4) memcpy(out, &lo, 4);
        if (len >= 8) memcpy(out + 4, &hi, 4);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_TEENSY3)
    /* Kinetis K20/K66 — SIM_UIDH/MH/ML/L (128-bit total, take 64). */
    #if defined(SIM_UIDMH) && defined(SIM_UIDL)
        uint32_t hi = SIM_UIDMH;
        uint32_t lo = SIM_UIDL;
        if (len >= 4) memcpy(out, &hi, 4);
        if (len >= 8) memcpy(out + 4, &lo, 4);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_TEENSY4)
    /* i.MX RT1062 — OCOTP CFG0/CFG1 (64-bit unique ID). */
    #if defined(HW_OCOTP_CFG0) && defined(HW_OCOTP_CFG1)
        uint32_t lo = HW_OCOTP_CFG0;
        uint32_t hi = HW_OCOTP_CFG1;
        if (len >= 4) memcpy(out, &lo, 4);
        if (len >= 8) memcpy(out + 4, &hi, 4);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_ESP8266)
    /* ESP8266 — chip ID (24-bit, padded). */
    #if __has_include(<Esp.h>)
        #include <Esp.h>
        uint32_t id = ESP.getChipId();
        if (len >= 4) memcpy(out, &id, 4);
    #endif

#elif defined(CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE)
    /* Classic AVR — no factory unique ID. Read the 3-byte device signature
     * from the boot section as a stable (but not chip-unique) identifier.
     * On boards with EEPROM, an installer could write a per-device serial
     * starting at EEPROM address 0; if found there (first byte non-0xFF),
     * that is preferred. */
    #if __has_include(<avr/boot.h>) && __has_include(<avr/eeprom.h>)
        #include <avr/boot.h>
        #include <avr/eeprom.h>
        uint8_t e0 = eeprom_read_byte((const uint8_t *)0);
        if (e0 != 0xFF) {
            for (uint8_t i = 0; i < len && i < 8; i++) {
                out[i] = eeprom_read_byte((const uint8_t *)(uintptr_t)i);
            }
        } else {
            if (len >= 1) out[0] = boot_signature_byte_get(0x0000);
            if (len >= 2) out[1] = boot_signature_byte_get(0x0002);
            if (len >= 3) out[2] = boot_signature_byte_get(0x0004);
        }
    #endif

#endif
    /* CONDUYT_MCU_ID_SOURCE_NONE / unknown → already zero-filled. */
}

#ifdef __cplusplus
}
#endif

#endif /* CONDUYT_MCU_ID_H */
