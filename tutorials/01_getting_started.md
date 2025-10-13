# Tutorial 1: Getting Started with ESP8266 and MicroPython

## Introduction

Welcome! This tutorial will guide you through setting up your ESP8266 board with MicroPython and running your first program.

## What You'll Need

### Hardware
- ESP8266 board (Wemos D1 or D1 Mini)
- USB cable (micro USB for Wemos D1 Mini)
- Computer

### Software
- Python 3.x installed on your computer
- USB serial driver (CH340G for Wemos)
- Thonny IDE (recommended for beginners)

## Step 1: Install USB Drivers

### Windows
1. Download CH340G driver from [WCH website](http://www.wch.cn/downloads/CH341SER_EXE.html)
2. Run installer
3. Restart computer if prompted

### macOS
1. Download driver from [GitHub](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)
2. Install and restart

### Linux
Driver usually included in kernel. If not:
```bash
sudo apt-get install python3-serial
```

Verify USB connection:
```bash
ls /dev/ttyUSB*  # Linux
ls /dev/tty.*    # macOS
# Check Device Manager on Windows
```

## Step 2: Install Required Tools

### Install esptool (for flashing firmware)
```bash
pip install esptool
```

### Install Thonny IDE
Download from [thonny.org](https://thonny.org/)

Alternative tools:
- ampy: `pip install adafruit-ampy`
- mpremote: `pip install mpremote`

## Step 3: Download MicroPython Firmware

1. Visit [micropython.org/download/esp8266/](https://micropython.org/download/esp8266/)
2. Download latest stable release (e.g., esp8266-*.bin)
3. Save to your computer

## Step 4: Erase Flash Memory

Before installing MicroPython, erase the flash:

### Linux/macOS
```bash
esptool.py --port /dev/ttyUSB0 erase_flash
```

### Windows
```bash
esptool.py --port COM3 erase_flash
```

**Note:** Replace `/dev/ttyUSB0` or `COM3` with your actual port.

Expected output:
```
Erasing flash (this may take a while)...
Chip erase completed successfully
```

## Step 5: Flash MicroPython Firmware

Now flash the MicroPython firmware:

### Linux/macOS
```bash
esptool.py --port /dev/ttyUSB0 --baud 460800 write_flash --flash_size=detect 0 esp8266-*.bin
```

### Windows
```bash
esptool.py --port COM3 --baud 460800 write_flash --flash_size=detect 0 esp8266-*.bin
```

If this fails, try slower baud rate:
```bash
esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash --flash_size=detect 0 esp8266-*.bin
```

Expected output:
```
Writing at 0x00000000... (100%)
Hash of data verified.
Leaving...
Hard resetting via RTS pin...
```

## Step 6: Connect to MicroPython REPL

### Using Thonny
1. Open Thonny IDE
2. Click "Run" → "Select Interpreter"
3. Choose "MicroPython (ESP8266)"
4. Select correct COM port
5. Click OK

You should see:
```
>>>
```

### Using Screen (Linux/macOS)
```bash
screen /dev/ttyUSB0 115200
```

Exit: Ctrl+A, then K, then Y

### Using PuTTY (Windows)
1. Open PuTTY
2. Connection type: Serial
3. Serial line: COM3 (your port)
4. Speed: 115200
5. Click Open

## Step 7: Test MicroPython

In the REPL, try these commands:

```python
>>> print("Hello, ESP8266!")
Hello, ESP8266!

>>> import sys
>>> print(sys.platform)
esp8266

>>> import esp
>>> print(f"Free memory: {esp.freemem()} bytes")
Free memory: 36000 bytes
```

## Step 8: Blink the LED (Your First Program!)

Type or paste this code in the REPL:

```python
from machine import Pin
import time

led = Pin(2, Pin.OUT)  # GPIO2 (onboard LED)

for i in range(10):
    led.off()  # Turn on (active LOW)
    time.sleep(0.5)
    led.on()   # Turn off (active LOW)
    time.sleep(0.5)

print("Done!")
```

Watch the onboard LED blink!

**Important:** The onboard LED on ESP8266 is "active LOW":
- `led.off()` turns it ON
- `led.on()` turns it OFF

## Step 9: Save Your First Program

### Using Thonny
1. Write your code in the editor
2. File → Save As
3. Select "MicroPython device"
4. Save as `main.py` (runs automatically on boot)

### Using ampy
```bash
# Save code to file on computer
echo 'print("Hello from ESP8266!")' > hello.py

# Upload to ESP8266
ampy --port /dev/ttyUSB0 put hello.py
```

## Step 10: Auto-run Code on Boot

Files that run automatically:
- `boot.py` - Runs first (system setup)
- `main.py` - Runs after boot.py (your main program)

Create `main.py`:
```python
# main.py
print("Starting my ESP8266 program...")

from machine import Pin
import time

led = Pin(2, Pin.OUT)

while True:
    led.off()
    time.sleep(1)
    led.on()
    time.sleep(1)
```

Save to device, then reset (press RST button). LED should blink automatically!

## Troubleshooting

### Cannot find serial port
- Check USB cable (use data cable, not charge-only)
- Install USB drivers
- Try different USB port

### "Failed to connect"
- Hold FLASH button while connecting
- Try slower baud rate (115200 instead of 460800)
- Reset ESP8266 and try again

### Garbage characters in REPL
- Check baud rate is 115200
- Press Ctrl+C to stop running code
- Press Ctrl+D to soft reset

### LED doesn't blink
- Remember: onboard LED is active LOW
- Use `led.off()` to turn it on
- Check pin number (GPIO2 = Pin 2)

## Next Steps

Congratulations! You've successfully:
- ✓ Installed MicroPython on ESP8266
- ✓ Connected to the REPL
- ✓ Run your first program
- ✓ Made the LED blink

Continue learning:
1. Try [Tutorial 2: GPIO Basics](02_gpio_basics.md)
2. Explore the [basic examples](../examples/basic/)
3. Read the [hardware documentation](../docs/hardware_overview.md)

## Additional Resources

- [MicroPython ESP8266 Quick Reference](https://docs.micropython.org/en/latest/esp8266/quickref.html)
- [MicroPython Tutorial](https://docs.micropython.org/en/latest/esp8266/tutorial/index.html)
- [ESP8266 Community Forum](https://www.esp8266.com/)

Happy coding! 🎉
