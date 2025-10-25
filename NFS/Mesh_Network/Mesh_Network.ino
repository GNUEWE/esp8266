/*
 * ESP8266 Mesh Network - Root/Gateway Node with Display
 * 
 * This is the main gateway node that:
 * - Connects to mesh network
 * - Serves web portal (HTTP server)
 * - Collects sensor data from mesh nodes via JSON
 * - Displays aggregate data on OLED (U8g2 library)
 * - Publishes data via MQTT (optional)
 * 
 * Hardware: ESP8266 Wemos D1 R2 (Arduino Uno footprint)
 *           SSD1306 OLED Display 128x64 (I2C: SDA=GPIO4/D2, SCL=GPIO5/D1)
 * 
 * Dependencies:
 * - painlessMesh library
 * - ArduinoJson library
 * - U8g2 library (for OLED display)
 * - PubSubClient library (for MQTT)
 */

#include <Arduino.h>
#include <painlessMesh.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <PubSubClient.h>

// OLED display configuration using U8g2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Mesh network configuration
#define MESH_SSID       "RooseveltMesh"
#define MESH_PASSWORD   "meshpass123"
#define MESH_PORT       5555

// MQTT configuration (optional - set MQTT_ENABLED to true to use)
#define MQTT_ENABLED    false
#define MQTT_SERVER     "mqtt.example.com"
#define MQTT_PORT       1883
#define MQTT_TOPIC      "roosevelt/sensors"

// Web server
ESP8266WebServer server(80);
Scheduler userScheduler;
painlessMesh mesh;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Display update interval
#define DISPLAY_INTERVAL 3000  // 3 seconds
unsigned long lastDisplayUpdate = 0;

// Store sensor data from mesh nodes
struct SensorData {
  uint32_t nodeId;
  String nodeType;
  float temperature;
  float pressure;
  float humidity;
  float gasResistance;
  unsigned long lastUpdate;
  bool hasData;
};

#define MAX_NODES 4
SensorData sensorNodes[MAX_NODES];
int nodeCount = 0;

// Forward declarations
void receivedCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
String generateWebPage();
void updateDisplay();
void publishToMQTT();
void reconnectMQTT();

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Initialize OLED display
  Wire.begin();
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Roosevelt Lake");
  u8g2.drawStr(0, 24, "Mesh Gateway");
  u8g2.drawStr(0, 38, "Initializing...");
  u8g2.sendBuffer();
  
  // Initialize sensor data storage
  for (int i = 0; i < MAX_NODES; i++) {
    sensorNodes[i].hasData = false;
    sensorNodes[i].nodeId = 0;
    sensorNodes[i].lastUpdate = 0;
  }
  
  // Configure mesh network
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  
  // Set this node as root (gateway)
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  
  Serial.println("Mesh Gateway Node Initialized");
  Serial.print("Node ID: ");
  Serial.println(mesh.getNodeId());
  
  // Setup MQTT if enabled
  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    Serial.println("MQTT configured");
  }
  
  // Setup web server
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", generateWebPage());
  });
  
  server.on("/api/sensors", HTTP_GET, []() {
    DynamicJsonDocument doc(2048);
    JsonArray sensors = doc.createNestedArray("sensors");
    
    for (int i = 0; i < MAX_NODES; i++) {
      if (sensorNodes[i].hasData) {
        JsonObject node = sensors.createNestedObject();
        node["nodeId"] = sensorNodes[i].nodeId;
        node["type"] = sensorNodes[i].nodeType;
        node["temperature"] = sensorNodes[i].temperature;
        node["pressure"] = sensorNodes[i].pressure;
        node["humidity"] = sensorNodes[i].humidity;
        node["gasResistance"] = sensorNodes[i].gasResistance;
        node["lastUpdate"] = millis() - sensorNodes[i].lastUpdate;
      }
    }
    
    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
  });
  
  server.begin();
  Serial.println("HTTP Server Started on Port 80");
  
  // Initial display update
  updateDisplay();
}

