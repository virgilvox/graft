/**
 * GRAFT — BLE Device Example (ESP32)
 *
 * Advertises as a BLE GATT device. A browser with Web Bluetooth
 * (or any BLE central) can discover and control it.
 */

#define GRAFT_MODULE_SERVO
#include <Graft.h>
#include <graft/transport/GraftBLE.h>

GraftBLE    transport("GRAFT-Servo");
GraftDevice device("BLEServo", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.addDatastream("angle", GRAFT_FLOAT32, "degrees", true);
  device.begin();
}

void loop() {
  device.poll();
}
