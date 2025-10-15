# Libraries

This directory contains helper libraries and utilities for ESP8266 MicroPython development.

## Required External Libraries

Most examples in this repository require external libraries that need to be installed on your ESP8266.

### Display Libraries

#### ssd1306.py
OLED display driver for SSD1306-based displays.

**Download:**
- Official: https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py
- Direct link: https://raw.githubusercontent.com/micropython/micropython/master/drivers/display/ssd1306.py

**Install:**
```bash
ampy --port /dev/ttyUSB0 put ssd1306.py
```

**Usage:**
```python
from machine import I2C, Pin
import ssd1306

i2c = I2C(scl=Pin(5), sda=Pin(4))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)
oled.text('Hello!', 0, 0)
oled.show()
```

### Sensor Libraries

#### bme280.py
Environmental sensor driver (temperature, humidity, pressure).

**Download:**
- https://github.com/catdog2/mpy_bme280_esp8266

**Install:**
```bash
ampy --port /dev/ttyUSB0 put bme280.py
```

**Usage:**
```python
from machine import I2C, Pin
import bme280

i2c = I2C(scl=Pin(5), sda=Pin(4))
bme = bme280.BME280(i2c=i2c)
temp, pressure, humidity = bme.read_compensated_data()
```

#### bme680.py
Advanced environmental sensor with air quality (VOC) measurement.

**Download:**
- https://github.com/robert-hh/BME680-Micropython

**Install:**
```bash
ampy --port /dev/ttyUSB0 put bme680.py
```

**Usage:**
```python
from machine import I2C, Pin
import bme680

i2c = I2C(scl=Pin(5), sda=Pin(4))
bme = bme680.BME680_I2C(i2c=i2c)
if bme.get_sensor_data():
    print(bme.data.temperature)
    print(bme.data.humidity)
    print(bme.data.gas_resistance)
```

#### ina219.py
Current and power sensor driver.

**Download:**
- https://github.com/chrisb2/pyb_ina219

**Install:**
```bash
ampy --port /dev/ttyUSB0 put ina219.py
```

**Usage:**
```python
from machine import I2C, Pin
from ina219 import INA219

i2c = I2C(scl=Pin(5), sda=Pin(4))
ina = INA219(0.1, i2c, max_expected_amps=0.4)
ina.configure()
print(f'Voltage: {ina.voltage()}V')
print(f'Current: {ina.current()}mA')
```

## Installation Methods

### Method 1: Using ampy
```bash
# Install ampy
pip install adafruit-ampy

# Upload single file
ampy --port /dev/ttyUSB0 put library.py

# Upload directory
ampy --port /dev/ttyUSB0 put libs/ /
```

### Method 2: Using mpremote
```bash
# Upload file
mpremote connect /dev/ttyUSB0 cp library.py :

# Upload multiple files
mpremote connect /dev/ttyUSB0 cp file1.py file2.py :
```

### Method 3: Using Thonny IDE
1. Open the library file in Thonny
2. File → Save As
3. Select "MicroPython device"
4. Save to root directory

### Method 4: Using WebREPL
1. Enable WebREPL on ESP8266
2. Connect via web browser
3. Upload files through web interface

## Verifying Installation

Check installed files:
```python
import os
print(os.listdir('/'))
```

Test library import:
```python
try:
    import ssd1306
    print("ssd1306 installed ✓")
except ImportError:
    print("ssd1306 not found ✗")

try:
    import bme280
    print("bme280 installed ✓")
except ImportError:
    print("bme280 not found ✗")
```

## Pre-compiled Libraries (.mpy)

To save memory, compile libraries to bytecode:

```bash
# Install mpy-cross
pip install mpy-cross

# Compile library
mpy-cross library.py

# Upload .mpy file instead of .py
ampy --port /dev/ttyUSB0 put library.mpy
```

Benefits:
- Faster import
- Less RAM usage
- Smaller file size

## Custom Helper Libraries

You can create your own helper libraries and place them in this directory.

Example: `helpers.py`
```python
"""Helper functions for common tasks"""

def map_range(value, in_min, in_max, out_min, out_max):
    """Map value from one range to another"""
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def clamp(value, min_val, max_val):
    """Clamp value between min and max"""
    return max(min_val, min(max_val, value))

def average(values):
    """Calculate average of list"""
    return sum(values) / len(values) if values else 0
```

## Library Versions

Check library versions and compatibility:
```python
# Some libraries have version info
import ssd1306
if hasattr(ssd1306, '__version__'):
    print(ssd1306.__version__)
```

## Troubleshooting

### ImportError: no module named 'xxx'
- Library not uploaded to ESP8266
- Check spelling and case sensitivity
- Verify file is in root directory

### MemoryError during import
- Compile to .mpy format
- Remove unused libraries
- Import only needed functions

### Library conflicts
- Check for duplicate files
- Ensure correct versions
- Update MicroPython firmware

## Additional Resources

- [MicroPython Libraries](https://docs.micropython.org/en/latest/library/)
- [awesome-micropython](https://github.com/mcauser/awesome-micropython)
- [MicroPython Package Index](https://micropython.org/pi/)

## Contributing

If you create useful helper libraries:
1. Document them clearly
2. Include usage examples
3. Submit a pull request
