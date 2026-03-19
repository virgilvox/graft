/**
 * GRAFT Device — Implementation
 */

#include "GraftDevice.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
/* Desktop builds: provide millis() stub */
#include <time.h>
static uint32_t millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif

/* ── Constructor ──────────────────────────────────────── */

GraftDevice::GraftDevice(const char *name, const char *version, GraftTransport &transport)
    : _transport(transport), _moduleCount(0), _dsCount(0), _subCount(0),
      _rxLen(0), _helloLen(0)
{
    memset(_name, 0, sizeof(_name));
    strncpy(_name, name, GRAFT_FIRMWARE_NAME_LEN);
    parseVersion(version);
    memset(_modules, 0, sizeof(_modules));
    memset(_datastreams, 0, sizeof(_datastreams));
    memset(_subs, 0, sizeof(_subs));
}

void GraftDevice::parseVersion(const char *version) {
    _verMajor = 0; _verMinor = 0; _verPatch = 0;
    if (!version) return;
    int parts[3] = {0, 0, 0};
    int idx = 0;
    for (const char *p = version; *p && idx < 3; p++) {
        if (*p == '.') { idx++; }
        else if (*p >= '0' && *p <= '9') {
            parts[idx] = parts[idx] * 10 + (*p - '0');
        }
    }
    _verMajor = (uint8_t)parts[0];
    _verMinor = (uint8_t)parts[1];
    _verPatch = (uint8_t)parts[2];
}

/* ── Registration ─────────────────────────────────────── */

void GraftDevice::addModule(GraftModuleBase *module) {
    if (_moduleCount < GRAFT_MAX_MODULES && module) {
        _modules[_moduleCount++] = module;
    }
}

void GraftDevice::addDatastream(const char *name, uint8_t type,
                                 const char *unit, bool writable)
{
    if (_dsCount >= GRAFT_MAX_DATASTREAMS) return;
    GraftDatastreamEntry &ds = _datastreams[_dsCount];
    memset(&ds, 0, sizeof(ds));
    strncpy(ds.name, name, GRAFT_DS_NAME_LEN);
    ds.type = type;
    if (unit) strncpy(ds.unit, unit, GRAFT_DS_UNIT_LEN);
    ds.writable = writable ? 1 : 0;
    ds.pinRef = 0xFF;
    ds.retain = writable ? 1 : 0;  /* writable datastreams retained by default */
    ds.onWrite = nullptr;
    _dsCount++;
}

void GraftDevice::onDatastreamWrite(const char *name,
                                     void (*callback)(GraftPayloadReader &, GraftContext &))
{
    int8_t idx = findDatastream(name);
    if (idx >= 0) {
        _datastreams[idx].onWrite = callback;
    }
}

int8_t GraftDevice::findDatastream(const char *name) {
    for (uint8_t i = 0; i < _dsCount; i++) {
        if (strncmp(_datastreams[i].name, name, GRAFT_DS_NAME_LEN) == 0) {
            return (int8_t)i;
        }
    }
    return -1;
}

/* ── Lifecycle ────────────────────────────────────────── */

void GraftDevice::begin() {
    _transport.begin();

    /* Initialize modules */
    for (uint8_t i = 0; i < _moduleCount; i++) {
        if (_modules[i]) {
            _modules[i]->setDevice(this);
            _modules[i]->begin();
        }
    }

    /* Pre-build HELLO_RESP payload */
    buildHelloResp();
}

/* ── Poll Loop ────────────────────────────────────────── */

