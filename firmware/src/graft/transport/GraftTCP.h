/**
 * GRAFT TCP Transport
 *
 * Raw TCP socket transport. Accepts any Arduino Client&.
 * No COBS needed — TCP provides message framing via packet length.
 */

#ifndef GRAFT_TCP_H
#define GRAFT_TCP_H

#include "GraftTransport.h"

#ifdef ARDUINO
#include <Client.h>

class GraftTCP : public GraftTransport {
public:
    /**
     * @param client  Arduino Client& (WiFiClient, EthernetClient, etc.)
     * @param host    Server hostname/IP
     * @param port    Server port
     */
    GraftTCP(Client &client, const char *host, uint16_t port)
        : _client(client), _host(host), _port(port) {}

    bool begin() override {
        return _client.connect(_host, _port);
    }

    bool connected() override {
        return _client.connected();
    }

    size_t write(const uint8_t *buf, size_t len) override {
        return _client.write(buf, len);
    }

    int available() override {
        return _client.available();
    }

    int read(uint8_t *buf, size_t maxLen) override {
        size_t count = 0;
        while (count < maxLen && _client.available()) {
            int b = _client.read();
            if (b < 0) break;
            buf[count++] = (uint8_t)b;
        }
        return (int)count;
    }

    void flush() override { _client.flush(); }
    bool needsCOBS() override { return false; }

private:
    Client &_client;
    const char *_host;
    uint16_t _port;
};

#endif /* ARDUINO */
#endif /* GRAFT_TCP_H */
