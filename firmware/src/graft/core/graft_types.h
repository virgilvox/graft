/**
 * GRAFT Core Types
 * Shared type definitions for the wire format layer.
 * Pure C — no Arduino or STL dependencies.
 */

#ifndef GRAFT_TYPES_H
#define GRAFT_TYPES_H

#include <stdint.h>
#include <stddef.h>

#include "graft_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Packet Structure ─────────────────────────────────── */

/**
 * Decoded GRAFT packet. Used for both encoding and decoding.
 * The payload pointer references external memory — caller manages lifetime.
 */
typedef struct {
    uint8_t   version;       /* Protocol version (GRAFT_PROTOCOL_VERSION) */
    uint8_t   type;          /* Command or event type byte */
    uint8_t   seq;           /* Rolling sequence number 0-255 */
    uint16_t  payload_len;   /* Payload length in bytes */
    uint8_t  *payload;       /* Pointer to payload data (not owned) */
    uint8_t   crc;           /* CRC8 Dallas/Maxim over [VER..end of PAYLOAD] */
} GraftPacket;

/* ── Encode/Decode Results ────────────────────────────── */

typedef enum {
    GRAFT_OK = 0,
    GRAFT_ERR_BUFFER_TOO_SMALL,
    GRAFT_ERR_INVALID_MAGIC,
    GRAFT_ERR_INVALID_VERSION,
    GRAFT_ERR_INVALID_CRC,
    GRAFT_ERR_INCOMPLETE_PACKET,
    GRAFT_ERR_COBS_DECODE_FAIL
} GraftResult;

/* ── HELLO_RESP Sub-structures ────────────────────────── */

#define GRAFT_FIRMWARE_NAME_LEN  16
#define GRAFT_MODULE_NAME_LEN     8
#define GRAFT_DS_NAME_LEN        16
#define GRAFT_DS_UNIT_LEN         8
#define GRAFT_MCU_ID_LEN          8

typedef struct {
    uint8_t module_id;
    char    name[GRAFT_MODULE_NAME_LEN + 1];  /* null-terminated */
    uint8_t version_major;
    uint8_t version_minor;
    uint8_t pin_count;
    uint8_t pins[16]; /* max 16 pins per module */
} GraftModuleDescriptor;

typedef struct {
    char    name[GRAFT_DS_NAME_LEN + 1];  /* null-terminated */
    uint8_t type;       /* GRAFT_TYPE_* */
    char    unit[GRAFT_DS_UNIT_LEN + 1];  /* null-terminated */
    uint8_t writable;   /* 0x01 = host can write */
    uint8_t pin_ref;    /* physical pin mapping, 0xFF if none */
    uint8_t retain;     /* 0x01 = broker retains last value */
} GraftDatastreamDescriptor;

#ifdef __cplusplus
}
#endif

#endif /* GRAFT_TYPES_H */
