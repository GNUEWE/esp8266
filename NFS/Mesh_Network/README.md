# Roosevelt Lake WiFi Mesh Network

A distributed sensor network using WiFi mesh technology to extend coverage across Roosevelt Lake and the Diversion Dam area. Multiple ESP8266 nodes communicate sensor data via JSON messages over a self-organizing mesh network.

## Overview

This mesh network solution enables:
- **Extended Range**: Mesh nodes relay data beyond single WiFi access point range
- **Multiple Sensors**: Support for BME280 (temp+pressure) and BME680 (full suite) sensors
- **Visual Displays**: OLED displays show real-time data from mesh
- **Self-Organizing**: Mesh automatically routes around failed nodes
- **JSON Communication**: Standardized JSON format for sensor data exchange

## Hardware Configuration

### 4-Node Mesh Setup

**Node 1: Gateway/Root Node**
- ESP8266 Wemos D1 R2
- Firmware: `Mesh_Network.ino`
- Role: Web server, data aggregation, mesh root
- No sensors needed

**Node 2: BME280 Sensor Node**
- ESP8266 Wemos D1 R2
- BME280 sensor (temperature + pressure only)
- Firmware: `Sensor_Node_BME280.ino`
- Wiring: SDA=GPIO4 (D2), SCL=GPIO5 (D1)

**Node 3: BME680 Sensor Node**
- ESP8266 Wemos D1 R2
- BME680 sensor (temperature + pressure + humidity + gas)
- Firmware: `Sensor_Node_BME680.ino`
- Wiring: SDA=GPIO4 (D2), SCL=GPIO5 (D1)

**Node 4: OLED Display Node**
- ESP8266 Wemos D1 R2
- SSD1306 OLED 128x64 display
- Firmware: `Display_Node_OLED.ino`
- Wiring: SDA=GPIO4 (D2), SCL=GPIO5 (D1)

### Additional Nodes (Optional)

You can add more BME280 sensor nodes or OLED display nodes to extend coverage:
- Additional BME280 nodes use `Sensor_Node_BME280.ino`
- Additional OLED displays use `Display_Node_OLED.ino`

## I2C Wiring

All sensors and displays use I2C bus on standard ESP8266 pins:

```
Component Pin → ESP8266 Pin
VCC/VIN       → 3.3V
GND           → GND
SDA           → GPIO4 (D2 on Wemos D1)
SCL           → GPIO5 (D1 on Wemos D1)
```

**Important Notes:**
- BME280 sensors may be at I2C address 0x76 or 0x77
- BME680 sensors typically at I2C address 0x77
- SSD1306 OLED displays typically at I2C address 0x3C
- All devices can share the same I2C bus

## Software Requirements

### Arduino IDE Libraries

Install these libraries via Arduino IDE Library Manager:

**Required for All Nodes:**
1. `painlessMesh` by Coopdis - Mesh networking
2. `ArduinoJson` by Benoit Blanchon - JSON parsing
3. `TaskScheduler` - (dependency of painlessMesh)

**For BME280 Nodes:**
4. `BME280` by Tyler Glenn - BME280/BMP280 sensor support

**For BME680 Nodes:**
5. `Adafruit BME680 Library` - BME680 sensor support
6. `Adafruit Unified Sensor` - (dependency)

**For Display Nodes:**
7. `Adafruit SSD1306` - OLED display driver
8. `Adafruit GFX Library` - Graphics library

### ESP8266 Board Support

Ensure ESP8266 board support is installed in Arduino IDE:
- Go to File → Preferences
- Add to Additional Board Manager URLs: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- Go to Tools → Board → Boards Manager
- Install "esp8266" by ESP8266 Community

## Installation Instructions

### 1. Configure Mesh Network

All nodes must use the same mesh credentials (already configured in code):
```cpp
#define MESH_SSID       "RooseveltMesh"
#define MESH_PASSWORD   "meshpass123"
#define MESH_PORT       5555
```

### 2. Program Each Node

**Gateway Node:**
1. Open `Mesh_Network.ino` in Arduino IDE
2. Select Board: "LOLIN(WEMOS) D1 R2 & mini"
3. Select correct COM port
4. Upload

**BME280 Sensor Nodes (x3):**
1. Wire BME280 sensor to ESP8266
2. Open `Sensor_Node_BME280.ino`
3. Upload to each ESP8266 with BME280

**BME680 Sensor Node:**
1. Wire BME680 sensor to ESP8266
2. Open `Sensor_Node_BME680.ino`
3. Upload

**OLED Display Nodes (x2):**
1. Wire SSD1306 OLED to ESP8266
2. Open `Display_Node_OLED.ino`
3. Upload to each ESP8266 with display

### 3. Deploy Nodes

Position nodes throughout the area:
- Gateway node: Central location, accessible to visitors
- Sensor nodes: Different areas to monitor (e.g., dam, campground, hiking trails)
- Display nodes: High-traffic visitor areas

Mesh will automatically form as nodes power on. Nodes should be within WiFi range of at least one other node.

## Usage

### Accessing the Web Portal

1. The gateway node creates a WiFi access point:
   - **SSID**: Based on painlessMesh (check Serial Monitor)
   - **IP Address**: Typically `192.168.4.1` or displayed in Serial Monitor

2. Connect to the mesh gateway WiFi network

3. Open web browser and navigate to the gateway IP address

4. View real-time data from all mesh sensor nodes

### JSON Data Format

Nodes communicate using this JSON structure:

**BME280 Message:**
```json
{
  "type": "BME280",
  "temperature": 25.5,
  "pressure": 1013.2,
  "humidity": 0.0,
  "gas": 0.0
}
```

