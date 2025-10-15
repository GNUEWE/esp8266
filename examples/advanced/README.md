# Advanced Examples

This directory contains advanced-level Arduino sketches that combine multiple sensors and components into complete projects.

## Examples

### 1. environmental_monitor
A comprehensive environmental monitoring station with OLED UI.

**Folder:** [environmental_monitor/](environmental_monitor/)  
**File:** environmental_monitor.ino

**What you'll learn:**
- Combining multiple I2C devices
- Building user interfaces
- Multi-screen display management
- Real-time data visualization
- Data logging and graphing

**Features:**
- Real-time environmental data display (temp, humidity, pressure)
- Distance monitoring with visual indicators
- Power consumption tracking (if INA219 present)
- Temperature graphing
- Automatic screen rotation
- Modular design - works with partial hardware

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64)
- BME280 or BME680 Environmental Sensor
- HC-SR04 Ultrasonic Sensor (bridged)
- INA219 Current Sensor (optional)

**Libraries required:**
- Adafruit GFX Library
- Adafruit SSD1306 Library
- Adafruit BME280 Library (or BME680)
- Adafruit INA219 Library (optional)
- Adafruit Unified Sensor Library

**Connections:**

I2C Devices (share the bus):
- SDA → D2 (GPIO4)
- SCL → D1 (GPIO5)
- VCC → 3.3V
- GND → GND

HC-SR04:
- TRIG/ECHO (bridged) → D7 (GPIO13)
- VCC → 5V
- GND → GND

---

## Using the Examples

### Setup
1. Install required libraries via Arduino IDE Library Manager
2. Connect all hardware components
3. Open the sketch in Arduino IDE
4. Select board and port
5. Upload and enjoy!

### Customization

The environmental monitor is designed to be modular. You can:
- Enable/disable specific sensors (code will detect automatically)
- Adjust screen rotation timing
- Modify display layouts
- Add new sensor types
- Change graph parameters

Example customization:
```cpp
// Change screen duration
const unsigned long SCREEN_DURATION = 5000;  // 5 seconds

// Adjust graph history
const int MAX_HISTORY = 64;
```

## Project Ideas

Based on these examples, you can create:

1. **Weather Station**
   - Add WiFi connectivity
   - Upload data to cloud
   - Create web dashboard

2. **Parking Sensor**
   - Use ultrasonic for distance
   - Visual/audio alerts
   - LED distance indicators

3. **Power Monitor**
   - Track battery usage
   - Calculate runtime
   - Efficiency monitoring

4. **Air Quality Monitor**
   - BME680 for air quality
   - Display warnings
   - Data logging to SD card

5. **Smart Display**
   - Show IoT data
   - Weather forecasts
   - Time and date

## Performance Tips

- Use appropriate I2C frequency (100kHz for reliability, 400kHz for speed)
- Add delays between sensor readings
- Handle errors gracefully
- Consider deep sleep for battery-powered projects

## Troubleshooting

### Display Issues
- Verify I2C address (0x3C or 0x3D for SSD1306)
- Check if display is 128x64 or 128x32
- Reduce I2C frequency if communication fails

### Multiple I2C Devices
- All devices share SDA and SCL
- Each device must have unique address
- Use I2C scanner to verify addresses

### Memory Issues
- ESP8266 has limited RAM
- Use smaller history buffers
- Split into functions
- Consider using PROGMEM for constants

### Power Issues
- Ensure adequate power supply
- Some sensors need 5V (HC-SR04)
- Others need 3.3V (I2C devices)
- Use appropriate power rails

## Next Steps

1. Modify examples to suit your needs
2. Add WiFi connectivity for IoT
3. Implement data logging
4. Create custom UI themes
5. Share your projects!

## Resources

- [ESP8266 Arduino Core Documentation](https://arduino-esp8266.readthedocs.io/)
- [Adafruit Learning System](https://learn.adafruit.com/)
