/**
 * GRAFT BLE Transport — ESP32 / nRF52
 *
 * BLE GATT server with TX (notify) and RX (write) characteristics.
 * MTU-aware chunking for packets larger than (MTU-3).
 * COBS framing for packet boundaries.
 */

#ifndef GRAFT_BLE_H
#define GRAFT_BLE_H

#include "GraftTransport.h"

/* ── ESP32 BLE ─────────────────────────────────── */

#if defined(GRAFT_PLATFORM_ESP32) || defined(ESP32)

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define GRAFT_BLE_SERVICE_UUID     "0000gf01-0000-1000-8000-00805f9b34fb"
#define GRAFT_BLE_TX_CHAR_UUID     "0000gf02-0000-1000-8000-00805f9b34fb"
#define GRAFT_BLE_RX_CHAR_UUID     "0000gf03-0000-1000-8000-00805f9b34fb"

#ifndef GRAFT_BLE_RX_BUF_SIZE
#define GRAFT_BLE_RX_BUF_SIZE 512
#endif

class GraftBLE : public GraftTransport, public BLEServerCallbacks, public BLECharacteristicCallbacks {
public:
    GraftBLE(const char *deviceName = "GRAFT")
        : _deviceName(deviceName), _connected(false),
          _rxLen(0), _hasData(false), _mtu(23) {}

    bool begin() override {
        BLEDevice::init(_deviceName);
        BLEDevice::setMTU(517); // Request high MTU

        _server = BLEDevice::createServer();
        _server->setCallbacks(this);

        BLEService *service = _server->createService(GRAFT_BLE_SERVICE_UUID);

        // TX: device → host (notify)
        _txChar = service->createCharacteristic(
            GRAFT_BLE_TX_CHAR_UUID,
            BLECharacteristic::PROPERTY_NOTIFY
        );
        _txChar->addDescriptor(new BLE2902());

        // RX: host → device (write)
        _rxChar = service->createCharacteristic(
            GRAFT_BLE_RX_CHAR_UUID,
            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
        );
        _rxChar->setCallbacks(this);

        service->start();

        BLEAdvertising *adv = BLEDevice::getAdvertising();
        adv->addServiceUUID(GRAFT_BLE_SERVICE_UUID);
        adv->setScanResponse(true);
        adv->setMinPreferred(0x06);
        BLEDevice::startAdvertising();

        return true;
    }

    bool connected() override { return _connected; }

    size_t write(const uint8_t *buf, size_t len) override {
        if (!_connected || !_txChar) return 0;

        // MTU-aware chunking
        size_t chunkSize = _mtu - 3;
        if (chunkSize < 20) chunkSize = 20;

        size_t sent = 0;
        while (sent < len) {
            size_t n = (len - sent > chunkSize) ? chunkSize : (len - sent);
            _txChar->setValue((uint8_t *)(buf + sent), n);
            _txChar->notify();
            sent += n;
            if (sent < len) delay(1); // Small gap between chunks
        }
        return sent;
    }

    int available() override {
        return _hasData ? (int)_rxLen : 0;
    }

    int read(uint8_t *buf, size_t maxLen) override {
        if (!_hasData) return 0;
        size_t n = (_rxLen < maxLen) ? _rxLen : maxLen;
        memcpy(buf, _rxBuf, n);
        _hasData = false;
        _rxLen = 0;
        return (int)n;
    }

    void flush() override {}
    bool needsCOBS() override { return true; }

    // BLEServerCallbacks
    void onConnect(BLEServer *server) override {
        _connected = true;
        _mtu = BLEDevice::getMTU();
    }

    void onDisconnect(BLEServer *server) override {
        _connected = false;
        // Restart advertising
        BLEDevice::startAdvertising();
    }

    // BLECharacteristicCallbacks (RX)
    void onWrite(BLECharacteristic *characteristic) override {
        uint8_t *data = characteristic->getData();
        size_t len = characteristic->getLength();
        if (len > 0 && _rxLen + len <= GRAFT_BLE_RX_BUF_SIZE) {
            memcpy(_rxBuf + _rxLen, data, len);
            _rxLen += len;
            _hasData = true;
        }
    }

private:
    const char *_deviceName;
    BLEServer *_server = nullptr;
    BLECharacteristic *_txChar = nullptr;
    BLECharacteristic *_rxChar = nullptr;
    bool _connected;
    uint16_t _mtu;

    uint8_t _rxBuf[GRAFT_BLE_RX_BUF_SIZE];
    size_t _rxLen;
    bool _hasData;
};

#endif /* ESP32 */

/* ── nRF52 BLE (stub — uses Adafruit_nRF52_Arduino) ──── */

#if defined(GRAFT_PLATFORM_NRF52)
// TODO: Implement using bluefruit API
// BLEService, BLECharacteristic, BLEDis
#endif

#endif /* GRAFT_BLE_H */
