/*
 * ESP8266 Mesh Network - OLED Display Node
 * 
 * This node:
 * - Displays sensor data from mesh network on SSD1306 OLED
 * - Shows temperature, pressure, humidity from nearby sensors
 * - Relays messages between other mesh nodes
 * 
 * Hardware:
 * - ESP8266 Wemos D1 R2
 * - SSD1306 OLED Display 128x64 (I2C: SDA=GPIO4/D2, SCL=GPIO5/D1)
 * 
 * Dependencies:
 * - painlessMesh library
 * - ArduinoJson library
 * - U8g2 library (lightweight OLED driver)
 */

#include <Arduino.h>
#include <painlessMesh.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>

// OLED display configuration using U8g2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Mesh network configuration
#define MESH_SSID       "RooseveltMesh"
#define MESH_PASSWORD   "meshpass123"
#define MESH_PORT       5555

// Display update interval
#define DISPLAY_INTERVAL 5000  // 5 seconds

Scheduler userScheduler;
painlessMesh mesh;

// Store latest sensor data
struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  String nodeType;
  unsigned long lastUpdate;
  bool hasData;
} latestData;

// Task to update display
Task taskUpdateDisplay(DISPLAY_INTERVAL, TASK_FOREVER, []() {
  updateDisplay();
});

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize OLED display with U8g2
  Wire.begin();
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Roosevelt Lake");
  u8g2.drawStr(0, 24, "Mesh Network");
  u8g2.drawStr(0, 38, "Initializing...");
  u8g2.sendBuffer();
  
  Serial.println("OLED Display initialized");
  
  // Initialize sensor data
  latestData.hasData = false;
  latestData.lastUpdate = 0;
  
  // Configure mesh network
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(receivedCallback);
  mesh.onNewConnection([](uint32_t nodeId) {
    Serial.printf("New Connection: %u\n", nodeId);
  });
  mesh.onChangedConnections([]() {
    Serial.printf("Connections changed\n");
  });
  
  Serial.println("Display Mesh Node Initialized");
  Serial.print("Node ID: ");
  Serial.println(mesh.getNodeId());
  
  // Start display update task
  userScheduler.addTask(taskUpdateDisplay);
  taskUpdateDisplay.enable();
}

void loop() {
  mesh.update();
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u: %s\n", from, msg.c_str());
  
  // Parse JSON message
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, msg);
  
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Update sensor data
  latestData.nodeType = doc["type"] | "unknown";
  latestData.temperature = doc["temperature"] | 0.0;
  latestData.pressure = doc["pressure"] | 0.0;
  latestData.humidity = doc["humidity"] | 0.0;
  latestData.lastUpdate = millis();
  latestData.hasData = true;
  
  // Immediately update display with new data
  updateDisplay();
}

void updateDisplay() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Header
  u8g2.drawStr(0, 10, "Roosevelt Lake");
  u8g2.drawLine(0, 12, 128, 12);
  
  char buf[32];
  
  if (latestData.hasData) {
    unsigned long age = millis() - latestData.lastUpdate;
    
    if (age < 120000) {  // Data less than 2 minutes old
      // Display sensor type
      snprintf(buf, sizeof(buf), "%s", latestData.nodeType.c_str());
      u8g2.drawStr(0, 24, buf);
      
      // Temperature
      u8g2.setFont(u8g2_font_ncenB10_tr);
      snprintf(buf, sizeof(buf), "%.1fC", latestData.temperature);
      u8g2.drawStr(0, 40, buf);
      
      // Pressure
      u8g2.setFont(u8g2_font_ncenB08_tr);
      snprintf(buf, sizeof(buf), "P: %.0f hPa", latestData.pressure);
      u8g2.drawStr(0, 52, buf);
      
      // Humidity (if available)
      if (latestData.humidity > 0.1) {
        snprintf(buf, sizeof(buf), "H: %.0f%%", latestData.humidity);
        u8g2.drawStr(0, 63, buf);
      } else {
        // Show node count instead
        snprintf(buf, sizeof(buf), "Nodes: %d", mesh.getNodeList().size() + 1);
        u8g2.drawStr(0, 63, buf);
      }
      
    } else {
      // Data is stale
      u8g2.drawStr(0, 28, "WAITING FOR");
      u8g2.drawStr(0, 40, "SENSOR DATA");
      snprintf(buf, sizeof(buf), "Last: %lus ago", age / 1000);
      u8g2.drawStr(0, 56, buf);
    }
  } else {
    // No data yet
    u8g2.drawStr(0, 28, "CONNECTING TO");
    u8g2.drawStr(0, 40, "MESH NETWORK");
    snprintf(buf, sizeof(buf), "Nodes: %d", mesh.getNodeList().size() + 1);
    u8g2.drawStr(0, 56, buf);
  }
  
  u8g2.sendBuffer();
}
