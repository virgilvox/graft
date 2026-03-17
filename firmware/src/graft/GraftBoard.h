/**
 * GRAFT Board Detection
 *
 * Compile-time board detection via #ifdef matrix.
 * Provides pin count, capability defaults, and feature flags.
 */

#ifndef GRAFT_BOARD_H
#define GRAFT_BOARD_H

#include "core/graft_constants.h"

/* ── Platform Detection ───────────────────────────────── */

#if defined(ESP32)
    #define GRAFT_PLATFORM_ESP32
    #define GRAFT_BOARD_NAME        "ESP32"
    #define GRAFT_HAS_WIFI          1
    #define GRAFT_HAS_BLE           1
    #define GRAFT_HAS_USB_CDC       0
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(ESP8266)
    #define GRAFT_PLATFORM_ESP8266
    #define GRAFT_BOARD_NAME        "ESP8266"
    #define GRAFT_HAS_WIFI          1
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       0
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(ARDUINO_ARCH_NRF52) || defined(NRF52)
    #define GRAFT_PLATFORM_NRF52
    #define GRAFT_BOARD_NAME        "nRF52"
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           1
    #define GRAFT_HAS_USB_CDC       1
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(ARDUINO_ARCH_RP2040) || defined(PICO)
    #define GRAFT_PLATFORM_RP2040
    #define GRAFT_BOARD_NAME        "RP2040"
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       1
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(ARDUINO_ARCH_STM32) || defined(STM32)
    #define GRAFT_PLATFORM_STM32
    #define GRAFT_BOARD_NAME        "STM32"
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       1
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(ARDUINO_ARCH_SAMD)
    #define GRAFT_PLATFORM_SAMD
    #define GRAFT_BOARD_NAME        "SAMD"
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       1
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   256
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       6
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 12
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   8
    #endif

#elif defined(TEENSYDUINO)
    #define GRAFT_PLATFORM_TEENSY
    #define GRAFT_BOARD_NAME        "Teensy"
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       1
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   512
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   16
    #endif

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560) || defined(__AVR__)
    #define GRAFT_PLATFORM_AVR
    #ifdef __AVR_ATmega2560__
        #define GRAFT_BOARD_NAME    "ATmega2560"
    #else
        #define GRAFT_BOARD_NAME    "ATmega328P"
    #endif
    #define GRAFT_HAS_WIFI          0
    #define GRAFT_HAS_BLE           0
    #define GRAFT_HAS_USB_CDC       0
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   128
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       4
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 8
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   4
    #endif

#else
    /* Unknown board — use conservative defaults */
    #define GRAFT_PLATFORM_UNKNOWN
    #ifndef GRAFT_BOARD_NAME
        #define GRAFT_BOARD_NAME    "Unknown"
    #endif
    #ifndef GRAFT_HAS_WIFI
        #define GRAFT_HAS_WIFI      0
    #endif
    #ifndef GRAFT_HAS_BLE
        #define GRAFT_HAS_BLE       0
    #endif
    #ifndef GRAFT_HAS_USB_CDC
        #define GRAFT_HAS_USB_CDC   0
    #endif
    #ifndef GRAFT_PACKET_BUF_SIZE
        #define GRAFT_PACKET_BUF_SIZE   256
    #endif
    #ifndef GRAFT_MAX_MODULES
        #define GRAFT_MAX_MODULES       8
    #endif
    #ifndef GRAFT_MAX_SUBSCRIPTIONS
        #define GRAFT_MAX_SUBSCRIPTIONS 16
    #endif
    #ifndef GRAFT_MAX_DATASTREAMS
        #define GRAFT_MAX_DATASTREAMS   8
    #endif

#endif

/* ── COBS buffer overhead ─────────────────────────────── */

#define GRAFT_COBS_BUF_SIZE (GRAFT_PACKET_BUF_SIZE + (GRAFT_PACKET_BUF_SIZE / 254) + 2)

#endif /* GRAFT_BOARD_H */
