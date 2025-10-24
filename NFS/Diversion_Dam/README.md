# Diversion Dam Weather Station

ESP8266-based weather station for Roosevelt Lake's Diversion Dam area with BME280 sensor integration.

## Features

- **Live Weather Data**: Temperature, humidity, and barometric pressure from BME280 I2C sensor
- **Auto-refresh**: Weather page updates every 30 seconds
- **Graceful Fallback**: Works without sensor, showing general weather information
- **Offline Portal**: Self-tour information accessible via WiFi AP
- **Mobile-Friendly**: Responsive design for all devices

## Hardware Setup

### Required
- ESP8266 (NodeMCU, Wemos D1 Mini, or similar)
- USB cable for programming

### Optional - For Weather Monitoring
- BME280 I2C sensor module

### Wiring (BME280)
```
BME280    ESP8266
------    -------
VCC   →   3.3V
GND   →   GND
SDA   →   GPIO4 (D2)
SCL   →   GPIO5 (D1)
```

## Software Dependencies

Install via Arduino IDE Library Manager:
- ESP8266WiFi (included with ESP8266 board package)
- ESP8266WebServer (included with ESP8266 board package)
- Adafruit BME280 Library
- Adafruit Unified Sensor Library

## Files

- `esp8266_webserver.ino` - Main sketch with WiFi, web server, and sensor initialization
- `pages.h` - HTML pages for home and activities/wildlife information
- `svg.h` - Roosevelt Lake logo SVG
- `weather.h` - BME280 sensor integration and weather page generation

## Web Pages

1. **Home** (`/`) - Welcome and location information
2. **Activities & Wildlife** (`/about`) - Recreation guide and wildlife info
3. **Weather** (`/weather`) - Live sensor readings and conditions
4. **Logo** (`/logo.svg`) - Roosevelt Lake scenic logo

## Configuration

### WiFi Settings
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### Access Point Fallback
```cpp
const char* ap_ssid = "Offline-Selftour";
const char* ap_password = "12345678";
```

## Usage

1. Connect BME280 sensor (optional)
2. Upload sketch to ESP8266
3. Device creates WiFi AP if it can't connect to configured network
4. Connect to "Offline-Selftour" network (password: 12345678)
5. Navigate to http://192.168.4.1
6. View live weather data on the Weather page

## Sensor Notes

- BME280 will be detected at I2C addresses 0x76 or 0x77
- If sensor is not connected, weather page shows general information
- Sensor readings update each time the weather page is loaded
- Page auto-refreshes every 30 seconds

## Weather Data Display

- **Temperature**: Shown in both Fahrenheit and Celsius
- **Humidity**: Relative humidity percentage
- **Pressure**: Barometric pressure in hPa and inHg
- **Recommendations**: Context-aware visitor guidance based on conditions
