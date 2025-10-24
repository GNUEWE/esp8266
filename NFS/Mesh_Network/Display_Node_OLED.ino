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
 * - Adafruit SSD1306 library
 * - Adafruit GFX library
 */

#include <Arduino.h>
#include <painlessMesh.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin not used
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Roosevelt Lake"));
  display.println(F("Mesh Network"));
  display.println();
  display.println(F("Initializing..."));
  display.display();
  
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  // Header
  display.setTextSize(1);
  display.println(F("Roosevelt Lake"));
  display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
  
  display.setCursor(0, 14);
  
  if (latestData.hasData) {
    unsigned long age = millis() - latestData.lastUpdate;
    
    if (age < 120000) {  // Data less than 2 minutes old
      // Display sensor type
      display.print(F("Sensor: "));
      display.println(latestData.nodeType);
      display.println();
      
      // Temperature
      display.setTextSize(2);
      display.print(latestData.temperature, 1);
      display.setTextSize(1);
      display.println(F(" C"));
      
      // Pressure
      display.setTextSize(1);
      display.print(F("Press: "));
      display.print(latestData.pressure, 0);
      display.println(F(" hPa"));
      
      // Humidity (if available)
      if (latestData.humidity > 0.1) {
        display.print(F("Humid: "));
        display.print(latestData.humidity, 0);
        display.println(F(" %"));
      }
      
      // Mesh info
      display.println();
      display.print(F("Nodes: "));
      display.println(mesh.getNodeList().size() + 1);
      
    } else {
      // Data is stale
      display.setTextSize(1);
      display.println();
      display.println(F("  WAITING FOR"));
      display.println(F("  SENSOR DATA"));
      display.println();
      display.print(F("Last: "));
      display.print(age / 1000);
      display.println(F("s ago"));
    }
  } else {
    // No data yet
    display.setTextSize(1);
    display.println();
    display.println(F("  CONNECTING TO"));
    display.println(F("  MESH NETWORK"));
    display.println();
    display.print(F("Nodes: "));
    display.println(mesh.getNodeList().size() + 1);
  }
  
  display.display();
}
