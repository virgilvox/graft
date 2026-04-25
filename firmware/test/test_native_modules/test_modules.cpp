/**
 * CONDUYT Module Native Tests
 *
 * Protocol-level tests for module classes that don't pull in external
 * vendor libraries: Encoder, Stepper, PID, I2CPassthrough.
 *
 * These cover the slice that the device-orchestrator tests in
 * test_native_device do not: each module's command dispatch, payload
 * parsing, ACK/NAK shape, and pin-claiming. ArduinoFake provides core
 * Arduino API (pinMode/digitalRead/analogRead/etc.) so the modules
 * compile and link in the native env.
 *
 * Library-dependent modules (Servo, NeoPixel, OLED, DHT) are not tested
 * here — they would need stub headers for Servo.h / Adafruit_NeoPixel.h
 * / Adafruit_SSD1306.h / DHT.h, which is a separate infrastructure
 * piece. Their wire format is exercised at the SDK end (see
 * sdk/python/tests/test_modules.py and sdk/js/tests/modules.test.ts).
 */

#include <ArduinoFake.h>
#include <unity.h>
#include <unistd.h>

extern "C" {
#include "../../src/conduyt/core/conduyt_wire.c"
#include "../../src/conduyt/core/conduyt_cobs.c"
}

#include "../../src/conduyt/core/conduyt_constants.h"
#include "../../src/conduyt/core/conduyt_types.h"
#include "../../src/conduyt/ConduytBoard.h"
#include "../../src/conduyt/ConduytPayload.h"
#include "../../src/conduyt/ConduytContext.h"
#include "../../src/conduyt/ConduytModuleBase.h"
#include "../../src/conduyt/transport/ConduytTransport.h"
#include "../../src/conduyt/ConduytDevice.h"
#include "../../src/conduyt/ConduytDevice.cpp"

#include "../../src/conduyt/modules/ConduytModuleEncoder.h"
#include "../../src/conduyt/modules/ConduytModuleStepper.h"
#include "../../src/conduyt/modules/ConduytModulePID.h"
#include "../../src/conduyt/modules/ConduytModuleI2CPassthrough.h"

#include "../test_native_device/MockTransport.h"

using namespace fakeit;

static MockTransport transport;

/** Build a MOD_CMD payload: [module_id, cmd_byte, ...module_payload] and feed it. */
static void feedModCmd(uint8_t moduleId, uint8_t cmdByte, uint8_t seq,
                       const uint8_t *modPayload = nullptr, size_t modPayloadLen = 0) {
    uint8_t buf[256];
    buf[0] = moduleId;
    buf[1] = cmdByte;
    if (modPayloadLen > 0 && modPayload) {
        memcpy(buf + 2, modPayload, modPayloadLen);
    }

    ConduytPacket pkt;
    pkt.version = CONDUYT_PROTOCOL_VERSION;
    pkt.type = CONDUYT_CMD_MOD_CMD;
    pkt.seq = seq;
    pkt.payload_len = (uint16_t)(2 + modPayloadLen);
    pkt.payload = buf;

    uint8_t wire[512];
    size_t len = conduyt_wire_encode(wire, sizeof(wire), &pkt);
    transport.feed(wire, len);
}

static ConduytResult decodeResponse(ConduytPacket *out, size_t offset = 0) {
    if (offset >= transport.writeLen) return CONDUYT_ERR_INCOMPLETE_PACKET;
    return conduyt_wire_decode(out, transport.writeBuf + offset, transport.writeLen - offset);
}

void setUp() { transport.reset(); }
void tearDown() {}

