# MQTT Integration Guide

The Roosevelt Lake mesh network gateway can publish sensor data to an MQTT broker for integration with IoT platforms, home automation systems, and cloud services.

## Overview

When MQTT is enabled, the gateway publishes JSON-formatted sensor data to your MQTT broker whenever new readings arrive from mesh nodes. This enables:

- **Home Assistant** integration for dashboards and automations
- **Node-RED** flows for data processing
- **Cloud storage** for historical data logging
- **Mobile apps** with MQTT clients
- **Custom applications** via MQTT subscription

## Configuration

### 1. Enable MQTT in Gateway Firmware

Edit `Mesh_Network.ino` and configure these settings:

```cpp
// MQTT configuration
#define MQTT_ENABLED    true                      // Set to true to enable MQTT
#define MQTT_SERVER     "mqtt.example.com"        // Your MQTT broker hostname/IP
#define MQTT_PORT       1883                      // MQTT port (default 1883)
#define MQTT_TOPIC      "roosevelt/sensors"       // Topic to publish sensor data
```

### 2. Upload Updated Firmware

After editing, upload the firmware to your gateway ESP8266.

## MQTT Message Format

The gateway publishes JSON messages to the configured topic:

### Topic
```
roosevelt/sensors
```

### Payload Example
```json
{
  "sensors": [
    {
      "nodeId": 1234567890,
      "type": "BME280",
      "temperature": 25.5,
      "pressure": 1013.2,
      "humidity": 0.0,
      "gasResistance": 0.0
    },
    {
      "nodeId": 9876543210,
      "type": "BME680",
      "temperature": 26.3,
      "pressure": 1012.8,
      "humidity": 45.2,
      "gasResistance": 125000.0
    }
  ]
}
```

### Field Descriptions

- `nodeId`: Unique mesh node identifier
- `type`: Sensor type ("BME280" or "BME680")
- `temperature`: Temperature in degrees Celsius
- `pressure`: Atmospheric pressure in hPa
- `humidity`: Relative humidity percentage (0.0 for BME280)
- `gasResistance`: Gas resistance in Ohms (0.0 for BME280)

## MQTT Brokers

### Option 1: Cloud MQTT Brokers

**HiveMQ Cloud (Free Tier)**
```cpp
#define MQTT_SERVER     "your-cluster.hivemq.cloud"
#define MQTT_PORT       8883  // Use TLS
```

**CloudMQTT / Adafruit IO**
```cpp
#define MQTT_SERVER     "io.adafruit.com"
#define MQTT_PORT       1883
```

### Option 2: Local MQTT Broker

**Mosquitto (Raspberry Pi or PC)**
```cpp
#define MQTT_SERVER     "192.168.1.100"  // Your local broker IP
#define MQTT_PORT       1883
```

Install Mosquitto:
```bash
# On Raspberry Pi / Ubuntu
sudo apt install mosquitto mosquitto-clients

# Start service
sudo systemctl start mosquitto
sudo systemctl enable mosquitto
```

## Testing MQTT

### Subscribe to Messages

Use `mosquitto_sub` to verify messages are being published:

```bash
# Subscribe to the topic
mosquitto_sub -h mqtt.example.com -t "roosevelt/sensors" -v

# With authentication
mosquitto_sub -h mqtt.example.com -u username -P password -t "roosevelt/sensors" -v
```

### Monitor from Command Line

```bash
# Watch for new messages in real-time
mosquitto_sub -h localhost -t "roosevelt/#" -v

# Save messages to file
mosquitto_sub -h localhost -t "roosevelt/sensors" > sensor_log.txt
```

## Home Assistant Integration

Add sensor entities from MQTT data:

### Configuration (configuration.yaml)

```yaml
mqtt:
  sensor:
    - name: "Roosevelt Temp Node 1"
      state_topic: "roosevelt/sensors"
      value_template: "{{ value_json.sensors[0].temperature }}"
      unit_of_measurement: "°C"
      device_class: temperature
      
    - name: "Roosevelt Pressure Node 1"
      state_topic: "roosevelt/sensors"
      value_template: "{{ value_json.sensors[0].pressure }}"
      unit_of_measurement: "hPa"
      device_class: pressure
      
    - name: "Roosevelt Humidity BME680"
      state_topic: "roosevelt/sensors"
      value_template: >
        {% for sensor in value_json.sensors %}
          {% if sensor.type == "BME680" %}
            {{ sensor.humidity }}
          {% endif %}
        {% endfor %}
      unit_of_measurement: "%"
      device_class: humidity
```

### Automation Example

Trigger action when temperature exceeds threshold:

```yaml
automation:
  - alias: "High Temperature Alert"
    trigger:
      - platform: mqtt
        topic: "roosevelt/sensors"
    condition:
      - condition: template
        value_template: "{{ trigger.payload_json.sensors[0].temperature > 35 }}"
    action:
      - service: notify.mobile_app
        data:
          message: "High temperature detected at Roosevelt Lake!"
```

## Node-RED Flow

### Import This Flow

