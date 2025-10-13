# Tutorial 3: I2C Communication

## Introduction

I2C (Inter-Integrated Circuit) is a communication protocol that allows your ESP8266 to connect with multiple sensors and devices using just two wires. This tutorial covers I2C basics and working with common I2C devices.

## Prerequisites

- Completed previous tutorials
- ESP8266 with MicroPython installed
- Basic understanding of GPIO

## What is I2C?

I2C uses two wires for communication:
- **SDA** (Serial Data): Data transfer
- **SCL** (Serial Clock): Clock signal

**Advantages:**
- Multiple devices on same bus (up to 127)
- Only 2 wires needed
- Standardized protocol
- Built-in addressing

**On ESP8266:**
- Default SDA: D2 (GPIO4)
- Default SCL: D1 (GPIO5)

## Part 1: I2C Basics

### Scanning the I2C Bus

Find connected I2C devices:

```python
from machine import I2C, Pin

# Initialize I2C
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

# Scan for devices
devices = i2c.scan()

if devices:
    print(f"I2C devices found: {len(devices)}")
    for device in devices:
        print(f"  Address: {hex(device)}")
else:
    print("No I2C devices found!")
```

Common I2C addresses:
- 0x3C or 0x3D: SSD1306 OLED
- 0x76 or 0x77: BME280/BME680
- 0x40: INA219

### Configuring I2C

```python
from machine import I2C, Pin

# Standard configuration (400 kHz)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

# Slower for reliability (100 kHz)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=100000)

# Custom pins (if needed)
i2c = I2C(scl=Pin(14), sda=Pin(12), freq=400000)
```

## Part 2: SSD1306 OLED Display

### Hardware Setup

**Connections:**
```
SSD1306    Wemos D1 Mini
-------    -------------
VCC    →   3.3V
GND    →   GND
SDA    →   D2 (GPIO4)
SCL    →   D1 (GPIO5)
```

### Installing the Library

Download ssd1306.py and upload to ESP8266:
```bash
# Download
wget https://raw.githubusercontent.com/micropython/micropython/master/drivers/display/ssd1306.py

# Upload
ampy --port /dev/ttyUSB0 put ssd1306.py
```

### Basic Display Example

```python
from machine import I2C, Pin
import ssd1306
import time

# Initialize I2C and display
i2c = I2C(scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# Clear display
oled.fill(0)
oled.show()

# Display text
oled.text('Hello ESP8266!', 0, 0)
oled.text('MicroPython', 0, 10)
oled.show()

time.sleep(2)

# Draw shapes
oled.fill(0)
oled.rect(10, 10, 40, 20, 1)      # Rectangle
oled.fill_rect(60, 10, 40, 20, 1)  # Filled rectangle
oled.line(0, 40, 127, 40, 1)       # Line
oled.show()
```

### Display Functions

```python
# Text
oled.text('Hello', x, y)           # Display text at x, y

# Shapes
oled.pixel(x, y, color)            # Set pixel (0=off, 1=on)
oled.line(x1, y1, x2, y2, color)   # Draw line
oled.rect(x, y, w, h, color)       # Draw rectangle outline
oled.fill_rect(x, y, w, h, color)  # Draw filled rectangle

# Fill/Clear
oled.fill(0)                        # Clear display
oled.fill(1)                        # Fill display

# Update
oled.show()                         # Send buffer to display
```

### Scrolling Text Example

```python
from machine import I2C, Pin
import ssd1306
import time

i2c = I2C(scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

text = "ESP8266 + MicroPython = Awesome!"
text_width = len(text) * 8  # Each character is 8 pixels wide

for x in range(128, -text_width, -2):
    oled.fill(0)
    oled.text(text, x, 28)
    oled.show()
    time.sleep(0.05)
```

### Real-time Data Display

```python
from machine import I2C, Pin, ADC
import ssd1306
import time

i2c = I2C(scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)
adc = ADC(0)

while True:
    # Read sensor
    value = adc.read()
    voltage = value / 1023.0
    
    # Display
    oled.fill(0)
    oled.text('ADC Monitor', 20, 0)
    oled.text(f'Value: {value}', 0, 20)
    oled.text(f'Volt: {voltage:.2f}V', 0, 35)
    
    # Progress bar
    bar_width = int(value * 128 / 1023)
    oled.fill_rect(0, 50, bar_width, 10, 1)
    
    oled.show()
    time.sleep(0.5)
```

## Part 3: Environmental Sensors (BME280/BME680)

### Hardware Setup

**Connections:**
```
BME280     Wemos D1 Mini
-------    -------------
VCC    →   3.3V
GND    →   GND
SDA    →   D2 (GPIO4)
SCL    →   D1 (GPIO5)
```

### Installing Libraries

Download and upload:
- BME280: https://github.com/catdog2/mpy_bme280_esp8266
- BME680: https://github.com/robert-hh/BME680-Micropython

### BME280 Example

