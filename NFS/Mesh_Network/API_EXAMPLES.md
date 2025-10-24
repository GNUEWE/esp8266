# API Usage Examples

This document provides examples of how to interact with the mesh network gateway's JSON API.

## REST API Endpoint

The gateway provides a REST API endpoint for programmatic access to sensor data:

```
GET http://192.168.4.1/api/sensors
```

## Response Format

The API returns JSON with all active sensor nodes:

```json
{
  "sensors": [
    {
      "nodeId": 1234567890,
      "type": "BME280",
      "temperature": 25.5,
      "pressure": 1013.2,
      "humidity": 0.0,
      "gasResistance": 0.0,
      "lastUpdate": 5234
    },
    {
      "nodeId": 9876543210,
      "type": "BME680",
      "temperature": 26.3,
      "pressure": 1012.8,
      "humidity": 45.2,
      "gasResistance": 125000.0,
      "lastUpdate": 3421
    }
  ]
}
```

### Field Descriptions

- `nodeId`: Unique identifier for the mesh node (unsigned 32-bit integer)
- `type`: Sensor type ("BME280", "BME680", or "DISPLAY")
- `temperature`: Temperature in degrees Celsius
- `pressure`: Atmospheric pressure in hectopascals (hPa)
- `humidity`: Relative humidity in percent (0.0 for BME280)
- `gasResistance`: Gas resistance in Ohms (0.0 for BME280, air quality indicator for BME680)
- `lastUpdate`: Milliseconds since last update from this node

---

## Example 1: JavaScript Fetch API

Retrieve sensor data from a web page:

```javascript
// Fetch sensor data
fetch('http://192.168.4.1/api/sensors')
  .then(response => response.json())
  .then(data => {
    console.log('Sensor Data:', data);
    
    // Process each sensor
    data.sensors.forEach(sensor => {
      console.log(`Node ${sensor.nodeId} (${sensor.type}):`);
      console.log(`  Temperature: ${sensor.temperature}°C`);
      console.log(`  Pressure: ${sensor.pressure} hPa`);
      
      if (sensor.type === 'BME680') {
        console.log(`  Humidity: ${sensor.humidity}%`);
        console.log(`  Air Quality: ${sensor.gasResistance}Ω`);
      }
      
      console.log(`  Last Update: ${sensor.lastUpdate}ms ago`);
    });
  })
  .catch(error => {
    console.error('Error fetching sensor data:', error);
  });
```

---

## Example 2: Python Script

Read and display sensor data:

```python
#!/usr/bin/env python3
import requests
import json
import time

GATEWAY_URL = "http://192.168.4.1/api/sensors"

def fetch_sensor_data():
    try:
        response = requests.get(GATEWAY_URL, timeout=5)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")
        return None

def display_sensors(data):
    if not data or 'sensors' not in data:
        print("No sensor data available")
        return
    
    print("\n" + "="*60)
    print("Roosevelt Lake Mesh Network - Sensor Data")
    print("="*60)
    
    for sensor in data['sensors']:
        print(f"\nNode ID: {sensor['nodeId']}")
        print(f"Type: {sensor['type']}")
        print(f"Temperature: {sensor['temperature']:.1f}°C ({sensor['temperature']*9/5+32:.1f}°F)")
        print(f"Pressure: {sensor['pressure']:.1f} hPa ({sensor['pressure']*0.02953:.2f} inHg)")
        
        if sensor['humidity'] > 0:
            print(f"Humidity: {sensor['humidity']:.1f}%")
        
        if sensor['gasResistance'] > 0:
            print(f"Gas Resistance: {sensor['gasResistance']:.0f} Ω")
        
        age_seconds = sensor['lastUpdate'] / 1000
        print(f"Last Update: {age_seconds:.1f}s ago")
    
    print("\n" + "="*60)

def main():
    print("Monitoring Roosevelt Lake Mesh Network")
    print("Press Ctrl+C to exit\n")
    
    try:
        while True:
            data = fetch_sensor_data()
            if data:
                display_sensors(data)
            else:
                print("Failed to retrieve sensor data")
            
            time.sleep(10)  # Update every 10 seconds
    
    except KeyboardInterrupt:
        print("\nMonitoring stopped")

if __name__ == "__main__":
    main()
```

**Usage:**
```bash
python3 monitor_sensors.py
```

---

## Example 3: Arduino/ESP8266 Client

