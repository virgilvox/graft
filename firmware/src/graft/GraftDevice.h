/**
 * GRAFT Device — Main Firmware Orchestrator
 *
 * The central class that ties transport, modules, subscriptions,
 * and datastreams together. Users create one GraftDevice in setup()
 * and call poll() in loop().
 */

#ifndef GRAFT_DEVICE_H
#define GRAFT_DEVICE_H

#include <stdint.h>
#include <string.h>

#include "core/graft_constants.h"
#include "core/graft_types.h"
#include "core/graft_wire.h"
#include "core/graft_cobs.h"
#include "core/graft_crc8.h"
#include "GraftBoard.h"
#include "GraftPayload.h"
#include "GraftContext.h"
#include "GraftModuleBase.h"
#include "transport/GraftTransport.h"

#ifdef ARDUINO
#include <Wire.h>
#include <SPI.h>
#endif

#ifdef GRAFT_OTA
#include "GraftOTA.h"
#endif

/* ── Datastream Storage ───────────────────────────────── */

struct GraftDatastreamEntry {
    char    name[GRAFT_DS_NAME_LEN + 1];
    uint8_t type;       /* GRAFT_TYPE_* */
    char    unit[GRAFT_DS_UNIT_LEN + 1];
    uint8_t writable;
    uint8_t pinRef;
    uint8_t retain;
    /* Callback for host writes to writable datastreams */
    void (*onWrite)(GraftPayloadReader &payload, GraftContext &ctx);
};

/* ── Subscription Storage ─────────────────────────────── */

struct GraftSubscriptionEntry {
    uint8_t  pin;
    uint8_t  mode;      /* GRAFT_SUB_* */
    uint16_t intervalMs;
    uint16_t threshold;
    uint16_t lastValue;
    uint32_t lastReportMs;
    bool     active;
};

/* ── GraftDevice ──────────────────────────────────────── */

class GraftDevice {
public:
    /**
     * @param name      Firmware name (max 16 chars)
     * @param version   Firmware version string "major.minor.patch"
     * @param transport Reference to an initialized transport
     */
    GraftDevice(const char *name, const char *version, GraftTransport &transport);

    /** Register a module. Call before begin(). */
    void addModule(GraftModuleBase *module);

    /**
     * Declare a datastream. Call before begin().
     * @param name     Datastream name (max 16 chars)
     * @param type     GRAFT_TYPE_* constant
     * @param unit     Unit string (max 8 chars, "" if none)
     * @param writable true if host can write to this datastream
     */
    void addDatastream(const char *name, uint8_t type,
                       const char *unit, bool writable);

    /**
     * Set callback for host writes to a writable datastream.
     */
    void onDatastreamWrite(const char *name,
                           void (*callback)(GraftPayloadReader &payload, GraftContext &ctx));

    /** Initialize the device. Call once in setup() after all modules/datastreams are added. */
    void begin();

    /** Process one incoming packet. Call in loop(). Non-blocking. */
    void poll();

    /** Write a value to a datastream (device → host push). */
    void writeDatastream(const char *name, float value);
    void writeDatastream(const char *name, int32_t value);
    void writeDatastream(const char *name, bool value);
    void writeDatastream(const char *name, const uint8_t *data, size_t len);

    /** Send a raw packet. Used internally and by GraftContext. */
    void sendPacket(uint8_t type, uint8_t seq, const uint8_t *payload, size_t len);

    /** Send ACK for a given seq. */
    void sendAck(uint8_t seq);

    /** Send NAK for a given seq with error code. */
    void sendNak(uint8_t seq, uint8_t errorCode);

    /* ── Accessors ──── */
    const char* firmwareName() const { return _name; }
    uint8_t versionMajor() const { return _verMajor; }
    uint8_t versionMinor() const { return _verMinor; }
    uint8_t versionPatch() const { return _verPatch; }
    uint8_t moduleCount() const { return _moduleCount; }
    uint8_t datastreamCount() const { return _dsCount; }
    GraftTransport &transport() { return _transport; }

private:
    /* ── Config ──── */
    char _name[GRAFT_FIRMWARE_NAME_LEN + 1];
    uint8_t _verMajor, _verMinor, _verPatch;
    GraftTransport &_transport;

    /* ── Modules ──── */
    GraftModuleBase *_modules[GRAFT_MAX_MODULES];
    uint8_t _moduleCount;

    /* ── Datastreams ──── */
    GraftDatastreamEntry _datastreams[GRAFT_MAX_DATASTREAMS];
    uint8_t _dsCount;

    /* ── Subscriptions ──── */
    GraftSubscriptionEntry _subs[GRAFT_MAX_SUBSCRIPTIONS];
    uint8_t _subCount;

    /* ── Buffers ──── */
    uint8_t _rxBuf[GRAFT_PACKET_BUF_SIZE + GRAFT_HEADER_SIZE];
    size_t  _rxLen;
    uint8_t _txBuf[GRAFT_PACKET_BUF_SIZE + GRAFT_HEADER_SIZE];
    uint8_t _cobsBuf[GRAFT_COBS_BUF_SIZE];

    /* ── HELLO_RESP cache ──── */
    uint8_t _helloBuf[GRAFT_PACKET_BUF_SIZE];
    size_t  _helloLen;

    /* ── Internal methods ──── */
    void parseVersion(const char *version);
    void buildHelloResp();
    void dispatch(const GraftPacket &pkt);
    void handlePing(const GraftPacket &pkt);
    void handleHello(const GraftPacket &pkt);
    void handlePinMode(const GraftPacket &pkt);
    void handlePinWrite(const GraftPacket &pkt);
    void handlePinRead(const GraftPacket &pkt);
    void handlePinSubscribe(const GraftPacket &pkt);
    void handlePinUnsubscribe(const GraftPacket &pkt);
    void handleModCmd(const GraftPacket &pkt);
    void handleDsWrite(const GraftPacket &pkt);
    void handleDsRead(const GraftPacket &pkt);
    void handleDsSubscribe(const GraftPacket &pkt);
    void handleReset(const GraftPacket &pkt);
    void handleI2CWrite(const GraftPacket &pkt);
    void handleI2CRead(const GraftPacket &pkt);
    void handleI2CReadReg(const GraftPacket &pkt);
    void handleSPIXfer(const GraftPacket &pkt);
    void handleStreamStart(const GraftPacket &pkt);
    void handleStreamStop(const GraftPacket &pkt);
    void handleOTABegin(const GraftPacket &pkt);
    void handleOTAChunk(const GraftPacket &pkt);
    void handleOTAFinalize(const GraftPacket &pkt);
    void processStreaming();
    void processSubscriptions();
    int8_t findDatastream(const char *name);

    /* ── Streaming ──── */
    uint16_t _streamPinMask = 0;
    uint16_t _streamRateHz = 0;
    uint32_t _streamIntervalUs = 0;
    uint32_t _streamLastUs = 0;
    bool _streamActive = false;

    /* ── OTA ──── */
#ifdef GRAFT_OTA
    GraftOTA _ota;
#endif

    /* COBS framing helpers */
    void sendFramed(const uint8_t *raw, size_t rawLen);
    bool receiveFramed();
};

#endif /* GRAFT_DEVICE_H */
