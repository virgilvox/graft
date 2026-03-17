/**
 * GRAFT — Servo Control Example
 *
 * Control a servo over serial. Connect with graft-js:
 *
 *   import { GraftDevice } from 'graft-js'
 *   import { SerialTransport } from 'graft-js/transports/serial'
 *   import { GraftServo } from 'graft-js/modules/servo'
 *
 *   const device = await GraftDevice.connect(
 *     new SerialTransport({ path: '/dev/ttyUSB0' })
 *   )
 *   const servo = new GraftServo(device)
 *   await servo.attach(9)
 *   await servo.write(90)
 */

#define GRAFT_MODULE_SERVO
#include <Graft.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("ServoDemo", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.begin();
}

void loop() {
  device.poll();
}
