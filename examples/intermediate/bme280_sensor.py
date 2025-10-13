"""
BME280 Sensor Example
======================
This example demonstrates reading temperature, humidity, and pressure
from a BME280 sensor via I2C.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- BME280 Environmental Sensor

Connections:
- Sensor SDA -> D2 (GPIO4)
- Sensor SCL -> D1 (GPIO5)
- Sensor VCC -> 3.3V
- Sensor GND -> GND

Note: Install bme280.py library to your ESP8266 first
You can download it from: https://github.com/catdog2/mpy_bme280_esp8266
"""

from machine import I2C, Pin
import bme280
import time

# Initialize I2C (GPIO5=SCL, GPIO4=SDA)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

print("BME280 Sensor Example")
print("Scanning I2C bus...")

# Scan for I2C devices
devices = i2c.scan()
if devices:
    print(f"Found I2C devices at addresses: {[hex(addr) for addr in devices]}")
else:
    print("No I2C devices found!")
    print("Check your connections!")
    exit()

# Initialize BME280 sensor
# Default I2C address is 0x76 (some modules use 0x77)
try:
    bme = bme280.BME280(i2c=i2c)
except:
    print("Could not find BME280 sensor!")
    print("Check connections and I2C address")
    exit()

print("BME280 sensor initialized successfully")
print("Reading temperature, humidity, and pressure...")
print("Press Ctrl+C to stop")

try:
    while True:
        # Read sensor values
        # Returns a tuple: (temperature, pressure, humidity)
        temp, pressure, humidity = bme.read_compensated_data()
        
        # Convert to human-readable units
        temperature = temp / 100  # Convert to Celsius
        pressure_hpa = pressure / 25600  # Convert to hPa
        humidity_percent = humidity / 1024  # Convert to %
        
        # Display values
        print(f"Temperature: {temperature:.2f} °C")
        print(f"Humidity:    {humidity_percent:.2f} %")
        print(f"Pressure:    {pressure_hpa:.2f} hPa")
        print("-" * 40)
        
        time.sleep(2)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