static void initArduinoMocks() {
    ArduinoFakeReset();
    When(Method(ArduinoFake(), millis)).AlwaysReturn(0);
    When(Method(ArduinoFake(), micros)).AlwaysReturn(0);
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(1);
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(512);
    When(Method(ArduinoFake(), analogWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();
}

/* ================================================================
 * ENCODER
 * ================================================================ */

void test_encoder_attach_acks() {
    ConduytModuleEncoder enc;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&enc);
    device.begin();
    transport.clearWrite();

    uint8_t modPayload[] = {2, 3}; // pinA=2, pinB=3
    feedModCmd(0, 0x01, 1, modPayload, 2);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(1, resp.seq);
}

void test_encoder_attach_claims_two_pins() {
    ConduytModuleEncoder enc;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&enc);
    device.begin();

    uint8_t modPayload[] = {7, 8};
    feedModCmd(0, 0x01, 2, modPayload, 2);
    device.poll();

    TEST_ASSERT_EQUAL(2, enc.pinCount());
    const uint8_t *pins = enc.pins();
    TEST_ASSERT_NOT_NULL(pins);
    TEST_ASSERT_EQUAL(7, pins[0]);
    TEST_ASSERT_EQUAL(8, pins[1]);
}

void test_encoder_pins_null_before_attach() {
    ConduytModuleEncoder enc;
    TEST_ASSERT_EQUAL(0, enc.pinCount());
    TEST_ASSERT_NULL(enc.pins());
}

void test_encoder_read_returns_int32_count() {
    ConduytModuleEncoder enc;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&enc);
    device.begin();

    uint8_t attachPayload[] = {2, 3};
    feedModCmd(0, 0x01, 1, attachPayload, 2);
    device.poll();
    transport.clearWrite();

    feedModCmd(0, 0x02, 5); // read
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_MOD_RESP, resp.type);
    TEST_ASSERT_EQUAL(5, resp.seq);
    /* Payload: [module_id, ...int32_le] = 5 bytes */
    TEST_ASSERT_EQUAL(5, resp.payload_len);
    TEST_ASSERT_EQUAL(0, resp.payload[0]); // module_id
    /* count starts at 0 */
    TEST_ASSERT_EQUAL(0, resp.payload[1]);
    TEST_ASSERT_EQUAL(0, resp.payload[2]);
    TEST_ASSERT_EQUAL(0, resp.payload[3]);
    TEST_ASSERT_EQUAL(0, resp.payload[4]);
}

void test_encoder_reset_acks() {
    ConduytModuleEncoder enc;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&enc);
    device.begin();
    transport.clearWrite();

    feedModCmd(0, 0x03, 9);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(9, resp.seq);
}

void test_encoder_unknown_cmd_naks() {
    ConduytModuleEncoder enc;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&enc);
    device.begin();
    transport.clearWrite();

    feedModCmd(0, 0x77, 12);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_NAK, resp.type);
    TEST_ASSERT_EQUAL(0x09, resp.payload[0]); // UNKNOWN_MODULE_CMD
}

void test_encoder_descriptor() {
    ConduytModuleEncoder enc;
    TEST_ASSERT_EQUAL_STRING("encoder", enc.name());
    TEST_ASSERT_EQUAL(1, enc.versionMajor());
    TEST_ASSERT_EQUAL(0, enc.versionMinor());
}

/* ================================================================
 * STEPPER
 * ================================================================ */

void test_stepper_config_acks_and_claims_three_pins() {
    ConduytModuleStepper step;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&step);
    device.begin();
    transport.clearWrite();

    /* config(stepPin=2, dirPin=3, enPin=4, stepsPerRev=200) */
    uint8_t payload[] = {2, 3, 4, 200, 0};
    feedModCmd(0, 0x01, 1, payload, 5);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);

    TEST_ASSERT_EQUAL(3, step.pinCount());
    const uint8_t *pins = step.pins();
    TEST_ASSERT_NOT_NULL(pins);
    TEST_ASSERT_EQUAL(2, pins[0]);
    TEST_ASSERT_EQUAL(3, pins[1]);
    TEST_ASSERT_EQUAL(4, pins[2]);
}

void test_stepper_pin_count_zero_before_config() {
    ConduytModuleStepper step;
    TEST_ASSERT_EQUAL(0, step.pinCount());
    TEST_ASSERT_NULL(step.pins());
}

