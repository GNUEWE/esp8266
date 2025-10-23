# ESP8266 Web Server

A WiFi-enabled web server for ESP8266 microcontrollers with automatic AP fallback.

## Features

- **WiFi Station Mode**: Connects to your existing WiFi network
- **AP Fallback**: Automatically creates an access point if WiFi connection fails
- **Web Server**: Serves HTML pages and SVG content
- **Multiple Routes**: Home page, About page, and logo endpoint
- **Responsive Design**: Mobile-friendly web interface

## Hardware Requirements

- ESP8266 module (NodeMCU, Wemos D1 Mini, or similar)
- USB cable for programming

## Software Requirements

- Arduino IDE with ESP8266 board support
- ESP8266WiFi library
- ESP8266WebServer library

## Installation

1. Open `esp8266_webserver.ino` in Arduino IDE
2. Update WiFi credentials in the sketch:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Select your ESP8266 board from Tools > Board
4. Select the correct COM port from Tools > Port
5. Upload the sketch to your ESP8266

## Usage

### Station Mode (Default)
When powered on, the ESP8266 attempts to connect to the configured WiFi network for 8 seconds. If successful, it prints the IP address to the Serial Monitor (115200 baud).

### Access Point Mode (Fallback)
If WiFi connection fails, the device automatically starts an access point:
- **SSID**: Offline-Selftour
- **Password**: 12345678
- **IP Address**: 192.168.4.1

### Accessing the Web Interface

Navigate to the device's IP address in a web browser:
- **Home**: `http://<device-ip>/`
- **About**: `http://<device-ip>/about`
- **Logo**: `http://<device-ip>/logo.svg`

## File Structure

- `esp8266_webserver.ino` - Main Arduino sketch with WiFi and server setup
- `pages.h` - HTML content for index and about pages
- `svg.h` - SVG logo graphic

## Configuration

### WiFi Settings
```cpp
const char* ssid = "BETSY";              // Your WiFi network name
const char* password = "joey1313";        // Your WiFi password
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