```json
[
  {
    "id": "mqtt_in",
    "type": "mqtt in",
    "topic": "roosevelt/sensors",
    "broker": "mqtt_broker",
    "name": "Roosevelt Sensors"
  },
  {
    "id": "json_parse",
    "type": "json",
    "name": "Parse JSON"
  },
  {
    "id": "function",
    "type": "function",
    "name": "Extract Temperature",
    "func": "msg.payload = msg.payload.sensors[0].temperature;\nreturn msg;"
  },
  {
    "id": "debug",
    "type": "debug",
    "name": "Display"
  }
]
```

### Flow Diagram

```
[MQTT In] → [JSON Parse] → [Function] → [Debug/Dashboard]
```

## Python MQTT Client

Subscribe and process sensor data with Python:

```python
#!/usr/bin/env python3
import paho.mqtt.client as mqtt
import json

MQTT_BROKER = "mqtt.example.com"
MQTT_PORT = 1883
MQTT_TOPIC = "roosevelt/sensors"

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    print(f"Received message on {msg.topic}")
    data = json.loads(msg.payload)
    
    for sensor in data['sensors']:
        print(f"Node {sensor['nodeId']} ({sensor['type']}):")
        print(f"  Temperature: {sensor['temperature']}°C")
        print(f"  Pressure: {sensor['pressure']} hPa")
        if sensor['humidity'] > 0:
            print(f"  Humidity: {sensor['humidity']}%")
        print()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_forever()
```

Run the script:
```bash
pip install paho-mqtt
python3 mqtt_subscriber.py
```

## Grafana Dashboard

### Store Data in InfluxDB

Use Telegraf to consume MQTT and write to InfluxDB:

**telegraf.conf:**
```toml
[[inputs.mqtt_consumer]]
  servers = ["tcp://localhost:1883"]
  topics = ["roosevelt/sensors"]
  data_format = "json"
  json_query = "sensors"
  tag_keys = ["nodeId", "type"]
  
[[outputs.influxdb_v2]]
  urls = ["http://localhost:8086"]
  token = "your-token"
  organization = "your-org"
  bucket = "roosevelt"
```

### Visualize in Grafana

1. Add InfluxDB data source
2. Create dashboard
3. Add panel with query:
   ```flux
   from(bucket: "roosevelt")
     |> range(start: -1h)
     |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer")
     |> filter(fn: (r) => r["_field"] == "temperature")
   ```

## Troubleshooting

### Gateway Not Connecting to MQTT

**Check Serial Monitor:**
```
Attempting MQTT connection...failed, rc=-2
```

**Common Issues:**
- `rc=-2`: Network connection failed (check broker address)
- `rc=-4`: Timeout (firewall blocking port?)
- `rc=5`: Authentication failed (check username/password)

**Solutions:**
1. Verify broker is reachable: `ping mqtt.example.com`
2. Check firewall allows port 1883
3. Test broker with: `mosquitto_pub -h mqtt.example.com -t test -m "hello"`
4. Enable authentication if required

### Messages Not Appearing

**Verify Publishing:**
1. Check Serial Monitor for "Published to MQTT"
2. Subscribe with mosquitto_sub to verify
3. Check topic name matches exactly

### Connection Drops

**Add Reconnection Logic:**
The gateway automatically reconnects every 5 seconds if disconnected.

**Monitor Connection:**
```cpp
if (!mqttClient.connected()) {
  Serial.println("MQTT disconnected, reconnecting...");
  reconnectMQTT();
}
```

## Security Considerations

### Use TLS/SSL

For production, enable TLS:

```cpp
WiFiClientSecure espClient;
espClient.setInsecure();  // Or use certificate
PubSubClient mqttClient(espClient);
```

### Authentication

Add username/password:

```cpp
mqttClient.connect(clientId.c_str(), "username", "password");
```

### Firewall Rules

Restrict MQTT port access to trusted IPs only.

## Advanced: Multiple Topics

Publish each sensor to individual topics:

```cpp
void publishToMQTT() {
  for (int i = 0; i < MAX_NODES; i++) {
    if (sensorNodes[i].hasData) {
      String topic = String(MQTT_TOPIC) + "/" + String(sensorNodes[i].nodeId);
      
      DynamicJsonDocument doc(256);
      doc["type"] = sensorNodes[i].nodeType;
      doc["temperature"] = sensorNodes[i].temperature;
      doc["pressure"] = sensorNodes[i].pressure;
      doc["humidity"] = sensorNodes[i].humidity;
      
      String output;
      serializeJson(doc, output);
      mqttClient.publish(topic.c_str(), output.c_str());
    }
  }
}
```

Topics become:
- `roosevelt/sensors/1234567890`
- `roosevelt/sensors/9876543210`

## Resources

- **MQTT.org**: https://mqtt.org/
- **Mosquitto**: https://mosquitto.org/
- **HiveMQ**: https://www.hivemq.com/
- **Home Assistant MQTT**: https://www.home-assistant.io/integrations/mqtt/
- **Node-RED**: https://nodered.org/

---

For more information, see [README.md](README.md)
