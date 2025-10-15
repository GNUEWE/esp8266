# Intermediate Examples

This directory contains intermediate-level Arduino sketches for those familiar with basic GPIO operations on ESP8266.

## Examples

### 1. i2c_scanner
**I2C Scanner Tool with Optional SSD1306 Display**

Scan the I2C bus for connected devices and display results on Serial Monitor and optionally on an SSD1306 OLED display.

**Folder:** [i2c_scanner/](i2c_scanner/)  
**File:** i2c_scanner.ino

**What you'll learn:**
- I2C bus scanning
- Device identification
- Optional display output
- I2C troubleshooting

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- Optional: SSD1306 OLED Display

**Libraries required:**
- Wire (built-in)
- Adafruit GFX Library (if using display)
- Adafruit SSD1306 Library (if using display)

---

### 2. ssd1306_display
Using I2C to communicate with an SSD1306 OLED display.

**Folder:** [ssd1306_display/](ssd1306_display/)  
**File:** ssd1306_display.ino

**What you'll learn:**
- I2C communication protocol
- Working with displays
- Drawing text, shapes, and animations
- Display buffer management

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64 or 128x32)

**Libraries required:**
- Adafruit GFX Library
- Adafruit SSD1306 Library

---

### 3. bme280_sensor
Reading temperature, humidity, and pressure from a BME280 sensor.

**Folder:** [bme280_sensor/](bme280_sensor/)  
**File:** bme280_sensor.ino

**What you'll learn:**
- Working with environmental sensors
- I2C sensor communication
- Data conversion
- Altitude calculation

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- BME280 Environmental Sensor

**Libraries required:**
- Adafruit BME280 Library
- Adafruit Unified Sensor Library

---

### 4. bme680_sensor
Reading temperature, humidity, pressure, and gas resistance from a BME680 sensor.

**Folder:** [bme680_sensor/](bme680_sensor/)  
**File:** bme680_sensor.ino

**What you'll learn:**
- Advanced environmental sensing
- Gas sensor operation
- Air quality monitoring
- Sensor configuration

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- BME680 Environmental Sensor

**Libraries required:**
- Adafruit BME680 Library
- Adafruit Unified Sensor Library

---

### 5. ina219_sensor
Measuring voltage, current, and power with an INA219 sensor.

**Folder:** [ina219_sensor/](ina219_sensor/)  
**File:** ina219_sensor.ino

**What you'll learn:**
- Current sensing
- Power monitoring
- Energy calculations

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- INA219 Current/Power Sensor
- Load to measure

**Libraries required:**
- Adafruit INA219 Library

---

### 6. hcsr04_bridged
Measuring distance with HC-SR04 ultrasonic sensor (bridged echo/trigger).

**Folder:** [hcsr04_bridged/](hcsr04_bridged/)  
**File:** hcsr04_bridged.ino

**What you'll learn:**
- Ultrasonic ranging
- Timing critical operations
- GPIO mode switching
- Special bridged configuration

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- HC-SR04 Ultrasonic Sensor (with echo and trigger bridged)

---

## Prerequisites

Before trying these examples:
1. Complete all basic examples
2. Understand GPIO and timing
3. Install required libraries via Arduino IDE Library Manager
4. Have basic electronics knowledge

## I2C Connections

All I2C devices connect to:
- **SDA** → D2 (GPIO4)
- **SCL** → D1 (GPIO5)
- **VCC** → 3.3V
- **GND** → GND

## Installing Libraries

Install libraries via Arduino IDE:
1. Go to Tools → Manage Libraries
2. Search for library name
3. Click Install

**Required libraries:**
- Adafruit GFX Library
- Adafruit SSD1306
- Adafruit BME280 Library
- Adafruit BME680 Library
- Adafruit INA219
- Adafruit Unified Sensor

## Common Issues

### I2C Not Working
- Check connections (SDA, SCL)
- Use I2C scanner to verify device address
- Ensure proper power supply (3.3V or 5V as required)

### Compilation Errors
- Install required libraries
- Update ESP8266 board package
- Check Arduino IDE version (1.8.x or 2.x)

### Upload Failures
- Select correct board and port
- Try different upload speed (115200)
- Press reset button before upload

## Next Steps

Once comfortable with these examples:
1. Try combining multiple sensors
2. Move to advanced examples
3. Build your own projects!

## Resources

- [ESP8266 Arduino Core Documentation](https://arduino-esp8266.readthedocs.io/)
- [Adafruit Learning System](https://learn.adafruit.com/)
