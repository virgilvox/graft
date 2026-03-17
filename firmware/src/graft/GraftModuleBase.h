/**
 * GRAFT Module Base — Abstract interface for device-side modules
 *
 * Modules are opt-in C++ classes compiled into firmware.
 * They register at boot, appear in HELLO_RESP, and handle MOD_CMD packets.
 */

#ifndef GRAFT_MODULE_BASE_H
#define GRAFT_MODULE_BASE_H

#include <stdint.h>
#include "GraftPayload.h"

class GraftContext;
class GraftDevice;

class GraftModuleBase {
public:
    virtual ~GraftModuleBase() {}

    /** Module name, max 8 chars. Appears in HELLO_RESP. */
    virtual const char* name() = 0;

    /** Module version. */
    virtual uint8_t versionMajor() { return 1; }
    virtual uint8_t versionMinor() { return 0; }

    /** Called once after GraftDevice::begin(). */
    virtual void begin() {}

    /**
     * Handle a MOD_CMD packet.
     * @param cmd  Command byte (module-specific)
     * @param payload  Payload reader
     * @param ctx  Context for ack/nak/emit
     */
    virtual void handle(uint8_t cmd, GraftPayloadReader &payload, GraftContext &ctx) = 0;

    /**
     * Called every poll() cycle for modules that need periodic work
     * (e.g., stepper step generation, PID computation).
     */
    virtual void poll() {}

    /** Number of physical pins this module claims. */
    virtual uint8_t pinCount() { return 0; }

    /** Array of physical pin numbers claimed by this module. */
    virtual const uint8_t* pins() { return nullptr; }

    /** Set the owning device reference. Called by GraftDevice::begin(). */
    void setDevice(GraftDevice *dev) { _device = dev; }

protected:
    GraftDevice *_device = nullptr;
};

/* ── Convenience Macros ───────────────────────────────── */

/**
 * Quick module definition macro.
 * Usage:
 *   GRAFT_MODULE(myServo) {
 *     GRAFT_ON_CMD(0x01) { ... ctx.ack(); }
 *     GRAFT_ON_CMD(0x02) { ... ctx.ack(); }
 *   }
 */
#define GRAFT_MODULE(className) \
    class className : public GraftModuleBase

#define GRAFT_ON_CMD(cmdByte) \
    if (cmd == (cmdByte))

#endif /* GRAFT_MODULE_BASE_H */
