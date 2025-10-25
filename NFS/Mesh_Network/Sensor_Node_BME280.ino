/*
 * ESP8266 Mesh Network - BME280 Sensor Node
 * 
 * This node:
 * - Reads BME280 sensor (temperature + pressure only, no humidity)
 * - Sends data to mesh network as JSON
 * - Relays messages between other mesh nodes
 * 
 * Hardware:
 * - ESP8266 Wemos D1 R2
 * - BME280 sensor (I2C: SDA=GPIO4/D2, SCL=GPIO5/D1)
 * 
 * Dependencies:
 * - painlessMesh library
 * - ArduinoJson library
 * - BME280I2C library by Tyler Glenn
 */

#include <Arduino.h>
#include <painlessMesh.h>
#include <Wire.h>
#include <BME280I2C.h>
#include <ArduinoJson.h>

// Mesh network configuration
#define MESH_SSID       "RooseveltMesh"
#define MESH_PASSWORD   "meshpass123"
#define MESH_PORT       5555

// Sensor reading interval (milliseconds)
#define SENSOR_INTERVAL 30000  // 30 seconds

Scheduler userScheduler;
painlessMesh mesh;
BME280I2C bme;

bool sensorAvailable = false;
float temperature = 0.0;
float pressure = 0.0;

// Task to send sensor data
Task taskSendSensorData(SENSOR_INTERVAL, TASK_FOREVER, []() {
  sendSensorData();
});

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Initialize I2C for BME280
  Wire.begin();
  
  // Initialize BME280 sensor
  if (!bme.begin()) {
    Serial.println("BME280 sensor not found!");
    sensorAvailable = false;
  } else {
    sensorAvailable = true;
    
    // Detect sensor type
    switch(bme.chipModel()) {
      case BME280::ChipModel_BME280:
        Serial.println("BME280 sensor found");
        break;
      case BME280::ChipModel_BMP280:
        Serial.println("BMP280 sensor found (no humidity)");
        break;
      default:
        Serial.println("Unknown sensor found");
    }
  }
  
  // Configure mesh network
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive([](uint32_t from, String &msg) {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());
  });
  mesh.onNewConnection([](uint32_t nodeId) {
    Serial.printf("New Connection: %u\n", nodeId);
  });
  mesh.onChangedConnections([]() {
    Serial.printf("Connections changed\n");
  });
  
  Serial.println("BME280 Mesh Sensor Node Initialized");
  Serial.print("Node ID: ");
  Serial.println(mesh.getNodeId());
  
  // Start sensor reading task
  userScheduler.addTask(taskSendSensorData);
  taskSendSensorData.enable();
  
  // Send initial data after 5 seconds
  delay(5000);
  sendSensorData();
}

void loop() {
  mesh.update();
}

void sendSensorData() {
  if (!sensorAvailable) {
    Serial.println("Sensor not available, skipping data send");
    return;
  }
  
  // Read sensor
  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
  
  bme.read(pres, temp, hum, tempUnit, presUnit);
  
  if (!isnan(temp)) temperature = temp;
  if (!isnan(pres)) pressure = pres;
  
  // Create JSON message
  DynamicJsonDocument doc(256);
  doc["type"] = "BME280";
  doc["temperature"] = temperature;
  doc["pressure"] = pressure;
  doc["humidity"] = 0.0;  // BME280 may not have humidity
  doc["gas"] = 0.0;       // No gas sensor
  
  String msg;
  serializeJson(doc, msg);
  
  // Broadcast to mesh
  mesh.sendBroadcast(msg);
  
  Serial.printf("Sent: T=%.1f°C, P=%.1fhPa\n", temperature, pressure);
}
