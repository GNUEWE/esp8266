"""
SSD1306 OLED Display Example
=============================
This example demonstrates using an SSD1306 OLED display via I2C.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64 or 128x32)

Connections:
- Display SDA -> D2 (GPIO4)
- Display SCL -> D1 (GPIO5)
- Display VCC -> 3.3V
- Display GND -> GND

Note: Install ssd1306.py library to your ESP8266 first
You can download it from: https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py
"""

from machine import I2C, Pin
import ssd1306
import time

# Initialize I2C (GPIO5=SCL, GPIO4=SDA)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

print("SSD1306 OLED Display Example")
print("Scanning I2C bus...")

# Scan for I2C devices
devices = i2c.scan()
if devices:
    print(f"Found I2C devices at addresses: {[hex(addr) for addr in devices]}")
else:
    print("No I2C devices found!")
    print("Check your connections!")
    exit()

# Initialize display (128x64 pixels)
# Use ssd1306.SSD1306_I2C(128, 32, i2c) for 128x32 display
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

print("Display initialized successfully")

try:
    # Clear display
    oled.fill(0)
    oled.show()
    
    # Display "Hello World"
    oled.text("Hello, ESP8266!", 0, 0)
    oled.show()
    time.sleep(2)
    
    # Clear and show MicroPython
    oled.fill(0)
    oled.text("MicroPython", 0, 0)
    oled.text("on ESP8266", 0, 10)
    oled.show()
    time.sleep(2)
    
    # Draw some shapes
    oled.fill(0)
    oled.text("Drawing...", 0, 0)
    
    # Draw rectangles
    oled.rect(10, 20, 30, 20, 1)
    oled.fill_rect(50, 20, 30, 20, 1)
    
    # Draw lines
    oled.line(0, 50, 127, 50, 1)
    oled.line(64, 0, 64, 63, 1)
    
    oled.show()
    time.sleep(2)
    
    # Scrolling text demo
    text = "ESP8266 + SSD1306 = Awesome!"
    for i in range(len(text) * 8):
        oled.fill(0)
        oled.text(text, -i, 28)
        oled.show()
        time.sleep(0.05)
    
    # Counter demo
    print("Counting up... Press Ctrl+C to stop")
    count = 0
    while True:
        oled.fill(0)
        oled.text("Counter:", 0, 20)
        oled.text(str(count), 0, 35)
        oled.show()
        count += 1
        time.sleep(0.5)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
    oled.fill(0)
    oled.text("Goodbye!", 30, 28)
    oled.show()
    time.sleep(1)
    oled.fill(0)
    oled.show()
