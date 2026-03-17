/**
 * GRAFT — MQTT Sensor Example (ESP32)
 *
 * Connects to WiFi and an MQTT broker.
 * Pushes temperature datastream every 5 seconds.
 * Host subscribes with: device.datastream('temperature').subscribe()
 */

#include <WiFi.h>
#define GRAFT_TRANSPORT_MQTT
#include <Graft.h>
#include <graft/transport/GraftMQTT.h>

const char* WIFI_SSID   = "your-ssid";
const char* WIFI_PASS   = "your-password";
const char* MQTT_BROKER = "broker.local";
const uint16_t MQTT_PORT = 1883;

WiFiClient wifiClient;
GraftMQTT  transport(wifiClient, MQTT_BROKER, MQTT_PORT, "sensor-01");
GraftDevice device("SensorNode", "1.0.0", transport);

unsigned long lastPush = 0;

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Declare datastreams
  device.addDatastream("temperature", GRAFT_FLOAT32, "celsius", false);
  device.addDatastream("humidity",    GRAFT_FLOAT32, "percent", false);

  device.begin();
}

void loop() {
  device.poll();

  if (millis() - lastPush > 5000) {
    // Simulated sensor readings (replace with real sensor)
    float temp = 22.5 + (analogRead(34) / 4095.0) * 10.0;
    float hum  = 45.0 + (analogRead(35) / 4095.0) * 30.0;

    device.writeDatastream("temperature", temp);
    device.writeDatastream("humidity", hum);

    lastPush = millis();
  }
}
