/**
 * GRAFT I2C Passthrough Module
 *
 * Raw I2C relay for host-driven peripherals not covered by other modules.
 * This is a thin wrapper — the host SDK handles register-level protocol.
 * Already handled by core I2C_WRITE/I2C_READ commands, so this module
 * exists primarily to appear in HELLO_RESP as a capability marker.
 *
 * Compile-time opt-in: #define GRAFT_MODULE_I2C_PASSTHROUGH
 */

#ifndef GRAFT_MODULE_I2C_PASSTHROUGH_H
#define GRAFT_MODULE_I2C_PASSTHROUGH_H

#ifdef GRAFT_MODULE_I2C_PASSTHROUGH

#include "../GraftModuleBase.h"
#include "../GraftContext.h"

class GraftModuleI2CPassthrough : public GraftModuleBase {
public:
    const char* name() override { return "i2c_pass"; }
    uint8_t versionMajor() override { return 1; }
    uint8_t versionMinor() override { return 0; }

    void handle(uint8_t cmd, GraftPayloadReader &payload, GraftContext &ctx) override {
        /* All I2C operations handled by core commands.
           This module is a capability marker. */
        ctx.nak(0x09);
    }
};

#endif /* GRAFT_MODULE_I2C_PASSTHROUGH */
#endif /* GRAFT_MODULE_I2C_PASSTHROUGH_H */
