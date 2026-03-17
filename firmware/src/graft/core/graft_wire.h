/**
 * GRAFT Wire Format
 * Pure C packet encode/decode. No Arduino or platform dependencies.
 *
 * Packet layout:
 *   MAGIC(2) + VER(1) + TYPE(1) + SEQ(1) + LEN(2) + PAYLOAD(N) + CRC8(1)
 *
 * CRC8 is computed over bytes [VER..end of PAYLOAD] (excludes MAGIC and CRC byte itself).
 * LEN is little-endian uint16.
 */

#ifndef GRAFT_WIRE_H
#define GRAFT_WIRE_H

#include <stdint.h>
#include <stddef.h>

#include "graft_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode a GraftPacket into a raw byte buffer (without COBS).
 *
 * @param dst      Output buffer
 * @param dst_len  Size of output buffer
 * @param pkt      Packet to encode (payload must be set, crc is computed)
 * @return Number of bytes written, or 0 on error (buffer too small)
 */
size_t graft_wire_encode(uint8_t *dst, size_t dst_len, const GraftPacket *pkt);

/**
 * Decode a raw byte buffer into a GraftPacket (without COBS).
 * The packet's payload pointer will reference into the src buffer.
 *
 * @param pkt      Output packet structure
 * @param src      Raw packet bytes (MAGIC through CRC)
 * @param src_len  Number of bytes available
 * @return GRAFT_OK on success, error code otherwise
 */
GraftResult graft_wire_decode(GraftPacket *pkt, const uint8_t *src, size_t src_len);

/**
 * Compute the total wire size for a packet with the given payload length.
 */
static inline size_t graft_wire_packet_size(uint16_t payload_len) {
    return GRAFT_HEADER_SIZE + payload_len;
    /* MAGIC(2) + VER(1) + TYPE(1) + SEQ(1) + LEN(2) + PAYLOAD(N) + CRC(1) = 8 + N */
}

/**
 * Quick check if a buffer starts with valid GRAFT magic bytes.
 */
static inline int graft_wire_has_magic(const uint8_t *buf, size_t len) {
    return len >= 2 && buf[0] == GRAFT_MAGIC_0 && buf[1] == GRAFT_MAGIC_1;
}

#ifdef __cplusplus
}
#endif

#endif /* GRAFT_WIRE_H */