**BME680 Message:**
```json
{
  "type": "BME680",
  "temperature": 26.3,
  "pressure": 1012.8,
  "humidity": 45.2,
  "gas": 125000.0
}
```

### REST API

The gateway provides a JSON API endpoint:

**GET /api/sensors**
```json
{
  "sensors": [
    {
      "nodeId": 123456789,
      "type": "BME680",
      "temperature": 26.3,
      "pressure": 1012.8,
      "humidity": 45.2,
      "gasResistance": 125000.0,
      "lastUpdate": 5234
    }
  ]
}
```

## Monitoring and Debugging

### Serial Monitor

Each node outputs debug information at 115200 baud:
- Node ID on startup
- Connection status
- Sensor readings
- Messages sent/received

**Gateway Node Serial Output:**
```
Mesh Gateway Node Initialized
Node ID: 3847562910
HTTP Server Started on Port 80
New Connection: 1234567890
Received from 1234567890: {"type":"BME280","temperature":25.5...}
```

**Sensor Node Serial Output:**
```
BME280 sensor found
BME280 Mesh Sensor Node Initialized
Node ID: 1234567890
Sent: T=25.5°C, P=1013.2hPa
```

### Troubleshooting

**Nodes Not Connecting to Mesh:**
- Verify all nodes use same MESH_SSID, MESH_PASSWORD, and MESH_PORT
- Check nodes are within WiFi range of each other
- Power cycle all nodes
- Check Serial Monitor for error messages

**Sensor Not Detected:**
- Verify I2C wiring (SDA to D2, SCL to D1, power and ground)
- Check I2C address with I2C scanner sketch
- Ensure proper 3.3V power supply
- Try different I2C pull-up resistors if needed

**Display Shows "Waiting for Data":**
- Verify sensor nodes are powered and connected to mesh
- Check Serial Monitor of sensor nodes for transmit confirmation
- Display will show "Waiting" if no sensor data received in 2 minutes

**Web Server Not Accessible:**
- Verify gateway node Serial Monitor for IP address
- Check WiFi connection to gateway
- Try both `http://192.168.4.1` and the IP shown in Serial Monitor
- Disable phone's cellular data when connected to mesh WiFi

## Mesh Network Details

### How It Works

1. **Self-Organization**: Nodes automatically discover neighbors and create routing tables
2. **Message Routing**: Messages hop through intermediate nodes to reach destination
3. **Broadcast**: Sensor data is broadcast to all nodes (gateway listens)
4. **Time Sync**: Mesh maintains synchronized time across all nodes
5. **Healing**: Network automatically routes around failed/offline nodes

### Network Topology

The mesh can form various topologies:
```
        [Gateway]
         /  |  \
        /   |   \
   [BME280][BME680][Display]
                    |
               [BME280]
```

Or:
```
[Display] - [BME280] - [Gateway] - [BME680] - [Display]
                                        |
                                    [BME280]
```

The actual topology depends on physical node placement and WiFi signal strength.

### Range Extension

Theoretical maximum:
- Each node extends range by ~50-100m outdoors
- 4 nodes could cover up to 200-400m in a line
- Actual range depends on terrain, obstacles, and interference

## Configuration Options

### Sensor Reading Interval

Adjust how often sensors send data (in sensor node firmware):
```cpp
#define SENSOR_INTERVAL 30000  // 30 seconds (default)
```

### Display Update Interval

Adjust display refresh rate (in display node firmware):
```cpp
#define DISPLAY_INTERVAL 5000  // 5 seconds (default)
```

### Maximum Sensor Nodes

Adjust storage in gateway firmware:
```cpp
#define MAX_NODES 4  // Default: 4 nodes
```

## Power Considerations

### Power Options

- **USB Power**: 5V USB adapter or power bank
- **Solar**: 5V solar panel + battery for remote locations
- **Battery**: Rechargeable battery pack (2000+ mAh recommended)

### Power Consumption

Approximate current draw at 5V:
- ESP8266: ~80mA active, ~15mA idle
- BME280: ~3.6µA
- BME680: ~3.7µA (with gas measurement)
- SSD1306 OLED: ~20mA (display on)

Total per node: ~80-100mA active

### Battery Life Estimation

With 2000mAh battery:
- Continuous operation: ~20-25 hours
- With deep sleep (not implemented): Days to weeks possible

## Future Enhancements

Potential improvements:
- [ ] Deep sleep mode for battery-powered nodes
- [ ] Data logging to SD card on gateway
- [ ] MQTT bridge for cloud connectivity
- [ ] OTA (Over-The-Air) firmware updates
- [ ] Web interface for mesh configuration
- [ ] Signal strength (RSSI) monitoring
- [ ] GPS coordinates for node locations
- [ ] Historical data graphing

## Technical Specifications

**Mesh Library**: painlessMesh
- Uses ESP8266 WiFi in mesh mode
- Automatic node discovery
- Self-healing network
- Message routing and broadcast
- Time synchronization

**Communication Protocol**:
- JSON message format
- Broadcast communication (one-to-all)
- 30-second sensor update interval
- Sub-second message latency

**Web Server**:
- ESP8266WebServer
- HTML5 responsive design
- JSON REST API
- Auto-refresh every 30 seconds

## Credits

- **painlessMesh Library**: https://gitlab.com/painlessMesh/painlessMesh
- **BME280 Library**: Tyler Glenn
- **Adafruit Sensor Libraries**: Adafruit Industries
- **ArduinoJson**: Benoit Blanchon

## License

This project is open source and available for modification and distribution.