void test_stepper_move_acks() {
    ConduytModuleStepper step;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&step);
    device.begin();

    uint8_t cfg[] = {2, 3, 4, 200, 0};
    feedModCmd(0, 0x01, 1, cfg, 5);
    device.poll();
    transport.clearWrite();

    /* move(steps=100, speed=1000) — int32 LE then uint16 LE */
    uint8_t mv[] = {100, 0, 0, 0, 0xE8, 0x03};
    feedModCmd(0, 0x02, 4, mv, 6);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(4, resp.seq);
}

void test_stepper_move_to_acks() {
    ConduytModuleStepper step;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&step);
    device.begin();

    uint8_t cfg[] = {2, 3, 4, 200, 0};
    feedModCmd(0, 0x01, 1, cfg, 5);
    device.poll();
    transport.clearWrite();

    /* moveTo(target=50, speed=500) */
    uint8_t mv[] = {50, 0, 0, 0, 0xF4, 0x01};
    feedModCmd(0, 0x03, 7, mv, 6);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(7, resp.seq);
}

void test_stepper_stop_acks() {
    ConduytModuleStepper step;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&step);
    device.begin();
    transport.clearWrite();

    feedModCmd(0, 0x04, 11);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(11, resp.seq);
}

void test_stepper_unknown_cmd_naks() {
    ConduytModuleStepper step;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&step);
    device.begin();
    transport.clearWrite();

    feedModCmd(0, 0x88, 13);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_NAK, resp.type);
    TEST_ASSERT_EQUAL(0x09, resp.payload[0]);
}

void test_stepper_descriptor() {
    ConduytModuleStepper step;
    TEST_ASSERT_EQUAL_STRING("stepper", step.name());
    TEST_ASSERT_EQUAL(1, step.versionMajor());
}

/* ================================================================
 * PID
 * ================================================================ */

void test_pid_config_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    /* config(kp=1.0, ki=0.5, kd=0.1) — three float32 LE.
       Wire bytes for these values: 1.0=0x3F800000, 0.5=0x3F000000, 0.1=0x3DCCCCCD */
    uint8_t payload[12] = {
        0x00, 0x00, 0x80, 0x3F,  // kp = 1.0
        0x00, 0x00, 0x00, 0x3F,  // ki = 0.5
        0xCD, 0xCC, 0xCC, 0x3D,  // kd = 0.1
    };
    feedModCmd(0, 0x01, 1, payload, 12);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
}

void test_pid_set_target_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    /* setTarget(50.0) — 0x42480000 LE */
    uint8_t payload[4] = { 0x00, 0x00, 0x48, 0x42 };
    feedModCmd(0, 0x02, 3, payload, 4);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
    TEST_ASSERT_EQUAL(3, resp.seq);
}

void test_pid_set_input_pin_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    uint8_t payload[1] = { 14 };
    feedModCmd(0, 0x03, 5, payload, 1);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
}

void test_pid_set_output_pin_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    uint8_t payload[1] = { 5 };
    feedModCmd(0, 0x04, 6, payload, 1);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
}

void test_pid_enable_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    uint8_t en[1] = { 1 };
    feedModCmd(0, 0x05, 7, en, 1);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
}

void test_pid_disable_acks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    uint8_t off[1] = { 0 };
    feedModCmd(0, 0x05, 8, off, 1);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_ACK, resp.type);
}

void test_pid_unknown_cmd_naks() {
    ConduytModulePID pid;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&pid);
    device.begin();
    transport.clearWrite();

    feedModCmd(0, 0x99, 14);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_NAK, resp.type);
    TEST_ASSERT_EQUAL(0x09, resp.payload[0]);
}

void test_pid_descriptor() {
    ConduytModulePID pid;
    TEST_ASSERT_EQUAL_STRING("pid", pid.name());
    TEST_ASSERT_EQUAL(1, pid.versionMajor());
}

void test_pid_no_pins_claimed() {
    /* PID does not occupy pins exclusively (input/output are configured
       per-command, no exclusive claim). pinCount stays 0. */
    ConduytModulePID pid;
    TEST_ASSERT_EQUAL(0, pid.pinCount());
}

/* ================================================================
 * I2C PASSTHROUGH (capability marker only)
 * ================================================================ */

