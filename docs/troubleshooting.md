# Troubleshooting Guide

Common issues and solutions for ESP8266 MicroPython development.

## Flashing and Connection Issues

### Cannot connect to ESP8266
**Symptoms:**
- Serial port not detected
- "Failed to connect" error

**Solutions:**
1. Install CH340G USB driver (for Wemos D1 Mini)
   - Windows: [CH340 Driver](http://www.wch.cn/downloads/CH341SER_EXE.html)
   - Mac: [CH340 Driver for Mac](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)
   - Linux: Usually included in kernel

2. Check USB cable
   - Use data cable (not charge-only)
   - Try different USB port
   - Try different cable

3. Check port permissions (Linux)
   ```bash
   sudo usermod -a -G dialout $USER
   # Log out and back in
   ```

### Failed to erase flash
**Solutions:**
1. Hold FLASH button while connecting
2. Try slower baud rate:
   ```bash
   esptool.py --port /dev/ttyUSB0 --baud 115200 erase_flash
   ```

3. Check power supply
   - Use powered USB hub
   - External 5V power supply

### Firmware upload fails
**Solutions:**
1. Erase flash first:
   ```bash
   esptool.py --port /dev/ttyUSB0 erase_flash
   ```

2. Use correct offset (0x0 for ESP8266):
   ```bash
   esptool.py --port /dev/ttyUSB0 --baud 460800 write_flash --flash_size=detect 0 firmware.bin
   ```

3. Try slower baud rate:
   ```bash
   esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash --flash_size=detect 0 firmware.bin
   ```

## MicroPython REPL Issues

### Cannot access REPL
**Solutions:**
1. Press Ctrl+C to stop running code
2. Press Ctrl+D to soft reset
3. Hard reset: Press RST button
4. Check baud rate (115200 for ESP8266)

### Garbage characters in REPL
**Solutions:**
1. Set correct baud rate: 115200
2. Try different terminal program
3. Reset ESP8266

### REPL freezes or hangs
**Solutions:**
1. Press Ctrl+C to interrupt
2. Hard reset if needed
3. Check for infinite loops in boot.py or main.py

## GPIO and Hardware Issues

### Onboard LED not working
**Remember:** ESP8266 onboard LED is **active LOW**
```python
from machine import Pin
led = Pin(2, Pin.OUT)
led.off()  # This turns it ON
led.on()   # This turns it OFF
```

### GPIO pins not working
**Solutions:**
1. Check boot mode restrictions:
   - GPIO0 (D3): Must be HIGH at boot
   - GPIO2 (D4): Must be HIGH at boot
   - GPIO15 (D8): Must be LOW at boot

2. Check if pin is already used:
   ```python
   # Don't use TX/RX if using serial
   # Don't use D1/D2 if using I2C
   ```

3. Verify pin number:
   - Use GPIO numbers, not Wemos labels
   - D4 = GPIO2, not GPIO4

### PWM not working on GPIO16 (D0)
**Solution:** GPIO16 does NOT support PWM. Use different pin (D5, D6, D7).

### ADC reading wrong values
**Solutions:**
1. Check voltage range (0-1.0V max)
2. Use voltage divider for higher voltages:
   ```
   3.3V ---[220kΩ]--- A0 ---[100kΩ]--- GND
   ```

3. Average multiple readings:
   ```python
   from machine import ADC
   import time
   
   adc = ADC(0)
   readings = [adc.read() for _ in range(10)]
   avg = sum(readings) / len(readings)
   ```

## I2C Issues

### I2C device not found
**Solutions:**
1. Scan I2C bus:
   ```python
   from machine import I2C, Pin
   i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)
   print([hex(addr) for addr in i2c.scan()])
   ```

2. Check connections:
   - SDA to D2 (GPIO4)
   - SCL to D1 (GPIO5)
   - VCC to 3.3V
   - GND to GND

3. Try different I2C address:
   - SSD1306: 0x3C or 0x3D
   - BME280: 0x76 or 0x77

4. Add pull-up resistors (4.7kΩ):
   ```
   3.3V ---[4.7kΩ]--- SDA
   3.3V ---[4.7kΩ]--- SCL
   ```

5. Reduce I2C frequency:
   ```python
   i2c = I2C(scl=Pin(5), sda=Pin(4), freq=100000)
   ```

### Multiple I2C devices conflict
**Solutions:**
1. Check each device has unique address
2. Power all devices properly
3. Add pull-up resistors (4.7kΩ)
4. Initialize one device at a time for testing

### SSD1306 display shows nothing
**Solutions:**
1. Check contrast settings
2. Call `oled.show()` after drawing
3. Try `oled.fill(1)` then `oled.show()` to test
4. Check if display is 128x64 or 128x32:
   ```python
   oled = ssd1306.SSD1306_I2C(128, 64, i2c)  # or 128, 32
   ```

## Sensor Issues

### BME280/BME680 wrong readings
**Solutions:**
1. Wait for sensor to stabilize (few seconds)
2. Check calibration data is loading
3. For BME680 gas readings:
   - Need several minutes to stabilize
   - Configure heater properly
   - First readings will be inaccurate

### HC-SR04 returns None or timeout
**Solutions:**
1. Check connections:
   - VCC to 5V (not 3.3V)
   - Ensure trigger/echo are bridged properly

2. Check distance:
   - Minimum: 2cm
   - Maximum: 400cm
   - Surface must be reflective

3. Add delay between measurements:
   ```python
   time.sleep(0.1)  # 100ms between readings
   ```

4. Increase timeout:
   ```python
   timeout = 50000  # 50ms
   ```

### INA219 wrong current readings
**Solutions:**
1. Check shunt resistor value (usually 0.1Ω)
2. Configure for correct range:
   ```python
   ina = INA219(0.1, i2c, max_expected_amps=0.4)
   ```

3. Ensure proper connections:
   - VIN+ to power supply positive
   - VIN- to load positive
   - Load negative to power supply negative

## Memory Issues

### MemoryError exception
**Solutions:**
1. ESP8266 has limited RAM (~36KB free)
2. Delete unused variables:
   ```python
   import gc
   del large_variable
   gc.collect()
   ```

3. Use smaller buffers:
   ```python
   # Instead of large lists
   history = []  # Limited to 32 items
   if len(history) > 32:
       history.pop(0)
   ```

4. Split code into modules
5. Use generators instead of lists

### Out of memory during import
**Solutions:**
1. Pre-compile modules to .mpy:
   ```bash
   mpy-cross module.py
   ```

2. Import only what you need:
   ```python
   from module import specific_function
   # Instead of: import module
   ```

3. Reduce imported libraries

## WiFi Issues

### Cannot connect to WiFi
**Solutions:**
1. Check SSID and password
2. Use 2.4GHz network (ESP8266 doesn't support 5GHz)
3. Check signal strength
4. Example connection code:
   ```python
   import network
   
   sta_if = network.WLAN(network.STA_IF)
   sta_if.active(True)
   sta_if.connect('SSID', 'password')
   
   while not sta_if.isconnected():
       pass
   print('Connected:', sta_if.ifconfig())
   ```

### ESP8266 resets when using WiFi
**Solutions:**
1. Power supply issue - use better power source
2. Add decoupling capacitors (100µF + 0.1µF)
3. Use powered USB hub
4. External 5V power supply with adequate current (>500mA)

## File System Issues

### Cannot upload files
**Solutions:**
1. Check available space:
   ```python
   import os
   stats = os.statvfs('/')
   free = stats[0] * stats[3]
   print(f'Free space: {free} bytes')
   ```

2. Use correct tool:
   - ampy: `ampy --port /dev/ttyUSB0 put file.py`
   - mpremote: `mpremote cp file.py :`
   - Thonny: File → Save As → MicroPython device

3. Close other programs using serial port

### Files disappear after reset
**Solution:** Files in `/tmp` are lost. Save to root:
```python
with open('/data.txt', 'w') as f:
    f.write('persistent data')
```

## Common Error Messages

### `OSError: [Errno 19] ENODEV`
**Cause:** I2C device not found

**Solution:** Check I2C connections and address

### `OSError: [Errno 116] ETIMEDOUT`
**Cause:** I2C communication timeout

**Solutions:**
- Reduce I2C frequency
- Add pull-up resistors
- Check power supply

### `ImportError: no module named 'xxx'`
**Cause:** Library not installed

**Solution:** Upload required library to ESP8266

### `ValueError: invalid pin`
**Cause:** Using invalid GPIO pin

**Solution:** Check pin reference and use valid GPIO number

### `MemoryError: memory allocation failed`
**Cause:** Out of RAM

**Solutions:**
- Use `gc.collect()`
- Reduce buffer sizes
- Split into smaller functions

## Performance Issues

### Code runs slowly
**Solutions:**
1. Increase CPU frequency:
   ```python
   import machine
   machine.freq(160000000)  # 160 MHz
   ```

2. Optimize loops:
   - Use list comprehensions
   - Avoid repeated calculations
   - Cache results

3. Use native code for critical sections

### Display updates slowly
**Solutions:**
1. Reduce update frequency
2. Update only changed regions
3. Use smaller fonts or less graphics
4. Increase I2C frequency (up to 400kHz)

## Getting More Help

1. Check [MicroPython forum](https://forum.micropython.org/)
2. Search [ESP8266 community](https://www.esp8266.com/)
3. Check [GitHub issues](https://github.com/micropython/micropython/issues)
4. Enable debug output:
   ```python
   import esp
   esp.osdebug(None)  # Disable debug output
   esp.osdebug(0)     # Enable debug output
   ```

## Diagnostic Commands

```python
# System information
import esp
print(f'Free heap: {esp.freemem()} bytes')
print(f'Flash size: {esp.flash_size()} bytes')

# Check chip ID
import machine
print(f'Chip ID: {machine.unique_id().hex()}')

# Test I2C
from machine import I2C, Pin
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)
print(f'I2C devices: {[hex(x) for x in i2c.scan()]}')

# Memory usage
import gc
gc.collect()
print(f'Free memory: {gc.mem_free()} bytes')
print(f'Used memory: {gc.mem_alloc()} bytes')
```