void GraftDevice::poll() {
    if (!_transport.connected()) return;

    _transport.poll();

    /* Try to receive one packet */
    if (_transport.needsCOBS()) {
        if (receiveFramed()) {
            GraftPacket pkt;
            GraftResult res = graft_wire_decode(&pkt, _rxBuf, _rxLen);
            if (res == GRAFT_OK) {
                dispatch(pkt);
            } else if (res == GRAFT_ERR_INVALID_CRC) {
                /* Send NAK with CRC error (seq 0 since we can't trust the seq) */
                sendNak(0, GRAFT_ERR_CRC_MISMATCH);
            }
            _rxLen = 0;
        }
    } else {
        /* Non-COBS transport: read directly */
        int avail = _transport.available();
        if (avail > 0) {
            size_t maxRead = sizeof(_rxBuf) - _rxLen;
            if ((size_t)avail > maxRead) avail = (int)maxRead;
            int n = _transport.read(_rxBuf + _rxLen, avail);
            if (n > 0) _rxLen += n;

            /* Try to decode */
            if (_rxLen >= GRAFT_HEADER_SIZE) {
                GraftPacket pkt;
                GraftResult res = graft_wire_decode(&pkt, _rxBuf, _rxLen);
                if (res == GRAFT_OK) {
                    dispatch(pkt);
                    /* Shift buffer */
                    size_t consumed = graft_wire_packet_size(pkt.payload_len);
                    if (consumed < _rxLen) {
                        memmove(_rxBuf, _rxBuf + consumed, _rxLen - consumed);
                        _rxLen -= consumed;
                    } else {
                        _rxLen = 0;
                    }
                } else if (res == GRAFT_ERR_INVALID_CRC) {
                    sendNak(0, GRAFT_ERR_CRC_MISMATCH);
                    _rxLen = 0;
                }
            }
        }
    }

    /* Process streaming data */
    if (_streamActive) {
        processStreaming();
    }

    /* Process active subscriptions */
    processSubscriptions();

    /* Poll modules */
    for (uint8_t i = 0; i < _moduleCount; i++) {
        if (_modules[i]) _modules[i]->poll();
    }
}

/* ── COBS Framing ─────────────────────────────────────── */

bool GraftDevice::receiveFramed() {
    while (_transport.available()) {
        uint8_t b;
        if (_transport.read(&b, 1) != 1) break;

        if (b == 0x00) {
            /* Delimiter — decode what we have */
            if (_rxLen > 0) {
                uint8_t decoded[GRAFT_PACKET_BUF_SIZE + GRAFT_HEADER_SIZE];
                size_t decLen = graft_cobs_decode(decoded, sizeof(decoded),
                                                   _rxBuf, _rxLen);
                if (decLen > 0) {
                    memcpy(_rxBuf, decoded, decLen);
                    _rxLen = decLen;
                    return true;
                }
                _rxLen = 0; /* decode failed, discard */
            }
        } else {
            if (_rxLen < sizeof(_rxBuf)) {
                _rxBuf[_rxLen++] = b;
            } else {
                _rxLen = 0; /* overflow, discard */
            }
        }
    }
    return false;
}

void GraftDevice::sendFramed(const uint8_t *raw, size_t rawLen) {
    if (_transport.needsCOBS()) {
        size_t cobsLen = graft_cobs_encode(_cobsBuf, sizeof(_cobsBuf), raw, rawLen);
        if (cobsLen > 0) {
            _transport.write(_cobsBuf, cobsLen);
            /* Send 0x00 delimiter */
            uint8_t delim = 0x00;
            _transport.write(&delim, 1);
            _transport.flush();
        }
    } else {
        _transport.write(raw, rawLen);
        _transport.flush();
    }
}

/* ── Packet Sending ───────────────────────────────────── */

void GraftDevice::sendPacket(uint8_t type, uint8_t seq,
                              const uint8_t *payload, size_t len)
{
    GraftPacket pkt;
    pkt.version = GRAFT_PROTOCOL_VERSION;
    pkt.type = type;
    pkt.seq = seq;
    pkt.payload_len = (uint16_t)len;
    pkt.payload = (uint8_t *)payload;

    size_t wireLen = graft_wire_encode(_txBuf, sizeof(_txBuf), &pkt);
    if (wireLen > 0) {
        sendFramed(_txBuf, wireLen);
    }
}

void GraftDevice::sendAck(uint8_t seq) {
    sendPacket(GRAFT_EVT_ACK, seq, nullptr, 0);
}

void GraftDevice::sendNak(uint8_t seq, uint8_t errorCode) {
    sendPacket(GRAFT_EVT_NAK, seq, &errorCode, 1);
}