Another ESP8266 can read data from the gateway:

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "RooseveltMesh-Gateway";
const char* password = "your-password";
const char* apiUrl = "http://192.168.4.1/api/sensors";

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  HTTPClient http;
  
  http.begin(wifiClient, apiUrl);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      JsonArray sensors = doc["sensors"];
      
      Serial.println("\n--- Sensor Data ---");
      for (JsonObject sensor : sensors) {
        Serial.print("Node: ");
        Serial.println(sensor["nodeId"].as<unsigned long>());
        Serial.print("Type: ");
        Serial.println(sensor["type"].as<String>());
        Serial.print("Temp: ");
        Serial.print(sensor["temperature"].as<float>());
        Serial.println("°C");
        Serial.print("Pressure: ");
        Serial.print(sensor["pressure"].as<float>());
        Serial.println(" hPa");
        Serial.println();
      }
    } else {
      Serial.println("JSON parse error");
    }
  } else {
    Serial.printf("HTTP error: %d\n", httpCode);
  }
  
  http.end();
  delay(10000);  // Update every 10 seconds
}
```

---

## Example 4: cURL Command Line

Fetch data from terminal:

```bash
# Simple GET request
curl http://192.168.4.1/api/sensors

# Pretty-print JSON (requires jq)
curl -s http://192.168.4.1/api/sensors | jq

# Extract just temperatures
curl -s http://192.168.4.1/api/sensors | jq '.sensors[].temperature'

# Filter BME680 sensors only
curl -s http://192.168.4.1/api/sensors | jq '.sensors[] | select(.type=="BME680")'

# Get average temperature
curl -s http://192.168.4.1/api/sensors | jq '[.sensors[].temperature] | add/length'
```

---

## Example 5: Node-RED Flow

Integrate with Node-RED for home automation:

```json
[
  {
    "id": "http_request",
    "type": "http request",
    "method": "GET",
    "url": "http://192.168.4.1/api/sensors",
    "name": "Get Sensors",
    "x": 200,
    "y": 100
  },
  {
    "id": "json_parse",
    "type": "json",
    "name": "Parse JSON",
    "x": 400,
    "y": 100
  },
  {
    "id": "function",
    "type": "function",
    "name": "Process Data",
    "func": "msg.payload = msg.payload.sensors; return msg;",
    "x": 600,
    "y": 100
  },
  {
    "id": "debug",
    "type": "debug",
    "name": "Display",
    "x": 800,
    "y": 100
  }
]
```

---

## Example 6: Data Logging to CSV

Log sensor data to CSV file:

```python
#!/usr/bin/env python3
import requests
import csv
import time
from datetime import datetime

GATEWAY_URL = "http://192.168.4.1/api/sensors"
CSV_FILE = "sensor_log.csv"

def log_sensor_data():
    try:
        response = requests.get(GATEWAY_URL, timeout=5)
        data = response.json()
        
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        with open(CSV_FILE, 'a', newline='') as csvfile:
            writer = csv.writer(csvfile)
            
            # Write header if file is empty
            if csvfile.tell() == 0:
                writer.writerow([
                    'Timestamp', 'NodeID', 'Type', 
                    'Temperature', 'Pressure', 'Humidity', 
                    'GasResistance', 'LastUpdate'
                ])
            
            # Write each sensor reading
            for sensor in data.get('sensors', []):
                writer.writerow([
                    timestamp,
                    sensor['nodeId'],
                    sensor['type'],
                    sensor['temperature'],
                    sensor['pressure'],
                    sensor['humidity'],
                    sensor['gasResistance'],
                    sensor['lastUpdate']
                ])
        
        print(f"{timestamp}: Logged {len(data.get('sensors', []))} sensors")
        
    except Exception as e:
        print(f"Error: {e}")

def main():
    print("Starting sensor data logger...")
    print(f"Logging to: {CSV_FILE}")
    
    try:
        while True:
            log_sensor_data()
            time.sleep(60)  # Log every minute
    except KeyboardInterrupt:
        print("\nLogging stopped")

if __name__ == "__main__":
    main()
