/**
 * GRAFT — Full Kitchen Sink Example (ESP32 + MQTT)
 *
 * Demonstrates: modules, datastreams, MQTT transport, all in one sketch.
 */

#include <WiFi.h>

#define GRAFT_MODULE_SERVO
#define GRAFT_MODULE_NEOPIXEL
#define GRAFT_TRANSPORT_MQTT
#include <Graft.h>
#include <graft/transport/GraftMQTT.h>

const char* WIFI_SSID   = "your-ssid";
const char* WIFI_PASS   = "your-password";
const char* MQTT_BROKER = "broker.local";

WiFiClient wifiClient;
GraftMQTT  transport(wifiClient, MQTT_BROKER, 1883, "kitchen-01");
GraftDevice device("FullKitchen", "1.0.0", transport);

float targetTemp = 22.0;
unsigned long lastPush = 0;

void onSetpoint(GraftPayloadReader &payload, GraftContext &ctx) {
  targetTemp = payload.readFloat32();
  ctx.ack();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  device.addModule(new GraftModuleServo());
  device.addModule(new GraftModuleNeoPixel());

  device.addDatastream("temperature", GRAFT_FLOAT32, "celsius",  false);
  device.addDatastream("humidity",    GRAFT_FLOAT32, "percent",  false);
  device.addDatastream("setpoint",    GRAFT_FLOAT32, "celsius",  true);
  device.addDatastream("led_mode",    GRAFT_UINT8,   "",         true);

  device.onDatastreamWrite("setpoint", onSetpoint);

  device.begin();
}

void loop() {
  device.poll();

  if (millis() - lastPush > 2000) {
    float temp = 20.0 + (analogRead(34) / 4095.0) * 15.0;
    float hum  = 40.0 + (analogRead(35) / 4095.0) * 40.0;
    device.writeDatastream("temperature", temp);
    device.writeDatastream("humidity", hum);
    lastPush = millis();
  }
}
