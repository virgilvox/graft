/**
 * GRAFT NeoPixel Module
 *
 * Commands:
 *   0x01  begin(pin, count, type)
 *   0x02  setPixel(index, r, g, b, w)
 *   0x03  setRange(start, count, r, g, b)
 *   0x04  fill(r, g, b, w)
 *   0x05  show()
 *   0x06  setBrightness(0-255)
 *
 * Compile-time opt-in: #define GRAFT_MODULE_NEOPIXEL
 * Requires: Adafruit_NeoPixel library
 */

#ifndef GRAFT_MODULE_NEOPIXEL_H
#define GRAFT_MODULE_NEOPIXEL_H

#ifdef GRAFT_MODULE_NEOPIXEL

#include "../GraftModuleBase.h"
#include "../GraftContext.h"

#ifdef ARDUINO
#include <Adafruit_NeoPixel.h>
#endif

class GraftModuleNeoPixel : public GraftModuleBase {
public:
    const char* name() override { return "neopixel"; }
    uint8_t versionMajor() override { return 1; }
    uint8_t versionMinor() override { return 0; }

    void begin() override {}

    void handle(uint8_t cmd, GraftPayloadReader &payload, GraftContext &ctx) override {
        switch (cmd) {
            case 0x01: { // begin
                _pin = payload.readUInt8();
                uint16_t count = payload.readUInt16();
                uint8_t type = payload.readUInt8();
#ifdef ARDUINO
                if (_strip) { delete _strip; _strip = nullptr; }
                _strip = new Adafruit_NeoPixel(count, _pin,
                    type == 0 ? NEO_GRB + NEO_KHZ800 : type);
                _strip->begin();
                _strip->show();
#endif
                ctx.ack();
                break;
            }
            case 0x02: { // setPixel
                uint16_t index = payload.readUInt16();
                uint8_t r = payload.readUInt8();
                uint8_t g = payload.readUInt8();
                uint8_t b = payload.readUInt8();
                uint8_t w = (payload.remaining() > 0) ? payload.readUInt8() : 0;
#ifdef ARDUINO
                if (_strip) _strip->setPixelColor(index, r, g, b, w);
#endif
                (void)w;
                ctx.ack();
                break;
            }
            case 0x03: { // setRange
                uint16_t start = payload.readUInt16();
                uint16_t count = payload.readUInt16();
                uint8_t r = payload.readUInt8();
                uint8_t g = payload.readUInt8();
                uint8_t b = payload.readUInt8();
#ifdef ARDUINO
                if (_strip) {
                    uint32_t color = Adafruit_NeoPixel::Color(r, g, b);
                    for (uint16_t i = start; i < start + count; i++) {
                        _strip->setPixelColor(i, color);
                    }
                }
#endif
                ctx.ack();
                break;
            }
            case 0x04: { // fill
                uint8_t r = payload.readUInt8();
                uint8_t g = payload.readUInt8();
                uint8_t b = payload.readUInt8();
                uint8_t w = (payload.remaining() > 0) ? payload.readUInt8() : 0;
#ifdef ARDUINO
                if (_strip) {
                    _strip->fill(Adafruit_NeoPixel::Color(r, g, b, w));
                }
#endif
                (void)w;
                ctx.ack();
                break;
            }
            case 0x05: { // show
#ifdef ARDUINO
                if (_strip) _strip->show();
#endif
                ctx.ack();
                break;
            }
            case 0x06: { // setBrightness
                uint8_t brightness = payload.readUInt8();
#ifdef ARDUINO
                if (_strip) {
                    _strip->setBrightness(brightness);
                    _strip->show();
                }
#endif
                ctx.ack();
                break;
            }
            default:
                ctx.nak(0x09);
                break;
        }
    }

    uint8_t pinCount() override { return (_pin != 0xFF) ? 1 : 0; }
    const uint8_t* pins() override { return (_pin != 0xFF) ? &_pin : nullptr; }

#ifdef ARDUINO
    ~GraftModuleNeoPixel() { if (_strip) delete _strip; }
#endif

private:
#ifdef ARDUINO
    Adafruit_NeoPixel *_strip = nullptr;
#endif
    uint8_t _pin = 0xFF;
};

#endif /* GRAFT_MODULE_NEOPIXEL */
#endif /* GRAFT_MODULE_NEOPIXEL_H */