void loop() {
  mesh.update();
  server.handleClient();
  
  // Update display periodically
  if (millis() - lastDisplayUpdate > DISPLAY_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  // Handle MQTT if enabled
  if (MQTT_ENABLED) {
    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();
  }
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u: %s\n", from, msg.c_str());
  
  // Parse JSON message
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, msg);
  
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Find or create entry for this node
  int nodeIndex = -1;
  for (int i = 0; i < MAX_NODES; i++) {
    if (sensorNodes[i].nodeId == from) {
      nodeIndex = i;
      break;
    }
    if (!sensorNodes[i].hasData && nodeIndex == -1) {
      nodeIndex = i;
    }
  }
  
  if (nodeIndex == -1) {
    Serial.println("No space for new node!");
    return;
  }
  
  // Update node data
  sensorNodes[nodeIndex].nodeId = from;
  sensorNodes[nodeIndex].nodeType = doc["type"] | "unknown";
  sensorNodes[nodeIndex].temperature = doc["temperature"] | 0.0;
  sensorNodes[nodeIndex].pressure = doc["pressure"] | 0.0;
  sensorNodes[nodeIndex].humidity = doc["humidity"] | 0.0;
  sensorNodes[nodeIndex].gasResistance = doc["gas"] | 0.0;
  sensorNodes[nodeIndex].lastUpdate = millis();
  sensorNodes[nodeIndex].hasData = true;
  
  Serial.printf("Updated node %u: T=%.1f°C, P=%.1fhPa\n", 
                from, sensorNodes[nodeIndex].temperature, sensorNodes[nodeIndex].pressure);
  
  // Publish to MQTT if enabled
  if (MQTT_ENABLED) {
    publishToMQTT();
  }
  
  // Update display immediately when new data arrives
  updateDisplay();
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection: %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Connections changed. Nodes: %d\n", mesh.getNodeList().size());
  
  // List all connected nodes
  auto nodes = mesh.getNodeList();
  Serial.print("Node list: ");
  for (auto node : nodes) {
    Serial.printf("%u ", node);
  }
  Serial.println();
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Time adjusted: %d us\n", offset);
}

String generateWebPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Roosevelt Lake Mesh Network</title>
  <style>
    body {
      font-family: Georgia, serif;
      max-width: 1200px;
      margin: 0 auto;
      padding: 20px;
      background: linear-gradient(to bottom, #87CEEB 0%, #E0F6FF 100%);
    }
    .container {
      background-color: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
    }
    h1 {
      color: #2C5F2D;
      border-bottom: 3px solid #4A7C59;
      padding-bottom: 10px;
    }
    .mesh-info {
      background-color: #E8F5E9;
      padding: 15px;
      border-left: 4px solid #4A7C59;
      margin: 20px 0;
      border-radius: 5px;
    }
    .sensor-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
      gap: 20px;
      margin: 30px 0;
    }
    .sensor-card {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      padding: 20px;
      border-radius: 15px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    .sensor-card.bme280 {
      background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
    }
    .sensor-card.bme680 {
      background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
    }
    .sensor-card.display {
      background: linear-gradient(135deg, #43e97b 0%, #38f9d7 100%);
    }
    .sensor-header {
      font-size: 1.2em;
      font-weight: bold;
      margin-bottom: 10px;
      border-bottom: 2px solid rgba(255,255,255,0.3);
      padding-bottom: 5px;
    }
    .sensor-value {
      font-size: 1.8em;
      font-weight: bold;
      margin: 10px 0;
    }
    .sensor-label {
      font-size: 0.9em;
      opacity: 0.9;
    }
    .node-id {
      font-size: 0.8em;
      opacity: 0.7;
      margin-top: 10px;
    }
    .offline {
      opacity: 0.5;
      background: linear-gradient(135deg, #666 0%, #999 100%);
    }
    .status {
      display: inline-block;
      padding: 3px 8px;
      border-radius: 3px;
      background-color: rgba(255,255,255,0.2);
      font-size: 0.8em;
      margin-left: 10px;
    }
  </style>
  <script>
    function updateData() {
      fetch('/api/sensors')
        .then(response => response.json())
        .then(data => {
          console.log('Sensor data:', data);
        })
        .catch(error => console.error('Error:', error));
    }
    
    // Auto-refresh every 10 seconds
    setInterval(updateData, 10000);
    setInterval(() => location.reload(), 30000);
  </script>
</head>
<body>
  <div class="container">
    <h1>🌐 Roosevelt Lake Mesh Network</h1>
    
    <div class="mesh-info">
      <strong>📡 Mesh Status:</strong> )rawliteral";
  
  html += String(mesh.getNodeList().size() + 1) + " nodes connected<br>";
  html += "<strong>🆔 Gateway ID:</strong> " + String(mesh.getNodeId()) + "<br>";
  html += "<strong>⏱️ Last Update:</strong> " + String(millis() / 1000) + "s<br>";
  
  html += R"rawliteral(
    </div>
    
    <h2>Sensor Nodes</h2>
    <div class="sensor-grid">
)rawliteral";
  
  // Display sensor nodes
  for (int i = 0; i < MAX_NODES; i++) {
    if (sensorNodes[i].hasData) {
      unsigned long age = millis() - sensorNodes[i].lastUpdate;
      bool isStale = age > 60000; // 1 minute
      
      String cardClass = "sensor-card";
      if (sensorNodes[i].nodeType == "BME680") {
        cardClass += " bme680";
      } else if (sensorNodes[i].nodeType == "BME280") {
        cardClass += " bme280";
      } else if (sensorNodes[i].nodeType == "DISPLAY") {
        cardClass += " display";
      }
      
      if (isStale) {
        cardClass += " offline";
      }
      
      html += "<div class=\"" + cardClass + "\">";
      html += "<div class=\"sensor-header\">" + sensorNodes[i].nodeType;
      html += "<span class=\"status\">" + String(isStale ? "OFFLINE" : "ONLINE") + "</span>";
      html += "</div>";
      
      html += "<div class=\"sensor-value\">" + String(sensorNodes[i].temperature, 1) + "°C</div>";
      html += "<div class=\"sensor-label\">Temperature</div>";
      
      html += "<div class=\"sensor-value\">" + String(sensorNodes[i].pressure, 1) + " hPa</div>";
      html += "<div class=\"sensor-label\">Pressure</div>";
      
      if (sensorNodes[i].nodeType == "BME680") {
        html += "<div class=\"sensor-value\">" + String(sensorNodes[i].humidity, 1) + "%</div>";
        html += "<div class=\"sensor-label\">Humidity</div>";
        
        html += "<div class=\"sensor-value\">" + String(sensorNodes[i].gasResistance / 1000.0, 1) + " kΩ</div>";
        html += "<div class=\"sensor-label\">Gas Resistance</div>";
      }
      
      html += "<div class=\"node-id\">Node: " + String(sensorNodes[i].nodeId) + "</div>";
      html += "<div class=\"node-id\">Updated: " + String(age / 1000) + "s ago</div>";
      html += "</div>";
    }
  }
  
  html += R"rawliteral(
    </div>
    
    <div class="mesh-info">
      <strong>ℹ️ About Mesh Network:</strong><br>
      This gateway node collects data from multiple sensor nodes across the Roosevelt Lake area.
      Each node communicates via mesh WiFi, extending coverage beyond a single access point.
    </div>
  </div>
</body>
</html>
)rawliteral";
  
  return html;
}

