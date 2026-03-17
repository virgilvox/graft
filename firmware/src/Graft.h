/**
 * GRAFT — Generic Remote Actuator Firmware Transport
 *
 * Top-level include for the GRAFT firmware library.
 * Include this file in your Arduino sketch.
 *
 * Usage:
 *   #include <Graft.h>
 *
 *   GraftSerial transport(Serial, 115200);
 *   GraftDevice device("MyBoard", "1.0.0", transport);
 *
 *   void setup() {
 *     device.begin();
 *   }
 *
 *   void loop() {
 *     device.poll();
 *   }
 */

#ifndef GRAFT_H
#define GRAFT_H

/* Core */
#include "graft/core/graft_constants.h"
#include "graft/core/graft_types.h"
#include "graft/core/graft_crc8.h"
#include "graft/core/graft_cobs.h"
#include "graft/core/graft_wire.h"

/* Device */
#include "graft/GraftBoard.h"
#include "graft/GraftPayload.h"
#include "graft/GraftContext.h"
#include "graft/GraftModuleBase.h"
#include "graft/GraftDevice.h"

/* Transport */
#include "graft/transport/GraftTransport.h"
#include "graft/transport/GraftSerial.h"

/* ── Optional Modules (compile-time opt-in) ───────────── */

#ifdef GRAFT_MODULE_SERVO
#include "graft/modules/GraftModuleServo.h"
#endif

#ifdef GRAFT_MODULE_NEOPIXEL
#include "graft/modules/GraftModuleNeoPixel.h"
#endif

#ifdef GRAFT_MODULE_OLED
#include "graft/modules/GraftModuleOLED.h"
#endif

#ifdef GRAFT_MODULE_ENCODER
#include "graft/modules/GraftModuleEncoder.h"
#endif

#ifdef GRAFT_MODULE_STEPPER
#include "graft/modules/GraftModuleStepper.h"
#endif

#ifdef GRAFT_MODULE_DHT
#include "graft/modules/GraftModuleDHT.h"
#endif

#ifdef GRAFT_MODULE_PID
#include "graft/modules/GraftModulePID.h"
#endif

/* ── Macro Aliases ────────────────────────────────────── */

/** Alias for GRAFT data types in user code */
#define GRAFT_BOOL    GRAFT_TYPE_BOOL
#define GRAFT_INT8    GRAFT_TYPE_INT8
#define GRAFT_UINT8   GRAFT_TYPE_UINT8
#define GRAFT_INT16   GRAFT_TYPE_INT16
#define GRAFT_UINT16  GRAFT_TYPE_UINT16
#define GRAFT_INT32   GRAFT_TYPE_INT32
#define GRAFT_FLOAT32 GRAFT_TYPE_FLOAT32
#define GRAFT_STRING  GRAFT_TYPE_STRING
#define GRAFT_BYTES   GRAFT_TYPE_BYTES

#endif /* GRAFT_H */
