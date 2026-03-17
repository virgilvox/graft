/**
 * GRAFT MQTT Transport
 *
 * Publishes/subscribes to graft/{deviceId}/ topic tree.
 * Accepts any Arduino Client& (WiFiClient, EthernetClient, GSMClient, etc.)
 *
 * Topic schema:
 *   graft/{deviceId}/cmd/{typeHex}    ← host publishes commands
 *   graft/{deviceId}/evt/{typeHex}    ← device publishes events
 *   graft/{deviceId}/hello            ← retained HELLO_RESP binary
 *   graft/{deviceId}/status           ← "online"/"offline" (LWT)
 *   graft/{deviceId}/meta             ← JSON capability summary
 *   graft/{deviceId}/ds/{name}/cmd    ← host writes to datastream
 *   graft/{deviceId}/ds/{name}/evt    ← device pushes datastream values
 *
 * Compile-time: #define GRAFT_TRANSPORT_MQTT
 * Requires: PubSubClient library
 */

#ifndef GRAFT_MQTT_H
#define GRAFT_MQTT_H

#ifdef GRAFT_TRANSPORT_MQTT

#include "GraftTransport.h"
#include <PubSubClient.h>

#ifndef GRAFT_MQTT_MAX_TOPIC
#define GRAFT_MQTT_MAX_TOPIC 96
#endif

class GraftMQTT : public GraftTransport {
public:
    /**
     * @param client     Arduino Client& (WiFiClient, EthernetClient, etc.)
     * @param broker     MQTT broker hostname/IP
     * @param port       MQTT broker port (default 1883)
     * @param deviceId   Unique device identifier for topic routing
     */
    GraftMQTT(Client &client, const char *broker, uint16_t port,
              const char *deviceId)
        : _mqtt(client), _broker(broker), _port(port),
          _rxLen(0), _hasData(false)
    {
        strncpy(_deviceId, deviceId, sizeof(_deviceId) - 1);
        _deviceId[sizeof(_deviceId) - 1] = '\0';
    }

    bool begin() override {
        _mqtt.setServer(_broker, _port);
        _mqtt.setBufferSize(1024); // Ensure enough for HELLO_RESP

        // Set callback
        _mqtt.setCallback([this](char *topic, uint8_t *payload, unsigned int length) {
            _onMessage(topic, payload, length);
        });

        return _reconnect();
    }

    bool connected() override {
        return _mqtt.connected();
    }

    size_t write(const uint8_t *buf, size_t len) override {
        if (!_mqtt.connected()) return 0;

        // Determine event type from packet (byte at offset 3 after magic+ver)
        if (len >= 4) {
            uint8_t type = buf[3];
            char topic[GRAFT_MQTT_MAX_TOPIC];

            // Choose QoS based on packet type
            int qos = 0;
            if (type == 0x81) { // HELLO_RESP
                snprintf(topic, sizeof(topic), "graft/%s/hello", _deviceId);
                _mqtt.publish(topic, buf, len, true); // retained
                return len;
            } else if (type == 0xFF) { // FATAL
                qos = 2;
            } else if (type == 0x82 || type == 0x83 || type == 0xC0 || type == 0xC1) {
                qos = 1; // ACK/NAK/MOD_EVENT/MOD_RESP
            }

            snprintf(topic, sizeof(topic), "graft/%s/evt/%02x", _deviceId, type);
            _mqtt.publish(topic, buf, len, false);
        }

        return len;
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

    bool needsCOBS() override { return false; }

    void poll() override {
        if (!_mqtt.connected()) {
            _reconnect();
        }
        _mqtt.loop();
    }

    /** Publish online status. Call after HELLO_RESP is sent. */
    void publishOnline() {
        char topic[GRAFT_MQTT_MAX_TOPIC];
        snprintf(topic, sizeof(topic), "graft/%s/status", _deviceId);
        _mqtt.publish(topic, (const uint8_t*)"online", 6, true);
    }

private:
    PubSubClient _mqtt;
    const char *_broker;
    uint16_t _port;
    char _deviceId[32];

    uint8_t _rxBuf[1024];
    size_t _rxLen;
    bool _hasData;

    bool _reconnect() {
        char willTopic[GRAFT_MQTT_MAX_TOPIC];
        snprintf(willTopic, sizeof(willTopic), "graft/%s/status", _deviceId);

        if (_mqtt.connect(_deviceId, willTopic, 1, true, "offline")) {
            // Subscribe to command topics
            char sub[GRAFT_MQTT_MAX_TOPIC];
            snprintf(sub, sizeof(sub), "graft/%s/cmd/#", _deviceId);
            _mqtt.subscribe(sub, 1);

            snprintf(sub, sizeof(sub), "graft/%s/ds/+/cmd", _deviceId);
            _mqtt.subscribe(sub, 1);

            publishOnline();
            return true;
        }
        return false;
    }

    void _onMessage(char *topic, uint8_t *payload, unsigned int length) {
        if (length > sizeof(_rxBuf)) return;
        memcpy(_rxBuf, payload, length);
        _rxLen = length;
        _hasData = true;
    }
};

#endif /* GRAFT_TRANSPORT_MQTT */
#endif /* GRAFT_MQTT_H */