// Update OLED display with sensor data
void updateDisplay() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Header
  u8g2.drawStr(0, 10, "Roosevelt Lake");
  u8g2.drawLine(0, 12, 128, 12);
  
  // Mesh status
  char buf[32];
  snprintf(buf, sizeof(buf), "Nodes: %d", mesh.getNodeList().size() + 1);
  u8g2.drawStr(0, 24, buf);
  
  // Find most recent sensor data
  int recentIndex = -1;
  unsigned long mostRecent = 0;
  for (int i = 0; i < MAX_NODES; i++) {
    if (sensorNodes[i].hasData && sensorNodes[i].lastUpdate > mostRecent) {
      mostRecent = sensorNodes[i].lastUpdate;
      recentIndex = i;
    }
  }
  
  if (recentIndex >= 0) {
    // Display most recent sensor data
    u8g2.setFont(u8g2_font_ncenB10_tr);
    snprintf(buf, sizeof(buf), "%.1fC", sensorNodes[recentIndex].temperature);
    u8g2.drawStr(0, 40, buf);
    
    u8g2.setFont(u8g2_font_ncenB08_tr);
    snprintf(buf, sizeof(buf), "%.0fhPa", sensorNodes[recentIndex].pressure);
    u8g2.drawStr(0, 52, buf);
    
    if (sensorNodes[recentIndex].humidity > 0.1) {
      snprintf(buf, sizeof(buf), "H:%.0f%%", sensorNodes[recentIndex].humidity);
      u8g2.drawStr(0, 63, buf);
    }
    
    // Show sensor type
    u8g2.drawStr(70, 52, sensorNodes[recentIndex].nodeType.c_str());
  } else {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 40, "Waiting for");
    u8g2.drawStr(0, 52, "sensor data...");
  }
  
  u8g2.sendBuffer();
}

// Publish sensor data to MQTT
void publishToMQTT() {
  if (!mqttClient.connected()) {
    return;
  }
  
  DynamicJsonDocument doc(1024);
  JsonArray sensors = doc.createNestedArray("sensors");
  
  for (int i = 0; i < MAX_NODES; i++) {
    if (sensorNodes[i].hasData) {
      JsonObject node = sensors.createNestedObject();
      node["nodeId"] = sensorNodes[i].nodeId;
      node["type"] = sensorNodes[i].nodeType;
      node["temperature"] = sensorNodes[i].temperature;
      node["pressure"] = sensorNodes[i].pressure;
      node["humidity"] = sensorNodes[i].humidity;
      node["gasResistance"] = sensorNodes[i].gasResistance;
    }
  }
  
  String output;
  serializeJson(doc, output);
  mqttClient.publish(MQTT_TOPIC, output.c_str());
  Serial.println("Published to MQTT");
}

// Reconnect to MQTT broker
void reconnectMQTT() {
  if (millis() % 5000 < 100) {  // Try every 5 seconds
    Serial.print("Attempting MQTT connection...");
    String clientId = "RooseveltMesh-";
    clientId += String(mesh.getNodeId());
    
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttClient.state());
    }
  }
}
