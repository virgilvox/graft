/**
 * GRAFT Protocol Constants
 * Generated from protocol/constants.json — DO NOT EDIT
 *
 * GRAFT — Generic Remote Actuator Firmware Transport
 * Protocol Version: 1
 */

#ifndef GRAFT_CONSTANTS_H
#define GRAFT_CONSTANTS_H

#include <stdint.h>

/* ── Protocol ─────────────────────────────────────────── */

#define GRAFT_PROTOCOL_VERSION  0x01
#define GRAFT_MAGIC_0           0x47  /* 'G' */
#define GRAFT_MAGIC_1           0x46  /* 'F' */

/* ── Header Sizes ─────────────────────────────────────── */

#define GRAFT_HEADER_SIZE       8  /* MAGIC(2) + VER(1) + TYPE(1) + SEQ(1) + LEN(2) + CRC(1) */
#define GRAFT_MAGIC_SIZE        2
#define GRAFT_CRC_SIZE          1

/* ── Host → Device Commands ───────────────────────────── */

#define GRAFT_CMD_PING            0x01
#define GRAFT_CMD_HELLO           0x02
#define GRAFT_CMD_PIN_MODE        0x10
#define GRAFT_CMD_PIN_WRITE       0x11
#define GRAFT_CMD_PIN_READ        0x12
#define GRAFT_CMD_PIN_SUBSCRIBE   0x13
#define GRAFT_CMD_PIN_UNSUBSCRIBE 0x14
#define GRAFT_CMD_I2C_WRITE       0x20
#define GRAFT_CMD_I2C_READ        0x21
#define GRAFT_CMD_I2C_READ_REG    0x22
#define GRAFT_CMD_SPI_XFER        0x30
#define GRAFT_CMD_MOD_CMD         0x40
#define GRAFT_CMD_STREAM_START    0x50
#define GRAFT_CMD_STREAM_STOP     0x51
#define GRAFT_CMD_DS_WRITE        0x60
#define GRAFT_CMD_DS_READ         0x61
#define GRAFT_CMD_DS_SUBSCRIBE    0x62
#define GRAFT_CMD_OTA_BEGIN       0x70
#define GRAFT_CMD_OTA_CHUNK       0x71
#define GRAFT_CMD_OTA_FINALIZE    0x72
#define GRAFT_CMD_RESET           0xF0

/* ── Device → Host Events ─────────────────────────────── */

#define GRAFT_EVT_PONG            0x80
#define GRAFT_EVT_HELLO_RESP      0x81
#define GRAFT_EVT_ACK             0x82
#define GRAFT_EVT_NAK             0x83
#define GRAFT_EVT_PIN_EVENT       0x90
#define GRAFT_EVT_PIN_READ_RESP   0x91
#define GRAFT_EVT_I2C_READ_RESP   0xA0
#define GRAFT_EVT_SPI_XFER_RESP   0xB0
#define GRAFT_EVT_MOD_EVENT       0xC0
#define GRAFT_EVT_MOD_RESP        0xC1
#define GRAFT_EVT_STREAM_DATA     0xD0
#define GRAFT_EVT_DS_EVENT        0xD1
#define GRAFT_EVT_DS_READ_RESP    0xD2
#define GRAFT_EVT_LOG             0xE0
#define GRAFT_EVT_FATAL           0xFF

/* ── NAK Error Codes ──────────────────────────────────── */

#define GRAFT_ERR_UNKNOWN_TYPE         0x01
#define GRAFT_ERR_CRC_MISMATCH         0x02
#define GRAFT_ERR_PAYLOAD_TOO_LARGE    0x03
#define GRAFT_ERR_INVALID_PIN          0x04
#define GRAFT_ERR_PIN_MODE_UNSUPPORTED 0x05
#define GRAFT_ERR_I2C_NOT_AVAILABLE    0x06
#define GRAFT_ERR_I2C_NACK             0x07
#define GRAFT_ERR_MODULE_NOT_LOADED    0x08
#define GRAFT_ERR_UNKNOWN_MODULE_CMD   0x09
#define GRAFT_ERR_MODULE_BUSY          0x0A
#define GRAFT_ERR_SUB_LIMIT_REACHED    0x0B
#define GRAFT_ERR_OUT_OF_MEMORY        0x0C
#define GRAFT_ERR_UNKNOWN_DATASTREAM   0x0D
#define GRAFT_ERR_DATASTREAM_READONLY  0x0E
#define GRAFT_ERR_OTA_INVALID          0x0F
#define GRAFT_ERR_VERSION_MISMATCH     0x10

/* ── Datastream Type Codes ────────────────────────────── */

#define GRAFT_TYPE_BOOL    0x01
#define GRAFT_TYPE_INT8    0x02
#define GRAFT_TYPE_UINT8   0x03
#define GRAFT_TYPE_INT16   0x04
#define GRAFT_TYPE_UINT16  0x05
#define GRAFT_TYPE_INT32   0x06
#define GRAFT_TYPE_FLOAT32 0x07
#define GRAFT_TYPE_STRING  0x08
#define GRAFT_TYPE_BYTES   0x09

/* ── Pin Capability Bitmask ───────────────────────────── */

#define GRAFT_PIN_CAP_DIGITAL_IN  (1 << 0)
#define GRAFT_PIN_CAP_DIGITAL_OUT (1 << 1)
#define GRAFT_PIN_CAP_PWM_OUT     (1 << 2)
#define GRAFT_PIN_CAP_ANALOG_IN   (1 << 3)
#define GRAFT_PIN_CAP_I2C_SDA     (1 << 4)
#define GRAFT_PIN_CAP_I2C_SCL     (1 << 5)
#define GRAFT_PIN_CAP_SPI         (1 << 6)
#define GRAFT_PIN_CAP_INTERRUPT   (1 << 7)

/* ── Pin Modes ────────────────────────────────────────── */

#define GRAFT_PIN_MODE_INPUT        0x00
#define GRAFT_PIN_MODE_OUTPUT       0x01
#define GRAFT_PIN_MODE_PWM          0x02
#define GRAFT_PIN_MODE_ANALOG       0x03
#define GRAFT_PIN_MODE_INPUT_PULLUP 0x04

/* ── Subscribe Modes ──────────────────────────────────── */

#define GRAFT_SUB_CHANGE      0x01
#define GRAFT_SUB_RISING      0x02
#define GRAFT_SUB_FALLING     0x03
#define GRAFT_SUB_ANALOG_POLL 0x04

#endif /* GRAFT_CONSTANTS_H */
