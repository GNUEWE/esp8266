# Roosevelt Lake Offline Self-Tour Portal

An ESP8266-powered offline information portal for Roosevelt Lake and the Diversion Dam area in Arizona. Perfect for providing visitors with information about the area when internet connectivity is unavailable.

## Features

- **Offline Access**: Works completely offline via WiFi access point
- **WiFi Fallback**: Attempts to connect to existing WiFi, then creates its own AP
- **Weather Monitoring**: Real-time weather data from BME280 sensor (temperature, humidity, pressure)
- **Comprehensive Information**: Details about Roosevelt Lake, Diversion Dam, activities, and wildlife
- **Self-Guided Tour**: Learn about hiking, fishing, kayaking, and local wildlife
- **Mobile-Friendly**: Responsive design works on all devices
- **Low Power**: Runs on ESP8266 microcontroller

## Hardware Requirements

- ESP8266 module (NodeMCU, Wemos D1 Mini, or similar)
- BME280 I2C sensor (optional, for weather data)
- USB cable for programming

### BME280 Wiring (Optional)

Connect the BME280 sensor to your ESP8266:
- VCC → 3.3V
- GND → GND
- SDA → GPIO4 (D2 on NodeMCU/Wemos D1 Mini)
- SCL → GPIO5 (D1 on NodeMCU/Wemos D1 Mini)

*Note: The weather page will work without the sensor, showing general weather information instead of live readings.*

## Software Requirements

- Arduino IDE with ESP8266 board support
- ESP8266WiFi library (included with ESP8266 board package)
- ESP8266WebServer library (included with ESP8266 board package)
- BME280 library by Tyler Glenn (minimal dependencies)

## Installation

1. Install required libraries in Arduino IDE:
   - Go to Sketch → Include Library → Manage Libraries
   - Search and install: "BME280" by Tyler Glenn
   - This library was chosen for its simplicity and minimal dependency footprint

2. Open `NFS/Diversion_Dam/esp8266_webserver.ino` in Arduino IDE

3. Update WiFi credentials in the sketch:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

4. Select your ESP8266 board from Tools > Board

5. Select the correct COM port from Tools > Port

6. Upload the sketch to your ESP8266

## Usage

### Deployment
Place the ESP8266 device at Roosevelt Lake near the Diversion Dam area. The device will create a WiFi access point that visitors can connect to for offline access to tour information.

### Station Mode (Default)
When powered on, the ESP8266 attempts to connect to a configured WiFi network for 8 seconds. If successful, it prints the IP address to the Serial Monitor (115200 baud).

### Access Point Mode (Primary Use Case)
If WiFi connection fails (or no network is available), the device automatically starts an access point:
- **SSID**: Offline-Selftour
- **Password**: 12345678
- **IP Address**: 192.168.4.1

### Accessing the Tour Information

Visitors connect to the "Offline-Selftour" WiFi network, then navigate to `http://192.168.4.1` in any web browser:
- **Home**: Information about Roosevelt Lake and Diversion Dam
- **Activities & Wildlife**: Detailed guide to fishing, hiking, kayaking, and local wildlife
- **Weather**: Live weather conditions from BME280 sensor (temp, humidity, pressure)
- **Logo**: Roosevelt Lake scenic logo

## Content

### Pages Included

1. **Home Page** - Welcome and overview of Roosevelt Lake and Diversion Dam
   - Location and elevation information
   - History of the Diversion Dam
   - Natural beauty description

2. **Activities & Wildlife Page** - Comprehensive recreation guide
   - **Fishing**: Bass, crappie, catfish - Roosevelt is a premier fishing destination
   - **Hiking**: Desert trails with lake views
   - **Kayaking & Rafting**: Calm waters perfect for paddling
   - **Camping**: Information about local campgrounds
   - **Wildlife**: Bald eagles, bighorn sheep, desert animals, and plants
   - **Safety Tips**: Desert safety and best visiting times

3. **Weather Page** - Live environmental conditions
   - **Temperature**: Real-time readings in °F and °C
   - **Humidity**: Current relative humidity percentage
   - **Pressure**: Barometric pressure in hPa and inHg
   - **Conditions**: Weather interpretation and visitor recommendations
   - **Auto-refresh**: Updates every 30 seconds with latest sensor data

## File Structure

```
esp8266/
├── README.md
└── NFS/                          # National Forest Service projects
    └── Diversion_Dam/            # Roosevelt Lake Diversion Dam project
        ├── esp8266_webserver.ino # Main Arduino sketch
        ├── pages.h               # HTML content for home and activities pages
        ├── svg.h                 # Roosevelt Lake scenic logo
        └── weather.h             # BME280 sensor integration and weather page
```

Each `.ino` project is organized in its own folder under `NFS/` for National Forest Service locations.

## Configuration

### WiFi Settings
```cpp
const char* ssid = "BWEEFEE";              // Your WiFi network name
const char* password = "12345678";        // Your WiFi password
```

### AP Fallback Settings
```cpp
const char* ap_ssid = "Offline-Selftour";     // AP name
const char* ap_password = "12345678";          // AP password (min 8 chars)
```

## Troubleshooting

- **Cannot connect to WiFi**: Check SSID and password are correct
- **Cannot find device**: Check Serial Monitor for IP address
- **Upload fails**: Ensure correct board and port are selected
- **Access Point not visible**: Check AP credentials and ensure device has power

## License

This project is open source and available for modification and distribution.
