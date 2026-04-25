/**
 * CONDUYT Board Profiles (auto-generated)
 *
 * Per-board capability tables. The Arduino sketch macro `ARDUINO_<board>`
 * (set by the toolchain) selects which board block compiles in.
 *
 * For boards not listed here, a fallback profile is used (DIGITAL_IN +
 * DIGITAL_OUT on every pin, no PWM/ANALOG/I2C/SPI/INTERRUPT advertised).
 * Sketches can call device.declarePinCaps(pin, caps) to override.
 *
 * Generated from protocol/boards/*.yml + protocol/mcus/*.yml.
 * DO NOT EDIT — re-run protocol/generate.ts.
 */

#ifndef CONDUYT_BOARD_PROFILES_GENERATED_H
#define CONDUYT_BOARD_PROFILES_GENERATED_H

#include <stdint.h>
#include "../core/conduyt_constants.h"

/* Each board block defines:
 *   CONDUYT_BOARD_PROFILE_KNOWN  — set to 1 when a per-board profile matched
 *   CONDUYT_BOARD_NAME           — string
 *   CONDUYT_BOARD_MCU_ID_SOURCE  — token used in selecting mcu_id_<x>.cpp
 *   CONDUYT_BOARD_PIN_COUNT      — uint8
 *   CONDUYT_BOARD_I2C_BUSES      — uint8
 *   CONDUYT_BOARD_SPI_BUSES      — uint8
 *   CONDUYT_BOARD_UART_COUNT     — uint8
 *   conduyt_board_pin_caps[]     — uint8 array of capability bitmasks
 *   conduyt_board_adc_channel[]  — uint8 array, 0xFF if not ADC-capable
 */

/* ── Arduino Nano ESP32 ────────────────────────────────────────── */
#if defined(ARDUINO_NANO_ESP32)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Nano ESP32"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp32
#define CONDUYT_MCU_ID_SOURCE_ESP32    1
#define CONDUYT_BOARD_PIN_COUNT         25
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         2
#define CONDUYT_BOARD_UART_COUNT        3