```

---

## Example 7: Web Dashboard (HTML + JavaScript)

Simple dashboard page:

```html
<!DOCTYPE html>
<html>
<head>
  <title>Roosevelt Lake Sensors</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    .sensor { 
      border: 1px solid #ccc; 
      padding: 15px; 
      margin: 10px 0; 
      border-radius: 5px;
    }
    .temp { background-color: #ffebee; }
    .pressure { background-color: #e3f2fd; }
  </style>
</head>
<body>
  <h1>Roosevelt Lake Mesh Network</h1>
  <div id="sensors"></div>
  
  <script>
    function updateSensorData() {
      fetch('http://192.168.4.1/api/sensors')
        .then(response => response.json())
        .then(data => {
          const container = document.getElementById('sensors');
          container.innerHTML = '';
          
          data.sensors.forEach(sensor => {
            const div = document.createElement('div');
            div.className = 'sensor';
            
            const tempF = (sensor.temperature * 9/5 + 32).toFixed(1);
            const pressureInHg = (sensor.pressure * 0.02953).toFixed(2);
            
            div.innerHTML = `
              <h3>${sensor.type} (Node: ${sensor.nodeId})</h3>
              <div class="temp">
                <strong>Temperature:</strong> 
                ${sensor.temperature.toFixed(1)}°C (${tempF}°F)
              </div>
              <div class="pressure">
                <strong>Pressure:</strong> 
                ${sensor.pressure.toFixed(1)} hPa (${pressureInHg} inHg)
              </div>
              ${sensor.humidity > 0 ? 
                `<div><strong>Humidity:</strong> ${sensor.humidity.toFixed(1)}%</div>` : ''}
              ${sensor.gasResistance > 0 ? 
                `<div><strong>Air Quality:</strong> ${sensor.gasResistance.toFixed(0)} Ω</div>` : ''}
              <div><em>Updated: ${(sensor.lastUpdate/1000).toFixed(0)}s ago</em></div>
            `;
            
            container.appendChild(div);
          });
        })
        .catch(error => {
          console.error('Error:', error);
          document.getElementById('sensors').innerHTML = 
            '<p style="color: red;">Error loading sensor data</p>';
        });
    }
    
    // Update immediately and then every 10 seconds
    updateSensorData();
    setInterval(updateSensorData, 10000);
  </script>
</body>
</html>
```

---

## Example 8: Alert System

Monitor for temperature thresholds:

```python
#!/usr/bin/env python3
import requests
import time

GATEWAY_URL = "http://192.168.4.1/api/sensors"
TEMP_THRESHOLD_HIGH = 40.0  # °C
TEMP_THRESHOLD_LOW = 0.0    # °C

def check_alerts():
    try:
        response = requests.get(GATEWAY_URL, timeout=5)
        data = response.json()
        
        for sensor in data.get('sensors', []):
            temp = sensor['temperature']
            node_id = sensor['nodeId']
            
            if temp > TEMP_THRESHOLD_HIGH:
                print(f"⚠️  HIGH TEMP ALERT: Node {node_id} at {temp}°C!")
            elif temp < TEMP_THRESHOLD_LOW:
                print(f"❄️  LOW TEMP ALERT: Node {node_id} at {temp}°C!")
            else:
                print(f"✅ Node {node_id}: {temp}°C (Normal)")
        
    except Exception as e:
        print(f"Error: {e}")

def main():
    print("Temperature alert monitor running...")
    
    try:
        while True:
            check_alerts()
            time.sleep(30)
    except KeyboardInterrupt:
        print("\nMonitor stopped")

if __name__ == "__main__":
    main()
```

---

## API Integration Tips

1. **Polling Interval**: Don't poll faster than every 5 seconds to avoid overloading the ESP8266
2. **Timeout**: Always use timeouts (5-10 seconds) when making HTTP requests
3. **Error Handling**: Check for HTTP errors and JSON parsing errors
4. **Data Age**: Check `lastUpdate` field to determine if data is stale (>60s)
5. **Node Count**: Gateway tracks up to `MAX_NODES` sensors (default 4, configurable)

---

## Cross-Origin Resource Sharing (CORS)

The gateway does not currently implement CORS headers. To access the API from a different domain:

**Option 1**: Use a proxy server  
**Option 2**: Add CORS headers to gateway firmware:

```cpp
// In Mesh_Network.ino, in the /api/sensors handler:
server.sendHeader("Access-Control-Allow-Origin", "*");
server.send(200, "application/json", output);
```

---

## Future API Enhancements

Potential additions for future versions:

- `GET /api/nodes` - List all mesh nodes (not just sensors)
- `GET /api/sensors/{nodeId}` - Get specific node data
- `GET /api/history` - Historical data (requires SD card)
- `POST /api/config` - Update configuration
- WebSocket support for real-time updates

---

For more information, see [README.md](README.md)