/* ── Dispatch ─────────────────────────────────────────── */

void GraftDevice::dispatch(const GraftPacket &pkt) {
    switch (pkt.type) {
        case GRAFT_CMD_PING:            handlePing(pkt); break;
        case GRAFT_CMD_HELLO:           handleHello(pkt); break;
        case GRAFT_CMD_PIN_MODE:        handlePinMode(pkt); break;
        case GRAFT_CMD_PIN_WRITE:       handlePinWrite(pkt); break;
        case GRAFT_CMD_PIN_READ:        handlePinRead(pkt); break;
        case GRAFT_CMD_PIN_SUBSCRIBE:   handlePinSubscribe(pkt); break;
        case GRAFT_CMD_PIN_UNSUBSCRIBE: handlePinUnsubscribe(pkt); break;
        case GRAFT_CMD_MOD_CMD:         handleModCmd(pkt); break;
        case GRAFT_CMD_DS_WRITE:        handleDsWrite(pkt); break;
        case GRAFT_CMD_DS_READ:         handleDsRead(pkt); break;
        case GRAFT_CMD_DS_SUBSCRIBE:    handleDsSubscribe(pkt); break;
        case GRAFT_CMD_RESET:           handleReset(pkt); break;
        case GRAFT_CMD_I2C_WRITE:       handleI2CWrite(pkt); break;
        case GRAFT_CMD_I2C_READ:        handleI2CRead(pkt); break;
        case GRAFT_CMD_I2C_READ_REG:    handleI2CReadReg(pkt); break;
        case GRAFT_CMD_SPI_XFER:        handleSPIXfer(pkt); break;
        case GRAFT_CMD_STREAM_START:    handleStreamStart(pkt); break;
        case GRAFT_CMD_STREAM_STOP:     handleStreamStop(pkt); break;
        case GRAFT_CMD_OTA_BEGIN:       handleOTABegin(pkt); break;
        case GRAFT_CMD_OTA_CHUNK:       handleOTAChunk(pkt); break;
        case GRAFT_CMD_OTA_FINALIZE:    handleOTAFinalize(pkt); break;
        default:
            sendNak(pkt.seq, GRAFT_ERR_UNKNOWN_TYPE);
            break;
    }
}

/* ── Command Handlers ─────────────────────────────────── */

void GraftDevice::handlePing(const GraftPacket &pkt) {
    sendPacket(GRAFT_EVT_PONG, pkt.seq, nullptr, 0);
}

void GraftDevice::handleHello(const GraftPacket &pkt) {
    sendPacket(GRAFT_EVT_HELLO_RESP, pkt.seq, _helloBuf, _helloLen);
}

void GraftDevice::handlePinMode(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t pin = r.readUInt8();
    uint8_t mode = r.readUInt8();

    switch (mode) {
        case GRAFT_PIN_MODE_INPUT:        pinMode(pin, INPUT); break;
        case GRAFT_PIN_MODE_OUTPUT:       pinMode(pin, OUTPUT); break;
        case GRAFT_PIN_MODE_INPUT_PULLUP: pinMode(pin, INPUT_PULLUP); break;
        case GRAFT_PIN_MODE_PWM:          pinMode(pin, OUTPUT); break;
        case GRAFT_PIN_MODE_ANALOG:       /* No pinMode needed for analog input */ break;
        default:
            sendNak(pkt.seq, GRAFT_ERR_PIN_MODE_UNSUPPORTED);
            return;
    }
    sendAck(pkt.seq);
#else
    sendAck(pkt.seq);
#endif
}

void GraftDevice::handlePinWrite(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t pin = r.readUInt8();
    uint8_t value = r.readUInt8();

    /* If payload has a 3rd byte with mode hint, use it */
    if (pkt.payload_len >= 3) {
        uint8_t mode = r.readUInt8();
        if (mode == GRAFT_PIN_MODE_PWM) {
            analogWrite(pin, value);
        } else {
            digitalWrite(pin, value ? HIGH : LOW);
        }
    } else {
        /* Default: digital write for 0/1, PWM for other values */
        if (value <= 1) {
            digitalWrite(pin, value ? HIGH : LOW);
        } else {
            analogWrite(pin, value);
        }
    }
    sendAck(pkt.seq);
#else
    sendAck(pkt.seq);
#endif
}

