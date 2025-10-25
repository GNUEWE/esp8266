/*
 * ESP8266 Mesh Network - BME680 Sensor Node
 * 
 * This node:
 * - Reads BME680 sensor (temperature, humidity, pressure, gas resistance)
 * - Sends data to mesh network as JSON
 * - Relays messages between other mesh nodes
 * 
 * Hardware:
 * - ESP8266 Wemos D1 R2
 * - BME680 sensor (I2C: SDA=GPIO4/D2, SCL=GPIO5/D1)
 * 
 * Dependencies:
 * - painlessMesh library
 * - ArduinoJson library
 * - Adafruit BME680 library
 * - Adafruit Unified Sensor library
 */

#include <Arduino.h>
#include <painlessMesh.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <ArduinoJson.h>

// Mesh network configuration
#define MESH_SSID       "RooseveltMesh"
#define MESH_PASSWORD   "meshpass123"
#define MESH_PORT       5555

// Sensor reading interval (milliseconds)
#define SENSOR_INTERVAL 30000  // 30 seconds

Scheduler userScheduler;
painlessMesh mesh;
Adafruit_BME680 bme;

bool sensorAvailable = false;
float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float gasResistance = 0.0;

// Task to send sensor data
Task taskSendSensorData(SENSOR_INTERVAL, TASK_FOREVER, []() {
  sendSensorData();
});

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Initialize I2C for BME680
  Wire.begin();
  
  // Initialize BME680 sensor
  if (!bme.begin()) {
    Serial.println("BME680 sensor not found!");
    sensorAvailable = false;
  } else {
    sensorAvailable = true;
    Serial.println("BME680 sensor found");
    
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320°C for 150 ms
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
  
  Serial.println("BME680 Mesh Sensor Node Initialized");
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
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading from BME680");
    return;
  }
  
  temperature = bme.temperature;
  humidity = bme.humidity;
  pressure = bme.pressure / 100.0;  // Convert Pa to hPa
  gasResistance = bme.gas_resistance;
  
  // Create JSON message
  DynamicJsonDocument doc(256);
  doc["type"] = "BME680";
  doc["temperature"] = temperature;
  doc["pressure"] = pressure;
  doc["humidity"] = humidity;
  doc["gas"] = gasResistance;
  
  String msg;
  serializeJson(doc, msg);
  
  // Broadcast to mesh
  mesh.sendBroadcast(msg);
  
  Serial.printf("Sent: T=%.1f°C, H=%.1f%%, P=%.1fhPa, Gas=%.0fΩ\n", 
                temperature, humidity, pressure, gasResistance);
}