```python
from machine import I2C, Pin
import bme280
import time

i2c = I2C(scl=Pin(5), sda=Pin(4))
bme = bme280.BME280(i2c=i2c)

while True:
    temp, pressure, humidity = bme.read_compensated_data()
    
    # Convert to readable units
    temp_c = temp / 100
    pressure_hpa = pressure / 25600
    humidity_pct = humidity / 1024
    
    print(f"Temperature: {temp_c:.2f} °C")
    print(f"Pressure: {pressure_hpa:.2f} hPa")
    print(f"Humidity: {humidity_pct:.2f} %")
    print("-" * 40)
    
    time.sleep(2)
```

### Combined: Sensor + Display

```python
from machine import I2C, Pin
import ssd1306
import bme280
import time

# Initialize
i2c = I2C(scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)
bme = bme280.BME280(i2c=i2c)

while True:
    # Read sensor
    temp, pressure, humidity = bme.read_compensated_data()
    temp_c = temp / 100
    pressure_hpa = pressure / 25600
    humidity_pct = humidity / 1024
    
    # Display
    oled.fill(0)
    oled.text('Weather Station', 5, 0)
    oled.text(f'Temp: {temp_c:.1f}C', 0, 15)
    oled.text(f'Hum:  {humidity_pct:.1f}%', 0, 30)
    oled.text(f'Pres: {pressure_hpa:.0f}hPa', 0, 45)
    oled.show()
    
    time.sleep(2)
```

## Part 4: Multiple I2C Devices

Multiple devices can share the I2C bus:

```python
from machine import I2C, Pin
import ssd1306
import bme280
from ina219 import INA219
import time

# One I2C bus for all devices
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

# Initialize all devices
oled = ssd1306.SSD1306_I2C(128, 64, i2c)
bme = bme280.BME280(i2c=i2c)
ina = INA219(0.1, i2c, max_expected_amps=0.4)
ina.configure()

print("All I2C devices initialized!")
print(f"Addresses: {[hex(x) for x in i2c.scan()]}")

while True:
    # Read all sensors
    temp, pres, hum = bme.read_compensated_data()
    voltage = ina.voltage()
    current = ina.current()
    
    # Display on OLED
    oled.fill(0)
    oled.text(f'T:{temp/100:.1f}C H:{hum/1024:.0f}%', 0, 0)
    oled.text(f'V:{voltage:.2f}V', 0, 15)
    oled.text(f'I:{current:.0f}mA', 0, 30)
    oled.show()
    
    time.sleep(1)
```

## Part 5: Troubleshooting I2C

### Common Issues

#### No devices found
```python
# Check connections
# Try scanning with different frequency
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=100000)
devices = i2c.scan()
print(f"Found: {[hex(x) for x in devices]}")
```

#### Wrong address
```python
# Try both common addresses
addresses = [0x76, 0x77]  # For BME280/BME680
for addr in addresses:
    try:
        i2c.readfrom(addr, 1)
        print(f"Device found at {hex(addr)}")
    except:
        pass
```

#### Communication errors
- Add pull-up resistors (4.7kΩ) on SDA and SCL
- Reduce I2C frequency to 100kHz
- Check power supply (3.3V stable)
- Shorten wire lengths

### Testing I2C Devices

```python
from machine import I2C, Pin

i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

def test_i2c_device(addr):
    """Test if device responds at address"""
    try:
        i2c.readfrom(addr, 1)
        return True
    except:
        return False

# Test common addresses
common_addresses = {
    0x3C: "SSD1306 (alt 0x3D)",
    0x76: "BME280/BME680 (alt 0x77)",
    0x40: "INA219",
}

print("Scanning I2C bus...")
for addr, name in common_addresses.items():
    if test_i2c_device(addr):
        print(f"✓ {hex(addr)}: {name}")
    else:
        print(f"✗ {hex(addr)}: {name}")
```

## Practice Exercises

### Exercise 1: Temperature Logger
Display current and min/max temperatures on OLED.

### Exercise 2: Multi-screen Display
Create rotating screens showing different data.

### Exercise 3: Alert System
Flash LED when temperature exceeds threshold.

## Summary

You learned:
- ✓ I2C protocol basics
- ✓ Scanning I2C bus
- ✓ Using SSD1306 OLED display
- ✓ Reading environmental sensors
- ✓ Combining multiple I2C devices
- ✓ Troubleshooting I2C issues

## Next Steps

1. Try the [intermediate examples](../examples/intermediate/)
2. Build the [environmental monitor](../examples/advanced/environmental_monitor.py)
3. Add WiFi connectivity
4. Log data to SD card or cloud

## Resources

- [MicroPython I2C Documentation](https://docs.micropython.org/en/latest/library/machine.I2C.html)
- [I2C Protocol Tutorial](https://learn.sparkfun.com/tutorials/i2c)
- [Hardware Overview](../docs/hardware_overview.md)
- [Pin Reference](../docs/pin_reference.md)

Happy making! 🎉