void GraftDevice::handlePinRead(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 1) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t pin = r.readUInt8();
    uint8_t mode = (pkt.payload_len >= 2) ? r.readUInt8() : GRAFT_PIN_MODE_INPUT;

    uint8_t respBuf[4];
    GraftPayloadWriter w(respBuf, sizeof(respBuf));
    w.writeUInt8(pin);

    if (mode == GRAFT_PIN_MODE_ANALOG) {
        w.writeUInt16((uint16_t)analogRead(pin));
    } else {
        w.writeUInt16(digitalRead(pin) ? 1 : 0);
    }

    sendPacket(GRAFT_EVT_PIN_READ_RESP, pkt.seq, respBuf, w.length());
#else
    uint8_t respBuf[3] = {0, 0, 0};
    sendPacket(GRAFT_EVT_PIN_READ_RESP, pkt.seq, respBuf, 3);
#endif
}

void GraftDevice::handlePinSubscribe(const GraftPacket &pkt) {
    if (pkt.payload_len < 5) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }

    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t pin = r.readUInt8();
    uint8_t mode = r.readUInt8();
    uint16_t intervalMs = r.readUInt16();
    uint16_t threshold = (pkt.payload_len >= 7) ? r.readUInt16() : 0;

    /* Check for existing subscription on this pin and update in place */
    for (uint8_t i = 0; i < _subCount; i++) {
        if (_subs[i].pin == pin && _subs[i].active) {
            _subs[i].mode = mode;
            _subs[i].intervalMs = intervalMs ? intervalMs : 100;
            _subs[i].threshold = threshold;
            _subs[i].lastValue = 0;
            _subs[i].lastReportMs = millis();
            sendAck(pkt.seq);
            return;
        }
    }

    if (_subCount >= GRAFT_MAX_SUBSCRIPTIONS) {
        sendNak(pkt.seq, GRAFT_ERR_SUB_LIMIT_REACHED);
        return;
    }

    GraftSubscriptionEntry &sub = _subs[_subCount];
    sub.pin = pin;
    sub.mode = mode;
    sub.intervalMs = intervalMs ? intervalMs : 100;
    sub.threshold = threshold;
    sub.lastValue = 0;
    sub.lastReportMs = millis();
    sub.active = true;
    _subCount++;

    sendAck(pkt.seq);
}

void GraftDevice::handlePinUnsubscribe(const GraftPacket &pkt) {
    if (pkt.payload_len < 1) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }
    uint8_t pin = pkt.payload[0];

    for (uint8_t i = 0; i < _subCount; i++) {
        if (_subs[i].pin == pin && _subs[i].active) {
            _subs[i].active = false;
            sendAck(pkt.seq);
            return;
        }
    }
    sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN);
}

void GraftDevice::handleModCmd(const GraftPacket &pkt) {
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_MODULE_NOT_LOADED); return; }

    uint8_t moduleId = pkt.payload[0];
    uint8_t cmd = pkt.payload[1];

    if (moduleId >= _moduleCount || !_modules[moduleId]) {
        sendNak(pkt.seq, GRAFT_ERR_MODULE_NOT_LOADED);
        return;
    }

    GraftPayloadReader payload(pkt.payload + 2, pkt.payload_len - 2);
    GraftContext ctx(*this, pkt.seq);
    _modules[moduleId]->handle(cmd, payload, ctx);

    /* Auto-ack if module didn't respond */
    if (!ctx.responded()) {
        sendAck(pkt.seq);
    }
}