void test_i2cpass_any_cmd_naks() {
    ConduytModuleI2CPassthrough mod;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&mod);
    device.begin();
    transport.clearWrite();

    /* Even cmd 0x01 — module is just a marker, all real work is via
       core CMD_I2C_WRITE/CMD_I2C_READ. */
    feedModCmd(0, 0x01, 21);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_NAK, resp.type);
    TEST_ASSERT_EQUAL(0x09, resp.payload[0]);
}

void test_i2cpass_descriptor() {
    ConduytModuleI2CPassthrough mod;
    TEST_ASSERT_EQUAL_STRING("i2c_pass", mod.name());
    TEST_ASSERT_EQUAL(1, mod.versionMajor());
    TEST_ASSERT_EQUAL(0, mod.pinCount());
}

void test_i2cpass_appears_in_hello_resp() {
    ConduytModuleI2CPassthrough mod;
    ConduytDevice device("FW", "1.0.0", transport);
    device.addModule(&mod);
    device.begin();
    transport.clearWrite();

    /* Send HELLO and confirm the module name is present in payload. */
    ConduytPacket helloPkt;
    helloPkt.version = CONDUYT_PROTOCOL_VERSION;
    helloPkt.type = CONDUYT_CMD_HELLO;
    helloPkt.seq = 0;
    helloPkt.payload_len = 0;
    helloPkt.payload = nullptr;
    uint8_t buf[64];
    size_t len = conduyt_wire_encode(buf, sizeof(buf), &helloPkt);
    transport.feed(buf, len);
    device.poll();

    ConduytPacket resp;
    TEST_ASSERT_EQUAL(CONDUYT_OK, decodeResponse(&resp));
    TEST_ASSERT_EQUAL(CONDUYT_EVT_HELLO_RESP, resp.type);

    /* Search for "i2c_pass" substring in payload. The module section
       starts after firmware name+version+pin descriptors. We don't
       need an exact offset — substring match is enough to prove the
       marker registered. */
    bool found = false;
    for (size_t i = 0; i + 8 <= resp.payload_len; i++) {
        if (memcmp(resp.payload + i, "i2c_pass", 8) == 0) {
            found = true;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "i2c_pass module name not in HELLO_RESP");
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(int argc, char **argv) {
    initArduinoMocks();
    UNITY_BEGIN();

    /* Encoder */
    RUN_TEST(test_encoder_descriptor);
    RUN_TEST(test_encoder_pins_null_before_attach);
    RUN_TEST(test_encoder_attach_acks);
    RUN_TEST(test_encoder_attach_claims_two_pins);
    RUN_TEST(test_encoder_read_returns_int32_count);
    RUN_TEST(test_encoder_reset_acks);
    RUN_TEST(test_encoder_unknown_cmd_naks);

    /* Stepper */
    RUN_TEST(test_stepper_descriptor);
    RUN_TEST(test_stepper_pin_count_zero_before_config);
    RUN_TEST(test_stepper_config_acks_and_claims_three_pins);
    RUN_TEST(test_stepper_move_acks);
    RUN_TEST(test_stepper_move_to_acks);
    RUN_TEST(test_stepper_stop_acks);
    RUN_TEST(test_stepper_unknown_cmd_naks);

    /* PID */
    RUN_TEST(test_pid_descriptor);
    RUN_TEST(test_pid_no_pins_claimed);
    RUN_TEST(test_pid_config_acks);
    RUN_TEST(test_pid_set_target_acks);
    RUN_TEST(test_pid_set_input_pin_acks);
    RUN_TEST(test_pid_set_output_pin_acks);
    RUN_TEST(test_pid_enable_acks);
    RUN_TEST(test_pid_disable_acks);
    RUN_TEST(test_pid_unknown_cmd_naks);

    /* I2C Passthrough */
    RUN_TEST(test_i2cpass_descriptor);
    RUN_TEST(test_i2cpass_any_cmd_naks);
    RUN_TEST(test_i2cpass_appears_in_hello_resp);

    int result = UNITY_END();
    _exit(result);
    return result;
}
