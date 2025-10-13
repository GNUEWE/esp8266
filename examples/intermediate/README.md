# Intermediate Examples

This directory contains intermediate-level examples for those familiar with basic GPIO operations on ESP8266.

## Examples

### 1. ssd1306_display.py
Using I2C to communicate with an SSD1306 OLED display.

**What you'll learn:**
- I2C communication protocol
- Working with displays
- Drawing text, shapes, and animations
- Display buffer management

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64 or 128x32)
- Jumper wires

**Required library:** ssd1306.py

---

### 2. bme280_sensor.py
Reading temperature, humidity, and pressure from a BME280 sensor.

**What you'll learn:**
- Working with environmental sensors
- I2C sensor communication
- Data conversion and calibration
- Error handling

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- BME280 Environmental Sensor
- Jumper wires

**Required library:** bme280.py

---

### 3. bme680_sensor.py
Reading temperature, humidity, pressure, and gas resistance from a BME680 sensor.

**What you'll learn:**
- Advanced environmental sensing
- Gas sensor operation
- Air quality monitoring
- Sensor configuration

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- BME680 Environmental Sensor
- Jumper wires

**Required library:** bme680.py

---

### 4. ina219_sensor.py
Measuring voltage, current, and power with an INA219 sensor.

**What you'll learn:**
- Current sensing
- Power monitoring
- Energy calculations
- Shunt resistor concepts

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- INA219 Current/Power Sensor
- Jumper wires
- Load to measure

**Required library:** ina219.py

---

### 5. hcsr04_bridged.py
Measuring distance with HC-SR04 ultrasonic sensor (bridged echo/trigger).

**What you'll learn:**
- Ultrasonic ranging
- Timing critical operations
- GPIO mode switching
- Special bridged configuration

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- HC-SR04 Ultrasonic Sensor (with echo and trigger bridged)
- Jumper wires

---

## Prerequisites

Before trying these examples:
1. Complete all basic examples
2. Understand GPIO and timing
3. Be comfortable with Python classes
4. Have basic electronics knowledge

## I2C Connections

All I2C devices connect to:
- **SDA** → D2 (GPIO4)
- **SCL** → D1 (GPIO5)
- **VCC** → 3.3V
- **GND** → GND

## Required Libraries

Most examples require external libraries. Download and upload them to your ESP8266:

- **ssd1306.py**: https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py
- **bme280.py**: https://github.com/catdog2/mpy_bme280_esp8266
- **bme680.py**: https://github.com/robert-hh/BME680-Micropython
- **ina219.py**: https://github.com/chrisb2/pyb_ina219

Upload libraries using:
```bash
ampy --port /dev/ttyUSB0 put ssd1306.py
```

## Common Issues

### I2C Not Working
- Check connections (SDA, SCL)
- Verify device address (use `i2c.scan()`)
- Some sensors work better at 100kHz (try `freq=100000`)
- Ensure proper power supply (3.3V or 5V as required)

### Sensor Reading Errors
- Verify power supply voltage
- Check wiring
- Add delays between readings
- Handle exceptions properly

### HC-SR04 Issues
- Use 5V power if available
- Ensure trigger/echo pins are properly bridged
- Check for timeout errors
- Keep sensor stable during measurement

## Next Steps

Once comfortable with these examples:
1. Try combining multiple sensors
2. Add data logging
3. Move to advanced examples
4. Build your own projects!

## Resources

- [ESP8266 I2C Documentation](https://docs.micropython.org/en/latest/esp8266/quickref.html#i2c-bus)
- [MicroPython Documentation](https://docs.micropython.org/)