void GraftDevice::handleDsWrite(const GraftPacket &pkt) {
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_UNKNOWN_DATASTREAM); return; }

    uint8_t dsIndex = pkt.payload[0];
    if (dsIndex >= _dsCount) {
        sendNak(pkt.seq, GRAFT_ERR_UNKNOWN_DATASTREAM);
        return;
    }

    GraftDatastreamEntry &ds = _datastreams[dsIndex];
    if (!ds.writable) {
        sendNak(pkt.seq, GRAFT_ERR_DATASTREAM_READONLY);
        return;
    }

    GraftPayloadReader payload(pkt.payload + 1, pkt.payload_len - 1);
    GraftContext ctx(*this, pkt.seq);

    if (ds.onWrite) {
        ds.onWrite(payload, ctx);
    }

    if (!ctx.responded()) {
        sendAck(pkt.seq);
    }
}

void GraftDevice::handleDsRead(const GraftPacket &pkt) {
    /* For now, NAK — full implementation requires storing last values */
    sendNak(pkt.seq, GRAFT_ERR_UNKNOWN_DATASTREAM);
}

void GraftDevice::handleDsSubscribe(const GraftPacket &pkt) {
    /* Datastream subscriptions are handled similarly to pin subscriptions */
    sendAck(pkt.seq);
}

void GraftDevice::handleReset(const GraftPacket &pkt) {
    /* Clear all subscriptions */
    for (uint8_t i = 0; i < _subCount; i++) {
        _subs[i].active = false;
    }
    _subCount = 0;
    _rxLen = 0;
    sendAck(pkt.seq);
}

/* ── I2C Handlers ─────────────────────────────────────── */

void GraftDevice::handleI2CWrite(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t addr = r.readUInt8();
    if (addr < 0x08 || addr > 0x77) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    size_t dataLen = pkt.payload_len - 1;

    Wire.beginTransmission(addr);
    for (size_t i = 0; i < dataLen; i++) {
        Wire.write(r.readUInt8());
    }
    uint8_t result = Wire.endTransmission();
    if (result == 0) {
        sendAck(pkt.seq);
    } else {
        sendNak(pkt.seq, GRAFT_ERR_I2C_NACK);
    }
#else
    sendAck(pkt.seq);
#endif
}

void GraftDevice::handleI2CRead(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t addr = r.readUInt8();
    if (addr < 0x08 || addr > 0x77) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    uint8_t count = r.readUInt8();

    uint8_t respBuf[GRAFT_PACKET_BUF_SIZE];
    GraftPayloadWriter w(respBuf, sizeof(respBuf));

    uint8_t received = Wire.requestFrom(addr, count);
    for (uint8_t i = 0; i < received; i++) {
        w.writeUInt8(Wire.read());
    }
    sendPacket(GRAFT_EVT_I2C_READ_RESP, pkt.seq, respBuf, w.length());
#else
    sendPacket(GRAFT_EVT_I2C_READ_RESP, pkt.seq, nullptr, 0);
#endif
}

void GraftDevice::handleI2CReadReg(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 3) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t addr = r.readUInt8();
    if (addr < 0x08 || addr > 0x77) { sendNak(pkt.seq, GRAFT_ERR_I2C_NOT_AVAILABLE); return; }
    uint8_t reg = r.readUInt8();
    uint8_t count = r.readUInt8();

    Wire.beginTransmission(addr);
    Wire.write(reg);
    uint8_t result = Wire.endTransmission(false);
    if (result != 0) {
        sendNak(pkt.seq, GRAFT_ERR_I2C_NACK);
        return;
    }

    uint8_t respBuf[GRAFT_PACKET_BUF_SIZE];
    GraftPayloadWriter w(respBuf, sizeof(respBuf));

    uint8_t received = Wire.requestFrom(addr, count);
    for (uint8_t i = 0; i < received; i++) {
        w.writeUInt8(Wire.read());
    }
    sendPacket(GRAFT_EVT_I2C_READ_RESP, pkt.seq, respBuf, w.length());
#else
    sendPacket(GRAFT_EVT_I2C_READ_RESP, pkt.seq, nullptr, 0);
#endif
}

/* ── SPI Handler ──────────────────────────────────────── */

