"""
BME680 Sensor Example
======================
This example demonstrates reading temperature, humidity, pressure, and
gas resistance from a BME680 sensor via I2C.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- BME680 Environmental Sensor (Air Quality)

Connections:
- Sensor SDA -> D2 (GPIO4)
- Sensor SCL -> D1 (GPIO5)
- Sensor VCC -> 3.3V
- Sensor GND -> GND

Note: Install bme680.py library to your ESP8266 first
You can download it from: https://github.com/robert-hh/BME680-Micropython
"""

from machine import I2C, Pin
import bme680
import time

# Initialize I2C (GPIO5=SCL, GPIO4=SDA)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

print("BME680 Sensor Example")
print("Scanning I2C bus...")

# Scan for I2C devices
devices = i2c.scan()
if devices:
    print(f"Found I2C devices at addresses: {[hex(addr) for addr in devices]}")
else:
    print("No I2C devices found!")
    print("Check your connections!")
    exit()

# Initialize BME680 sensor
# Default I2C address is 0x76 (some modules use 0x77)
try:
    bme = bme680.BME680_I2C(i2c=i2c)
except:
    print("Could not find BME680 sensor!")
    print("Check connections and I2C address")
    exit()

print("BME680 sensor initialized successfully")

# Configure sensor settings
bme.set_humidity_oversample(bme680.OS_2X)
bme.set_pressure_oversample(bme680.OS_4X)
bme.set_temperature_oversample(bme680.OS_8X)
bme.set_filter(bme680.FILTER_SIZE_3)
bme.set_gas_status(bme680.ENABLE_GAS_MEAS)
bme.set_gas_heater_temperature(320)
bme.set_gas_heater_duration(150)
bme.select_gas_heater_profile(0)

print("Reading temperature, humidity, pressure, and gas resistance...")
print("Note: Gas readings stabilize after a few minutes")
print("Press Ctrl+C to stop")

try:
    while True:
        # Trigger a measurement
        if bme.get_sensor_data():
            # Read sensor values
            temperature = bme.data.temperature
            humidity = bme.data.humidity
            pressure = bme.data.pressure
            gas_resistance = bme.data.gas_resistance
            
            # Display values
            print(f"Temperature:    {temperature:.2f} °C")
            print(f"Humidity:       {humidity:.2f} %")
            print(f"Pressure:       {pressure:.2f} hPa")
            print(f"Gas Resistance: {gas_resistance:.0f} Ohms")
            
            # Calculate simple air quality index (0-100%)
            # Note: This is a simplified calculation
            # Higher gas resistance generally means better air quality
            if gas_resistance > 0:
                aqi = min(100, (gas_resistance / 50000) * 100)
                print(f"Air Quality:    {aqi:.1f}% (approx)")
            
            print("-" * 40)
        else:
            print("Waiting for sensor data...")
        
        time.sleep(2)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