static const uint8_t conduyt_board_pin_caps[25] = {
    /*  0 D0   */ 0x87,
    /*  1 D1   */ 0x87,
    /*  2 D2   */ 0x8F,
    /*  3 D3   */ 0x8F,
    /*  4 D4   */ 0x8F,
    /*  5 D5   */ 0x8F,
    /*  6 D6   */ 0x8F,
    /*  7 D7   */ 0x8F,
    /*  8 D8   */ 0x87,
    /*  9 D9   */ 0x87,
    /* 10 D10  */ 0xC7,
    /* 11 D11  */ 0xC7,
    /* 12 D12  */ 0xC7,
    /* 13 D13  */ 0xC7,
    /* 14 LED_RED */ 0x87,
    /* 15 LED_GREEN */ 0x87,
    /* 16 LED_BLUE */ 0x87,
    /* 17 A0   */ 0x8F,
    /* 18 A1   */ 0x8F,
    /* 19 A2   */ 0x8F,
    /* 20 A3   */ 0x8F,
    /* 21 A4   */ 0x97,
    /* 22 A5   */ 0xA7,
    /* 23 A6   */ 0x87,
    /* 24 A7   */ 0x87,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[25] = {
    0xFF, 0xFF, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_NANO_ESP32 */

/* ── ESP32-C3 DevKitM-1 ────────────────────────────────────────── */
#if defined(ARDUINO_ESP32C3_DEV)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "ESP32-C3 DevKitM-1"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp32
#define CONDUYT_MCU_ID_SOURCE_ESP32    1
#define CONDUYT_BOARD_PIN_COUNT         22
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        2

static const uint8_t conduyt_board_pin_caps[22] = {
    /*  0 GPIO0 */ 0x8F,
    /*  1 GPIO1 */ 0x8F,
    /*  2 GPIO2 */ 0x8F,
    /*  3 GPIO3 */ 0x8F,
    /*  4 GPIO4 */ 0xCF,
    /*  5 GPIO5 */ 0xC7,
    /*  6 GPIO6 */ 0xC7,
    /*  7 GPIO7 */ 0xC7,
    /*  8 GPIO8 */ 0x97,
    /*  9 GPIO9 */ 0xA7,
    /* 10 GPIO10 */ 0x87,
    /* 11 GPIO11 */ 0x00,
    /* 12 GPIO12 */ 0x00,
    /* 13 GPIO13 */ 0x00,
    /* 14 GPIO14 */ 0x00,
    /* 15 GPIO15 */ 0x00,
    /* 16 GPIO16 */ 0x00,
    /* 17 GPIO17 */ 0x00,
    /* 18 GPIO18 */ 0x87,
    /* 19 GPIO19 */ 0x87,
    /* 20 GPIO20 */ 0x87,
    /* 21 GPIO21 */ 0x87,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[22] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_ESP32C3_DEV */

/* ── ESP32-S2 Saola-1 ──────────────────────────────────────────── */
#if defined(ARDUINO_ESP32S2_DEV)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "ESP32-S2 Saola-1"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp32
#define CONDUYT_MCU_ID_SOURCE_ESP32    1
#define CONDUYT_BOARD_PIN_COUNT         47
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         2
#define CONDUYT_BOARD_UART_COUNT        2

static const uint8_t conduyt_board_pin_caps[47] = {
    /*  0 GPIO0 */ 0x87,
    /*  1 GPIO1 */ 0x8F,
    /*  2 GPIO2 */ 0x8F,
    /*  3 GPIO3 */ 0x8F,
    /*  4 GPIO4 */ 0x8F,
    /*  5 GPIO5 */ 0x8F,
    /*  6 GPIO6 */ 0x8F,
    /*  7 GPIO7 */ 0x8F,
    /*  8 GPIO8 */ 0x9F,
    /*  9 GPIO9 */ 0xAF,
    /* 10 GPIO10 */ 0x8F,
    /* 11 GPIO11 */ 0x87,
    /* 12 GPIO12 */ 0x87,
    /* 13 GPIO13 */ 0x87,
    /* 14 GPIO14 */ 0x87,
    /* 15 GPIO15 */ 0x87,
    /* 16 GPIO16 */ 0x87,
    /* 17 GPIO17 */ 0x87,
    /* 18 GPIO18 */ 0x87,
    /* 19 GPIO19 */ 0x87,
    /* 20 GPIO20 */ 0x87,
    /* 21 GPIO21 */ 0x87,
    /* 22 GPIO22 */ 0x00,
    /* 23 GPIO23 */ 0x00,
    /* 24 GPIO24 */ 0x00,
    /* 25 GPIO25 */ 0x00,
    /* 26 GPIO26 */ 0x00,
    /* 27 GPIO27 */ 0x00,
    /* 28 GPIO28 */ 0x00,
    /* 29 GPIO29 */ 0x00,
    /* 30 GPIO30 */ 0x00,
    /* 31 GPIO31 */ 0x00,
    /* 32 GPIO32 */ 0x00,
    /* 33 GPIO33 */ 0x87,
    /* 34 GPIO34 */ 0xC7,
    /* 35 GPIO35 */ 0xC7,
    /* 36 GPIO36 */ 0xC7,
    /* 37 GPIO37 */ 0xC7,
    /* 38 GPIO38 */ 0x87,
    /* 39 GPIO39 */ 0x87,
    /* 40 GPIO40 */ 0x87,
    /* 41 GPIO41 */ 0x87,
    /* 42 GPIO42 */ 0x87,
    /* 43 GPIO43 */ 0x87,
    /* 44 GPIO44 */ 0x87,
    /* 45 GPIO45 */ 0x87,
    /* 46 GPIO46 */ 0x81,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[47] = {
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_ESP32S2_DEV */

/* ── ESP32-S3 DevKitC-1 ────────────────────────────────────────── */
#if defined(ARDUINO_ESP32S3_DEV)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "ESP32-S3 DevKitC-1"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp32
#define CONDUYT_MCU_ID_SOURCE_ESP32    1
#define CONDUYT_BOARD_PIN_COUNT         49
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         2
#define CONDUYT_BOARD_UART_COUNT        3

static const uint8_t conduyt_board_pin_caps[49] = {
    /*  0 GPIO0 */ 0x87,
    /*  1 GPIO1 */ 0x8F,
    /*  2 GPIO2 */ 0x8F,
    /*  3 GPIO3 */ 0x8F,
    /*  4 GPIO4 */ 0x8F,
    /*  5 GPIO5 */ 0x8F,
    /*  6 GPIO6 */ 0x8F,
    /*  7 GPIO7 */ 0x8F,
    /*  8 GPIO8 */ 0x9F,
    /*  9 GPIO9 */ 0xAF,
    /* 10 GPIO10 */ 0xCF,
    /* 11 GPIO11 */ 0xC7,
    /* 12 GPIO12 */ 0xC7,
    /* 13 GPIO13 */ 0xC7,
    /* 14 GPIO14 */ 0x87,
    /* 15 GPIO15 */ 0x87,
    /* 16 GPIO16 */ 0x87,
    /* 17 GPIO17 */ 0x87,
    /* 18 GPIO18 */ 0x87,
    /* 19 GPIO19 */ 0x87,
    /* 20 GPIO20 */ 0x87,
    /* 21 GPIO21 */ 0x87,
    /* 22 GPIO22 */ 0x00,
    /* 23 GPIO23 */ 0x00,
    /* 24 GPIO24 */ 0x00,
    /* 25 GPIO25 */ 0x00,
    /* 26 GPIO26 */ 0x00,
    /* 27 GPIO27 */ 0x00,
    /* 28 GPIO28 */ 0x00,
    /* 29 GPIO29 */ 0x00,
    /* 30 GPIO30 */ 0x00,
    /* 31 GPIO31 */ 0x00,
    /* 32 GPIO32 */ 0x00,
    /* 33 GPIO33 */ 0x87,
    /* 34 GPIO34 */ 0x87,
    /* 35 GPIO35 */ 0x87,
    /* 36 GPIO36 */ 0x87,
    /* 37 GPIO37 */ 0x87,
    /* 38 GPIO38 */ 0x87,
    /* 39 GPIO39 */ 0x87,
    /* 40 GPIO40 */ 0x87,
    /* 41 GPIO41 */ 0x87,
    /* 42 GPIO42 */ 0x87,
    /* 43 GPIO43 */ 0x87,
    /* 44 GPIO44 */ 0x87,
    /* 45 GPIO45 */ 0x87,
    /* 46 GPIO46 */ 0x87,
    /* 47 GPIO47 */ 0x87,
    /* 48 GPIO48 */ 0x87,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[49] = {
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_ESP32S3_DEV */

/* ── ESP32 DevKit ──────────────────────────────────────────────── */
#if defined(ARDUINO_ESP32_DEV)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "ESP32 DevKit"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp32
#define CONDUYT_MCU_ID_SOURCE_ESP32    1
#define CONDUYT_BOARD_PIN_COUNT         40
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         2
#define CONDUYT_BOARD_UART_COUNT        3

static const uint8_t conduyt_board_pin_caps[40] = {
    /*  0 GPIO0 */ 0x87,
    /*  1 GPIO1 */ 0x87,
    /*  2 GPIO2 */ 0x87,
    /*  3 GPIO3 */ 0x87,
    /*  4 GPIO4 */ 0x87,
    /*  5 GPIO5 */ 0xC7,
    /*  6 GPIO6 */ 0x00,
    /*  7 GPIO7 */ 0x00,
    /*  8 GPIO8 */ 0x00,
    /*  9 GPIO9 */ 0x00,
    /* 10 GPIO10 */ 0x00,
    /* 11 GPIO11 */ 0x00,
    /* 12 GPIO12 */ 0x87,
    /* 13 GPIO13 */ 0x87,
    /* 14 GPIO14 */ 0x87,
    /* 15 GPIO15 */ 0x87,
    /* 16 GPIO16 */ 0x87,
    /* 17 GPIO17 */ 0x87,
    /* 18 GPIO18 */ 0xC7,
    /* 19 GPIO19 */ 0xC7,
    /* 20 GPIO20 */ 0x00,
    /* 21 GPIO21 */ 0x97,
    /* 22 GPIO22 */ 0xA7,
    /* 23 GPIO23 */ 0xC7,
    /* 24 GPIO24 */ 0x00,
    /* 25 GPIO25 */ 0x87,
    /* 26 GPIO26 */ 0x87,
    /* 27 GPIO27 */ 0x87,
    /* 28 GPIO28 */ 0x00,
    /* 29 GPIO29 */ 0x00,
    /* 30 GPIO30 */ 0x00,
    /* 31 GPIO31 */ 0x00,
    /* 32 GPIO32 */ 0x8F,
    /* 33 GPIO33 */ 0x8F,
    /* 34 GPIO34 */ 0x89,
    /* 35 GPIO35 */ 0x89,
    /* 36 GPIO36 */ 0x89,
    /* 37 GPIO37 */ 0x89,
    /* 38 GPIO38 */ 0x89,
    /* 39 GPIO39 */ 0x89,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[40] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x05, 0x06, 0x07, 0x00, 0x01, 0x02, 0x03
};

#endif /* ARDUINO_ESP32_DEV */

/* ── Arduino Leonardo ──────────────────────────────────────────── */
#if defined(ARDUINO_AVR_LEONARDO)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Leonardo"
#define CONDUYT_BOARD_MCU_ID_SOURCE     avr_signature
#define CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE    1
#define CONDUYT_BOARD_PIN_COUNT         30
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[30] = {
    /*  0 D0   */ 0x83,
    /*  1 D1   */ 0x83,
    /*  2 D2   */ 0x93,
    /*  3 D3   */ 0xA7,
    /*  4 D4   */ 0x0B,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x0F,
    /*  7 D7   */ 0x83,
    /*  8 D8   */ 0x0B,
    /*  9 D9   */ 0x0F,
    /* 10 D10  */ 0x0F,
    /* 11 D11  */ 0x07,
    /* 12 D12  */ 0x0B,
    /* 13 D13  */ 0x07,
    /* 14 D14  */ 0x43,
    /* 15 D15  */ 0x43,
    /* 16 D16  */ 0x43,
    /* 17 D17  */ 0x43,
    /* 18 A0   */ 0x0B,
    /* 19 A1   */ 0x0B,
    /* 20 A2   */ 0x0B,
    /* 21 A3   */ 0x0B,
    /* 22 A4   */ 0x0B,
    /* 23 A5   */ 0x0B,
    /* 24 A6   */ 0x08,
    /* 25 A7   */ 0x08,
    /* 26 A8   */ 0x08,
    /* 27 A9   */ 0x08,
    /* 28 A10  */ 0x08,
    /* 29 A11  */ 0x08,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[30] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0xFF, 0x0A, 0xFF, 0x0B, 0x0C, 0x0D, 0xFF, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x06, 0x05, 0x04, 0x01, 0x00, 0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x09
};

#endif /* ARDUINO_AVR_LEONARDO */

/* ── Arduino Mega 2560 ─────────────────────────────────────────── */
#if defined(ARDUINO_AVR_MEGA2560)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Mega 2560"
#define CONDUYT_BOARD_MCU_ID_SOURCE     avr_signature
#define CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE    1
#define CONDUYT_BOARD_PIN_COUNT         70
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        4

static const uint8_t conduyt_board_pin_caps[70] = {
    /*  0 D0   */ 0x03,
    /*  1 D1   */ 0x03,
    /*  2 D2   */ 0x87,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x07,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x07,
    /*  7 D7   */ 0x07,
    /*  8 D8   */ 0x07,
    /*  9 D9   */ 0x07,
    /* 10 D10  */ 0x07,
    /* 11 D11  */ 0x07,
    /* 12 D12  */ 0x07,
    /* 13 D13  */ 0x07,
    /* 14 D14  */ 0x03,
    /* 15 D15  */ 0x03,
    /* 16 D16  */ 0x03,
    /* 17 D17  */ 0x03,
    /* 18 D18  */ 0x83,
    /* 19 D19  */ 0x83,
    /* 20 D20  */ 0x93,
    /* 21 D21  */ 0xA3,
    /* 22 D22  */ 0x03,
    /* 23 D23  */ 0x03,
    /* 24 D24  */ 0x03,
    /* 25 D25  */ 0x03,
    /* 26 D26  */ 0x03,
    /* 27 D27  */ 0x03,
    /* 28 D28  */ 0x03,
    /* 29 D29  */ 0x03,
    /* 30 D30  */ 0x03,
    /* 31 D31  */ 0x03,
    /* 32 D32  */ 0x03,
    /* 33 D33  */ 0x03,
    /* 34 D34  */ 0x03,
    /* 35 D35  */ 0x03,
    /* 36 D36  */ 0x03,
    /* 37 D37  */ 0x03,
    /* 38 D38  */ 0x03,
    /* 39 D39  */ 0x03,
    /* 40 D40  */ 0x03,
    /* 41 D41  */ 0x03,
    /* 42 D42  */ 0x03,
    /* 43 D43  */ 0x03,
    /* 44 D44  */ 0x07,
    /* 45 D45  */ 0x07,
    /* 46 D46  */ 0x07,
    /* 47 D47  */ 0x03,
    /* 48 D48  */ 0x03,
    /* 49 D49  */ 0x03,
    /* 50 D50  */ 0x43,
    /* 51 D51  */ 0x43,
    /* 52 D52  */ 0x43,
    /* 53 D53  */ 0x43,
    /* 54 A0   */ 0x0B,
    /* 55 A1   */ 0x0B,
    /* 56 A2   */ 0x0B,
    /* 57 A3   */ 0x0B,
    /* 58 A4   */ 0x0B,
    /* 59 A5   */ 0x0B,
    /* 60 A6   */ 0x0B,
    /* 61 A7   */ 0x0B,
    /* 62 A8   */ 0x0B,
    /* 63 A9   */ 0x0B,
    /* 64 A10  */ 0x0B,
    /* 65 A11  */ 0x0B,
    /* 66 A12  */ 0x0B,
    /* 67 A13  */ 0x0B,
    /* 68 A14  */ 0x0B,
    /* 69 A15  */ 0x0B,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[70] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

#endif /* ARDUINO_AVR_MEGA2560 */

/* ── Arduino Nano ──────────────────────────────────────────────── */
#if defined(ARDUINO_AVR_NANO)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Nano"
#define CONDUYT_BOARD_MCU_ID_SOURCE     avr_signature
#define CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE    1
#define CONDUYT_BOARD_PIN_COUNT         22
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[22] = {
    /*  0 D0   */ 0x03,
    /*  1 D1   */ 0x03,
    /*  2 D2   */ 0x83,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x03,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x07,
    /*  7 D7   */ 0x03,
    /*  8 D8   */ 0x03,
    /*  9 D9   */ 0x07,
    /* 10 D10  */ 0x47,
    /* 11 D11  */ 0x47,
    /* 12 D12  */ 0x43,
    /* 13 D13  */ 0x43,
    /* 14 A0   */ 0x0B,
    /* 15 A1   */ 0x0B,
    /* 16 A2   */ 0x0B,
    /* 17 A3   */ 0x0B,
    /* 18 A4   */ 0x1B,
    /* 19 A5   */ 0x2B,
    /* 20 A6   */ 0x08,
    /* 21 A7   */ 0x08,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[22] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};

#endif /* ARDUINO_AVR_NANO */

/* ── Arduino Nano Every ────────────────────────────────────────── */
#if defined(ARDUINO_AVR_NANO_EVERY)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Nano Every"
#define CONDUYT_BOARD_MCU_ID_SOURCE     avr_signature
#define CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE    1
#define CONDUYT_BOARD_PIN_COUNT         22
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[22] = {
    /*  0 D0   */ 0x83,
    /*  1 D1   */ 0x83,
    /*  2 D2   */ 0x83,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x83,
    /*  5 D5   */ 0x87,
    /*  6 D6   */ 0x87,
    /*  7 D7   */ 0x83,
    /*  8 D8   */ 0xC3,
    /*  9 D9   */ 0x87,
    /* 10 D10  */ 0x87,
    /* 11 D11  */ 0xC3,
    /* 12 D12  */ 0xC3,
    /* 13 D13  */ 0xC3,
    /* 14 A0   */ 0x8B,
    /* 15 A1   */ 0x8B,
    /* 16 A2   */ 0x8B,
    /* 17 A3   */ 0x8B,
    /* 18 A4   */ 0x9B,
    /* 19 A5   */ 0xAB,
    /* 20 A6   */ 0x8B,
    /* 21 A7   */ 0x8B,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[22] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x02, 0x01, 0x00, 0x0C, 0x0D, 0x04, 0x05
};

#endif /* ARDUINO_AVR_NANO_EVERY */

/* ── NodeMCU 1.0 (ESP-12E) ─────────────────────────────────────── */
#if defined(ARDUINO_ESP8266_NODEMCU_ESP12E)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "NodeMCU 1.0 (ESP-12E)"
#define CONDUYT_BOARD_MCU_ID_SOURCE     esp8266
#define CONDUYT_MCU_ID_SOURCE_ESP8266    1
#define CONDUYT_BOARD_PIN_COUNT         18
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        2

static const uint8_t conduyt_board_pin_caps[18] = {
    /*  0 GPIO0 */ 0x87,
    /*  1 GPIO1 */ 0x87,
    /*  2 GPIO2 */ 0x87,
    /*  3 GPIO3 */ 0x87,
    /*  4 GPIO4 */ 0x97,
    /*  5 GPIO5 */ 0xA7,
    /*  6 GPIO6 */ 0x00,
    /*  7 GPIO7 */ 0x00,
    /*  8 GPIO8 */ 0x00,
    /*  9 GPIO9 */ 0x00,
    /* 10 GPIO10 */ 0x00,
    /* 11 GPIO11 */ 0x00,
    /* 12 GPIO12 */ 0xC7,
    /* 13 GPIO13 */ 0xC7,
    /* 14 GPIO14 */ 0xC7,
    /* 15 GPIO15 */ 0xC7,
    /* 16 GPIO16 */ 0x07,
    /* 17 A0   */ 0x08,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[18] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

#endif /* ARDUINO_ESP8266_NODEMCU_ESP12E */

/* ── nRF52840 DK (PCA10056) ────────────────────────────────────── */
#if defined(ARDUINO_NRF52840_PCA10056)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "nRF52840 DK (PCA10056)"
#define CONDUYT_BOARD_MCU_ID_SOURCE     nrf52
#define CONDUYT_MCU_ID_SOURCE_NRF52    1
#define CONDUYT_BOARD_PIN_COUNT         48
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        2

static const uint8_t conduyt_board_pin_caps[48] = {
    /*  0 P0_00 */ 0x00,
    /*  1 P0_01 */ 0x00,
    /*  2 P0_02 */ 0x8F,
    /*  3 P0_03 */ 0x8F,
    /*  4 P0_04 */ 0x8F,
    /*  5 P0_05 */ 0x8F,
    /*  6 P0_06 */ 0x87,
    /*  7 P0_07 */ 0x87,
    /*  8 P0_08 */ 0x87,
    /*  9 P0_09 */ 0x83,
    /* 10 P0_10 */ 0x83,
    /* 11 P0_11 */ 0x87,
    /* 12 P0_12 */ 0x87,
    /* 13 P0_13 */ 0x87,
    /* 14 P0_14 */ 0x87,
    /* 15 P0_15 */ 0x87,
    /* 16 P0_16 */ 0x87,
    /* 17 P0_17 */ 0x00,
    /* 18 P0_18 */ 0x87,
    /* 19 P0_19 */ 0x00,
    /* 20 P0_20 */ 0x00,
    /* 21 P0_21 */ 0x00,
    /* 22 P0_22 */ 0x00,
    /* 23 P0_23 */ 0x00,
    /* 24 P0_24 */ 0x87,
    /* 25 P0_25 */ 0x87,
    /* 26 P0_26 */ 0x97,
    /* 27 P0_27 */ 0xA7,
    /* 28 P0_28 */ 0x8F,
    /* 29 P0_29 */ 0x8F,
    /* 30 P0_30 */ 0x8F,
    /* 31 P0_31 */ 0x8F,
    /* 32 P1_00 */ 0x87,
    /* 33 P1_01 */ 0x87,
    /* 34 P1_02 */ 0x87,
    /* 35 P1_03 */ 0x87,
    /* 36 P1_04 */ 0x87,
    /* 37 P1_05 */ 0x87,
    /* 38 P1_06 */ 0x87,
    /* 39 P1_07 */ 0x87,
    /* 40 P1_08 */ 0x87,
    /* 41 P1_09 */ 0x87,
    /* 42 P1_10 */ 0x87,
    /* 43 P1_11 */ 0x87,
    /* 44 P1_12 */ 0x87,
    /* 45 P1_13 */ 0xC7,
    /* 46 P1_14 */ 0xC7,
    /* 47 P1_15 */ 0xC7,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[48] = {
    0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x05, 0x06, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_NRF52840_PCA10056 */

/* ── Raspberry Pi Pico ─────────────────────────────────────────── */
#if defined(ARDUINO_RASPBERRY_PI_PICO)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Raspberry Pi Pico"
#define CONDUYT_BOARD_MCU_ID_SOURCE     rp2040
#define CONDUYT_MCU_ID_SOURCE_RP2040    1
#define CONDUYT_BOARD_PIN_COUNT         30
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         2
#define CONDUYT_BOARD_UART_COUNT        2

static const uint8_t conduyt_board_pin_caps[30] = {
    /*  0 GP0  */ 0x87,
    /*  1 GP1  */ 0x87,
    /*  2 GP2  */ 0x87,
    /*  3 GP3  */ 0x87,
    /*  4 GP4  */ 0x97,
    /*  5 GP5  */ 0xA7,
    /*  6 GP6  */ 0x87,
    /*  7 GP7  */ 0x87,
    /*  8 GP8  */ 0x87,
    /*  9 GP9  */ 0x87,
    /* 10 GP10 */ 0x87,
    /* 11 GP11 */ 0x87,
    /* 12 GP12 */ 0xC7,
    /* 13 GP13 */ 0xC7,
    /* 14 GP14 */ 0xC7,
    /* 15 GP15 */ 0xC7,
    /* 16 GP16 */ 0xC7,
    /* 17 GP17 */ 0xC7,
    /* 18 GP18 */ 0xC7,
    /* 19 GP19 */ 0xC7,
    /* 20 GP20 */ 0x87,
    /* 21 GP21 */ 0x87,
    /* 22 GP22 */ 0x87,
    /* 23 GP23 */ 0x00,
    /* 24 GP24 */ 0x00,
    /* 25 GP25 */ 0x87,
    /* 26 GP26 */ 0x9F,
    /* 27 GP27 */ 0xAF,
    /* 28 GP28 */ 0x8F,
    /* 29 GP29 */ 0x00,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[30] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0xFF
};

#endif /* ARDUINO_RASPBERRY_PI_PICO */

/* ── Teensy 3.6 ────────────────────────────────────────────────── */
#if defined(__MK66FX1M0__)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Teensy 3.6"
#define CONDUYT_BOARD_MCU_ID_SOURCE     teensy3
#define CONDUYT_MCU_ID_SOURCE_TEENSY3    1
#define CONDUYT_BOARD_PIN_COUNT         58
#define CONDUYT_BOARD_I2C_BUSES         3
#define CONDUYT_BOARD_SPI_BUSES         3
#define CONDUYT_BOARD_UART_COUNT        6

static const uint8_t conduyt_board_pin_caps[58] = {
    /*  0 D0   */ 0xC3,
    /*  1 D1   */ 0xC3,
    /*  2 D2   */ 0x87,
    /*  3 D3   */ 0xA7,
    /*  4 D4   */ 0x97,
    /*  5 D5   */ 0x87,
    /*  6 D6   */ 0xC7,
    /*  7 D7   */ 0x87,
    /*  8 D8   */ 0x87,
    /*  9 D9   */ 0x87,
    /* 10 D10  */ 0xC7,
    /* 11 D11  */ 0xC3,
    /* 12 D12  */ 0xC3,
    /* 13 D13  */ 0xC3,
    /* 14 D14  */ 0x8F,
    /* 15 D15  */ 0x8B,
    /* 16 D16  */ 0x8F,
    /* 17 D17  */ 0x8F,
    /* 18 D18  */ 0x9B,
    /* 19 D19  */ 0xAB,
    /* 20 D20  */ 0x8F,
    /* 21 D21  */ 0x8F,
    /* 22 D22  */ 0x8F,
    /* 23 D23  */ 0x8F,
    /* 24 D24  */ 0x83,
    /* 25 D25  */ 0x83,
    /* 26 D26  */ 0x83,
    /* 27 D27  */ 0x83,
    /* 28 D28  */ 0x83,
    /* 29 D29  */ 0x87,
    /* 30 D30  */ 0x87,
    /* 31 D31  */ 0x8B,
    /* 32 D32  */ 0xCB,
    /* 33 D33  */ 0x8B,
    /* 34 D34  */ 0x8B,
    /* 35 D35  */ 0x8F,
    /* 36 D36  */ 0x8F,
    /* 37 D37  */ 0x8F,
    /* 38 D38  */ 0x8F,
    /* 39 D39  */ 0x8B,
    /* 40 D40  */ 0x83,
    /* 41 D41  */ 0x83,
    /* 42 D42  */ 0x83,
    /* 43 D43  */ 0xC3,
    /* 44 D44  */ 0xC3,
    /* 45 D45  */ 0xC3,
    /* 46 D46  */ 0xC3,
    /* 47 D47  */ 0x83,
    /* 48 D48  */ 0x83,
    /* 49 D49  */ 0x8B,
    /* 50 D50  */ 0x8B,
    /* 51 D51  */ 0x83,
    /* 52 D52  */ 0x83,
    /* 53 D53  */ 0x83,
    /* 54 D54  */ 0x83,
    /* 55 D55  */ 0x83,
    /* 56 D56  */ 0x83,
    /* 57 D57  */ 0x83,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[58] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x0E, 0x08, 0x09, 0x0D, 0x0C, 0x06, 0x07, 0x0F, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0x12, 0x84, 0x85, 0x86, 0x87, 0x91, 0x11, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8A, 0x8B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* __MK66FX1M0__ */

/* ── Teensy 4.0 ────────────────────────────────────────────────── */
#if defined(ARDUINO_TEENSY40)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Teensy 4.0"
#define CONDUYT_BOARD_MCU_ID_SOURCE     teensy4
#define CONDUYT_MCU_ID_SOURCE_TEENSY4    1
#define CONDUYT_BOARD_PIN_COUNT         40
#define CONDUYT_BOARD_I2C_BUSES         3
#define CONDUYT_BOARD_SPI_BUSES         3
#define CONDUYT_BOARD_UART_COUNT        7

static const uint8_t conduyt_board_pin_caps[40] = {
    /*  0 D0   */ 0xC7,
    /*  1 D1   */ 0xC7,
    /*  2 D2   */ 0x87,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x87,
    /*  5 D5   */ 0x87,
    /*  6 D6   */ 0x87,
    /*  7 D7   */ 0x87,
    /*  8 D8   */ 0x87,
    /*  9 D9   */ 0x87,
    /* 10 D10  */ 0xC7,
    /* 11 D11  */ 0xC7,
    /* 12 D12  */ 0xC7,
    /* 13 D13  */ 0xC7,
    /* 14 D14  */ 0x8F,
    /* 15 D15  */ 0x8F,
    /* 16 D16  */ 0xAB,
    /* 17 D17  */ 0x9B,
    /* 18 D18  */ 0x9F,
    /* 19 D19  */ 0xAF,
    /* 20 D20  */ 0x8B,
    /* 21 D21  */ 0x8B,
    /* 22 D22  */ 0x8F,
    /* 23 D23  */ 0x8F,
    /* 24 D24  */ 0xAF,
    /* 25 D25  */ 0x9F,
    /* 26 D26  */ 0xCB,
    /* 27 D27  */ 0xCB,
    /* 28 D28  */ 0x87,
    /* 29 D29  */ 0x87,
    /* 30 D30  */ 0x83,
    /* 31 D31  */ 0x83,
    /* 32 D32  */ 0x83,
    /* 33 D33  */ 0x87,
    /* 34 D34  */ 0xC7,
    /* 35 D35  */ 0xC7,
    /* 36 D36  */ 0xC7,
    /* 37 D37  */ 0xC7,
    /* 38 D38  */ 0x87,
    /* 39 D39  */ 0x87,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[40] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x08, 0x0C, 0x0B, 0x06, 0x05, 0x04, 0x03, 0x08, 0x09, 0x01, 0x02, 0x83, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_TEENSY40 */

/* ── Teensy 4.1 ────────────────────────────────────────────────── */
#if defined(ARDUINO_TEENSY41)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Teensy 4.1"
#define CONDUYT_BOARD_MCU_ID_SOURCE     teensy4
#define CONDUYT_MCU_ID_SOURCE_TEENSY4    1
#define CONDUYT_BOARD_PIN_COUNT         55
#define CONDUYT_BOARD_I2C_BUSES         3
#define CONDUYT_BOARD_SPI_BUSES         3
#define CONDUYT_BOARD_UART_COUNT        8

static const uint8_t conduyt_board_pin_caps[55] = {
    /*  0 D0   */ 0xC7,
    /*  1 D1   */ 0xC7,
    /*  2 D2   */ 0x87,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x87,
    /*  5 D5   */ 0x87,
    /*  6 D6   */ 0x87,
    /*  7 D7   */ 0x87,
    /*  8 D8   */ 0x87,
    /*  9 D9   */ 0x87,
    /* 10 D10  */ 0xC7,
    /* 11 D11  */ 0xC7,
    /* 12 D12  */ 0xC7,
    /* 13 D13  */ 0xC7,
    /* 14 D14  */ 0x8F,
    /* 15 D15  */ 0x8F,
    /* 16 D16  */ 0xAB,
    /* 17 D17  */ 0x9B,
    /* 18 D18  */ 0x9F,
    /* 19 D19  */ 0xAF,
    /* 20 D20  */ 0x8B,
    /* 21 D21  */ 0x8B,
    /* 22 D22  */ 0x8F,
    /* 23 D23  */ 0x8F,
    /* 24 D24  */ 0xAF,
    /* 25 D25  */ 0x9F,
    /* 26 D26  */ 0xCB,
    /* 27 D27  */ 0xCB,
    /* 28 D28  */ 0x87,
    /* 29 D29  */ 0x87,
    /* 30 D30  */ 0x83,
    /* 31 D31  */ 0x83,
    /* 32 D32  */ 0x83,
    /* 33 D33  */ 0x87,
    /* 34 D34  */ 0x83,
    /* 35 D35  */ 0x83,
    /* 36 D36  */ 0x87,
    /* 37 D37  */ 0x87,
    /* 38 D38  */ 0x8B,
    /* 39 D39  */ 0x8B,
    /* 40 D40  */ 0x8B,
    /* 41 D41  */ 0x8B,
    /* 42 D42  */ 0x87,
    /* 43 D43  */ 0xC7,
    /* 44 D44  */ 0x87,
    /* 45 D45  */ 0x87,
    /* 46 D46  */ 0x87,
    /* 47 D47  */ 0x87,
    /* 48 D48  */ 0x83,
    /* 49 D49  */ 0xC3,
    /* 50 D50  */ 0xC3,
    /* 51 D51  */ 0x87,
    /* 52 D52  */ 0x83,
    /* 53 D53  */ 0x83,
    /* 54 D54  */ 0xC7,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[55] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x08, 0x0C, 0x0B, 0x06, 0x05, 0x04, 0x03, 0x08, 0x09, 0x01, 0x02, 0x83, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11, 0x12, 0x89, 0x8A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#endif /* ARDUINO_TEENSY41 */

/* ── Arduino Uno R3 ────────────────────────────────────────────── */
#if defined(ARDUINO_AVR_UNO)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Uno R3"
#define CONDUYT_BOARD_MCU_ID_SOURCE     avr_signature
#define CONDUYT_MCU_ID_SOURCE_AVR_SIGNATURE    1
#define CONDUYT_BOARD_PIN_COUNT         20
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[20] = {
    /*  0 D0   */ 0x03,
    /*  1 D1   */ 0x03,
    /*  2 D2   */ 0x83,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x03,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x07,
    /*  7 D7   */ 0x03,
    /*  8 D8   */ 0x03,
    /*  9 D9   */ 0x07,
    /* 10 D10  */ 0x47,
    /* 11 D11  */ 0x47,
    /* 12 D12  */ 0x43,
    /* 13 D13  */ 0x43,
    /* 14 A0   */ 0x0B,
    /* 15 A1   */ 0x0B,
    /* 16 A2   */ 0x0B,
    /* 17 A3   */ 0x0B,
    /* 18 A4   */ 0x1B,
    /* 19 A5   */ 0x2B,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[20] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05
};

#endif /* ARDUINO_AVR_UNO */

/* ── Arduino Uno R4 Minima ─────────────────────────────────────── */
#if defined(ARDUINO_UNOR4_MINIMA)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Uno R4 Minima"
#define CONDUYT_BOARD_MCU_ID_SOURCE     ra4m1
#define CONDUYT_MCU_ID_SOURCE_RA4M1    1
#define CONDUYT_BOARD_PIN_COUNT         20
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[20] = {
    /*  0 D0   */ 0x83,
    /*  1 D1   */ 0x83,
    /*  2 D2   */ 0x83,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x03,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x07,
    /*  7 D7   */ 0x03,
    /*  8 D8   */ 0x83,
    /*  9 D9   */ 0x07,
    /* 10 D10  */ 0x47,
    /* 11 D11  */ 0x47,
    /* 12 D12  */ 0xC7,
    /* 13 D13  */ 0xC7,
    /* 14 A0   */ 0x0B,
    /* 15 A1   */ 0x8B,
    /* 16 A2   */ 0x8B,
    /* 17 A3   */ 0x8B,
    /* 18 A4   */ 0x9B,
    /* 19 A5   */ 0xAB,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[20] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x09, 0x00, 0x01, 0x02, 0x16, 0x15
};

#endif /* ARDUINO_UNOR4_MINIMA */

/* ── Arduino Uno R4 WiFi ───────────────────────────────────────── */
#if defined(ARDUINO_UNOR4_WIFI)

#define CONDUYT_BOARD_PROFILE_KNOWN     1
#define CONDUYT_BOARD_NAME              "Arduino Uno R4 WiFi"
#define CONDUYT_BOARD_MCU_ID_SOURCE     ra4m1
#define CONDUYT_MCU_ID_SOURCE_RA4M1    1
#define CONDUYT_BOARD_PIN_COUNT         20
#define CONDUYT_BOARD_I2C_BUSES         2
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1

static const uint8_t conduyt_board_pin_caps[20] = {
    /*  0 D0   */ 0x87,
    /*  1 D1   */ 0x87,
    /*  2 D2   */ 0x87,
    /*  3 D3   */ 0x87,
    /*  4 D4   */ 0x07,
    /*  5 D5   */ 0x07,
    /*  6 D6   */ 0x87,
    /*  7 D7   */ 0x07,
    /*  8 D8   */ 0x87,
    /*  9 D9   */ 0x07,
    /* 10 D10  */ 0x47,
    /* 11 D11  */ 0xC7,
    /* 12 D12  */ 0xC7,
    /* 13 D13  */ 0x47,
    /* 14 A0   */ 0x0B,
    /* 15 A1   */ 0x8B,
    /* 16 A2   */ 0x8B,
    /* 17 A3   */ 0x8B,
    /* 18 A4   */ 0x9F,
    /* 19 A5   */ 0xAF,
};

#define CONDUYT_BOARD_ADC_NONE         0xFF
static const uint8_t conduyt_board_adc_channel[20] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x09, 0x00, 0x01, 0x02, 0x16, 0x15
};

#endif /* ARDUINO_UNOR4_WIFI */

/* ── Fallback (unknown board) ─────────────────────────────────── */
#ifndef CONDUYT_BOARD_PROFILE_KNOWN
#define CONDUYT_BOARD_PROFILE_KNOWN     0
#define CONDUYT_BOARD_NAME              "unknown"
#define CONDUYT_BOARD_MCU_ID_SOURCE     none
#ifdef NUM_DIGITAL_PINS
#define CONDUYT_BOARD_PIN_COUNT         NUM_DIGITAL_PINS
#else
#define CONDUYT_BOARD_PIN_COUNT         20
#endif
#define CONDUYT_BOARD_I2C_BUSES         1
#define CONDUYT_BOARD_SPI_BUSES         1
#define CONDUYT_BOARD_UART_COUNT        1
/* No static caps array. ConduytDevice falls back to runtime probing of
 * digitalPinHasPWM / digitalPinToInterrupt / etc. */
#endif /* fallback */

#endif /* CONDUYT_BOARD_PROFILES_GENERATED_H */