void GraftDevice::handleSPIXfer(const GraftPacket &pkt) {
#ifdef ARDUINO
    if (pkt.payload_len < 2) { sendNak(pkt.seq, GRAFT_ERR_UNKNOWN_TYPE); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    uint8_t csPin = r.readUInt8();
    size_t dataLen = pkt.payload_len - 1;

    uint8_t respBuf[GRAFT_PACKET_BUF_SIZE];
    GraftPayloadWriter w(respBuf, sizeof(respBuf));

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(csPin, LOW);
    for (size_t i = 0; i < dataLen; i++) {
        w.writeUInt8(SPI.transfer(r.readUInt8()));
    }
    digitalWrite(csPin, HIGH);
    SPI.endTransaction();

    sendPacket(GRAFT_EVT_SPI_XFER_RESP, pkt.seq, respBuf, w.length());
#else
    sendPacket(GRAFT_EVT_SPI_XFER_RESP, pkt.seq, nullptr, 0);
#endif
}

/* ── Stream Handlers ──────────────────────────────────── */

void GraftDevice::handleStreamStart(const GraftPacket &pkt) {
    if (pkt.payload_len < 5) { sendNak(pkt.seq, GRAFT_ERR_INVALID_PIN); return; }
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    _streamPinMask = r.readUInt16();
    _streamRateHz = r.readUInt16();
    _streamActive = true;
    _streamIntervalUs = (_streamRateHz > 0) ? (1000000UL / _streamRateHz) : 10000;
    _streamLastUs = 0;
    sendAck(pkt.seq);
}

void GraftDevice::handleStreamStop(const GraftPacket &pkt) {
    _streamActive = false;
    _streamPinMask = 0;
    sendAck(pkt.seq);
}

/* ── OTA Handlers ─────────────────────────────────────── */

void GraftDevice::handleOTABegin(const GraftPacket &pkt) {
#ifdef GRAFT_OTA
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    GraftContext ctx(*this, pkt.seq);
    _ota.handleBegin(r, ctx);
#else
    sendNak(pkt.seq, GRAFT_ERR_OTA_INVALID);
#endif
}

void GraftDevice::handleOTAChunk(const GraftPacket &pkt) {
#ifdef GRAFT_OTA
    GraftPayloadReader r(pkt.payload, pkt.payload_len);
    GraftContext ctx(*this, pkt.seq);
    _ota.handleChunk(r, ctx);
#else
    sendNak(pkt.seq, GRAFT_ERR_OTA_INVALID);
#endif
}

void GraftDevice::handleOTAFinalize(const GraftPacket &pkt) {
#ifdef GRAFT_OTA
    GraftContext ctx(*this, pkt.seq);
    _ota.handleFinalize(ctx);
#else
    sendNak(pkt.seq, GRAFT_ERR_OTA_INVALID);
#endif
}

/* ── Streaming ────────────────────────────────────────── */

void GraftDevice::processStreaming() {
#ifdef ARDUINO
    uint32_t now = micros();
    if (now - _streamLastUs < _streamIntervalUs) return;
    _streamLastUs = now;

    uint8_t buf[GRAFT_PACKET_BUF_SIZE];
    GraftPayloadWriter w(buf, sizeof(buf));

    for (uint8_t p = 0; p < 16; p++) {
        if (_streamPinMask & (1 << p)) {
            w.writeUInt16((uint16_t)analogRead(p));
        }
    }
    if (w.length() > 0) {
        sendPacket(GRAFT_EVT_STREAM_DATA, 0, buf, w.length());
    }
#endif
}

/* ── Subscriptions ────────────────────────────────────── */

void GraftDevice::processSubscriptions() {
#ifdef ARDUINO
    uint32_t now = millis();

    for (uint8_t i = 0; i < _subCount; i++) {
        GraftSubscriptionEntry &sub = _subs[i];
        if (!sub.active) continue;

        if (sub.mode == GRAFT_SUB_ANALOG_POLL) {
            if (now - sub.lastReportMs >= sub.intervalMs) {
                uint16_t value = (uint16_t)analogRead(sub.pin);
                uint16_t delta = (value > sub.lastValue)
                    ? (value - sub.lastValue)
                    : (sub.lastValue - value);

                if (delta >= sub.threshold) {
                    uint8_t buf[3];
                    GraftPayloadWriter w(buf, sizeof(buf));
                    w.writeUInt8(sub.pin);
                    w.writeUInt16(value);
                    sendPacket(GRAFT_EVT_PIN_EVENT, 0, buf, w.length());
                    sub.lastValue = value;
                }
                sub.lastReportMs = now;
            }
        }
        /* Digital interrupt modes are handled via ISR (future step) */
    }
#endif
}

/* ── Datastream Writes (device → host) ────────────────── */

void GraftDevice::writeDatastream(const char *name, float value) {
    int8_t idx = findDatastream(name);
    if (idx < 0) return;
    uint8_t buf[5];
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8((uint8_t)idx);
    w.writeFloat32(value);
    sendPacket(GRAFT_EVT_DS_EVENT, 0, buf, w.length());
}

void GraftDevice::writeDatastream(const char *name, int32_t value) {
    int8_t idx = findDatastream(name);
    if (idx < 0) return;
    uint8_t buf[5];
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8((uint8_t)idx);
    w.writeInt32(value);
    sendPacket(GRAFT_EVT_DS_EVENT, 0, buf, w.length());
}

void GraftDevice::writeDatastream(const char *name, bool value) {
    int8_t idx = findDatastream(name);
    if (idx < 0) return;
    uint8_t buf[2];
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8((uint8_t)idx);
    w.writeBool(value);
    sendPacket(GRAFT_EVT_DS_EVENT, 0, buf, w.length());
}

void GraftDevice::writeDatastream(const char *name, const uint8_t *data, size_t len) {
    int8_t idx = findDatastream(name);
    if (idx < 0) return;
    uint8_t buf[GRAFT_PACKET_BUF_SIZE];
    if (1 + len > sizeof(buf)) return;
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8((uint8_t)idx);
    w.writeBytes(data, len);
    sendPacket(GRAFT_EVT_DS_EVENT, 0, buf, w.length());
}

/* ── HELLO_RESP Builder ───────────────────────────────── */

void GraftDevice::buildHelloResp() {
    GraftPayloadWriter w(_helloBuf, sizeof(_helloBuf));

    /* firmware_name: 16 bytes null-padded */
    w.writeString(_name, GRAFT_FIRMWARE_NAME_LEN);

    /* firmware_version: 3 bytes */
    w.writeUInt8(_verMajor);
    w.writeUInt8(_verMinor);
    w.writeUInt8(_verPatch);

    /* mcu_id: 8 bytes (zeros for now — platform-specific) */
    uint8_t mcuId[GRAFT_MCU_ID_LEN] = {0};
    w.writeBytes(mcuId, GRAFT_MCU_ID_LEN);

    /* ota_capable: 1 byte */
    w.writeUInt8(0x00); /* TODO: detect OTA capability */

    /* pin_count + per-pin capability bitmask */
#ifdef ARDUINO
    #ifdef NUM_DIGITAL_PINS
        uint8_t pinCount = NUM_DIGITAL_PINS;
    #else
        uint8_t pinCount = 20;
    #endif
#else
    uint8_t pinCount = 20; /* desktop stub */
#endif
    w.writeUInt8(pinCount);

    for (uint8_t p = 0; p < pinCount; p++) {
        uint8_t caps = GRAFT_PIN_CAP_DIGITAL_IN | GRAFT_PIN_CAP_DIGITAL_OUT;
        /* Mark analog-capable pins */
#ifdef ARDUINO
    #ifdef NUM_ANALOG_INPUTS
        #ifdef A0
            if (p >= A0 && p < A0 + NUM_ANALOG_INPUTS) {
                caps |= GRAFT_PIN_CAP_ANALOG_IN;
            }
        #endif
    #endif
    /* Mark PWM-capable pins */
    #if defined(digitalPinHasPWM)
            if (digitalPinHasPWM(p)) caps |= GRAFT_PIN_CAP_PWM_OUT;
    #endif
    /* Mark interrupt-capable pins */
    #if defined(digitalPinToInterrupt)
            if (digitalPinToInterrupt(p) != NOT_AN_INTERRUPT) caps |= GRAFT_PIN_CAP_INTERRUPT;
    #endif
#endif
        w.writeUInt8(caps);
    }

    /* i2c_buses */
#ifdef WIRE_INTERFACES_COUNT
    w.writeUInt8(WIRE_INTERFACES_COUNT);
#else
    w.writeUInt8(1); /* most boards have at least one I2C bus */
#endif

    /* spi_buses */
    w.writeUInt8(1);

    /* uart_count */
#ifdef SERIAL_PORT_HARDWARE_OPEN
    w.writeUInt8(2);
#else
    w.writeUInt8(1);
#endif

    /* max_payload: uint16 */
    w.writeUInt16(GRAFT_PACKET_BUF_SIZE);

    /* module_count + per-module descriptors */
    w.writeUInt8(_moduleCount);
    for (uint8_t i = 0; i < _moduleCount; i++) {
        GraftModuleBase *mod = _modules[i];
        if (!mod) continue;

        w.writeUInt8(i); /* module_id */
        w.writeString(mod->name(), GRAFT_MODULE_NAME_LEN);
        w.writeUInt8(mod->versionMajor());
        w.writeUInt8(mod->versionMinor());
        w.writeUInt8(mod->pinCount());
        const uint8_t *modPins = mod->pins();
        for (uint8_t p = 0; p < mod->pinCount(); p++) {
            w.writeUInt8(modPins ? modPins[p] : 0);
        }
    }

    /* datastream_count + per-datastream descriptors */
    w.writeUInt8(_dsCount);
    for (uint8_t i = 0; i < _dsCount; i++) {
        GraftDatastreamEntry &ds = _datastreams[i];
        w.writeString(ds.name, GRAFT_DS_NAME_LEN);
        w.writeUInt8(ds.type);
        w.writeString(ds.unit, GRAFT_DS_UNIT_LEN);
        w.writeUInt8(ds.writable);
        w.writeUInt8(ds.pinRef);
        w.writeUInt8(ds.retain);
    }

    _helloLen = w.length();
}

/* ── GraftContext Implementation ──────────────────────── */

void GraftContext::ack() {
    if (!_responded) {
        _device.sendAck(_seq);
        _responded = true;
    }
}

void GraftContext::nak(uint8_t errorCode) {
    if (!_responded) {
        _device.sendNak(_seq, errorCode);
        _responded = true;
    }
}

void GraftContext::emitModEvent(uint8_t moduleId, uint8_t eventCode,
                                const uint8_t *data, size_t len)
{
    uint8_t buf[GRAFT_PACKET_BUF_SIZE];
    if (2 + len > sizeof(buf)) return;
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8(moduleId);
    w.writeUInt8(eventCode);
    if (data && len > 0) w.writeBytes(data, len);
    _device.sendPacket(GRAFT_EVT_MOD_EVENT, 0, buf, w.length());
}

void GraftContext::emitDsEvent(uint8_t dsIndex, const uint8_t *data, size_t len) {
    uint8_t buf[GRAFT_PACKET_BUF_SIZE];
    if (1 + len > sizeof(buf)) return;
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8(dsIndex);
    if (data && len > 0) w.writeBytes(data, len);
    _device.sendPacket(GRAFT_EVT_DS_EVENT, 0, buf, w.length());
}

void GraftContext::sendModResp(uint8_t moduleId, const uint8_t *data, size_t len) {
    uint8_t buf[GRAFT_PACKET_BUF_SIZE];
    if (1 + len > sizeof(buf)) return;
    GraftPayloadWriter w(buf, sizeof(buf));
    w.writeUInt8(moduleId);
    if (data && len > 0) w.writeBytes(data, len);
    _device.sendPacket(GRAFT_EVT_MOD_RESP, _seq, buf, w.length());
    _responded = true;
}
