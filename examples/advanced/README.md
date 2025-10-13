# Advanced Examples

This directory contains advanced-level examples that combine multiple sensors and components into complete projects.

## Examples

### 1. environmental_monitor.py
A comprehensive environmental monitoring station with OLED UI.

**What you'll learn:**
- Combining multiple I2C devices
- Building user interfaces
- Multi-screen display management
- Real-time data visualization
- Data logging and graphing

**Features:**
- Real-time environmental data display (temp, humidity, pressure)
- Distance monitoring with visual indicators
- Power consumption tracking
- Temperature graphing
- Automatic screen rotation
- Error handling for missing sensors

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64)
- BME280 or BME680 Environmental Sensor
- HC-SR04 Ultrasonic Sensor (bridged)
- INA219 Current Sensor (optional)
- Jumper wires

**Required libraries:**
- ssd1306.py
- bme280.py or bme680.py
- ina219.py (optional)

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
1. Install required libraries on your ESP8266
2. Connect all hardware components
3. Upload the example script
4. Run and enjoy!

### Customization

The environmental monitor is designed to be modular. You can:
- Enable/disable specific sensors
- Adjust screen rotation timing
- Modify display layouts
- Add new sensor types
- Change graph parameters

Example customization:
```python
# Change screen duration
SCREEN_DURATION = 5000  # 5 seconds

# Disable power monitoring screen
screens = [draw_screen_0, draw_screen_1, draw_screen_3]

# Adjust graph history
MAX_HISTORY = 64
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
- Handle exceptions gracefully
- Use async patterns for better responsiveness
- Consider deep sleep for battery-powered projects

## Troubleshooting

### Display Issues
- Verify I2C address (0x3C or 0x3D for SSD1306)
- Check if display is 128x64 or 128x32
- Reduce I2C frequency if communication fails

### Multiple I2C Devices
- All devices share SDA and SCL
- Each device must have unique address
- Use `i2c.scan()` to verify addresses
- Add pull-up resistors (4.7kΩ) if needed

### Memory Issues
- ESP8266 has limited RAM (~36KB free)
- Use smaller history buffers
- Free unused variables
- Consider splitting into modules

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

- [ESP8266 MicroPython Docs](https://docs.micropython.org/en/latest/esp8266/)
- [SSD1306 Display Guide](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)
- [BME280 Tutorial](https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout)
- [BME680 Tutorial](https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas)
