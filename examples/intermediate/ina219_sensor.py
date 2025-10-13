"""
INA219 Current Sensor Example
==============================
This example demonstrates reading voltage, current, and power
from an INA219 sensor via I2C.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- INA219 Current/Power Sensor

Connections:
- Sensor SDA -> D2 (GPIO4)
- Sensor SCL -> D1 (GPIO5)
- Sensor VCC -> 3.3V
- Sensor GND -> GND
- Sensor VIN+ -> Positive side of load/power supply
- Sensor VIN- -> Negative side of load

Note: Install ina219.py library to your ESP8266 first
You can download it from: https://github.com/chrisb2/pyb_ina219
"""

from machine import I2C, Pin
from ina219 import INA219
import time

# Initialize I2C (GPIO5=SCL, GPIO4=SDA)
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

print("INA219 Current Sensor Example")
print("Scanning I2C bus...")

# Scan for I2C devices
devices = i2c.scan()
if devices:
    print(f"Found I2C devices at addresses: {[hex(addr) for addr in devices]}")
else:
    print("No I2C devices found!")
    print("Check your connections!")
    exit()

# Initialize INA219 sensor
# Default I2C address is 0x40
# Configure for 16V, 400mA range (adjust as needed)
SHUNT_OHMS = 0.1  # Shunt resistor value (typically 0.1 ohm)
MAX_EXPECTED_AMPS = 0.4  # Maximum expected current

try:
    ina = INA219(SHUNT_OHMS, i2c, max_expected_amps=MAX_EXPECTED_AMPS)
    ina.configure()
except:
    print("Could not find INA219 sensor!")
    print("Check connections and I2C address")
    exit()

print("INA219 sensor initialized successfully")
print("Reading voltage, current, and power...")
print("Press Ctrl+C to stop")

# Variables for tracking
total_energy = 0.0  # mWh
last_time = time.ticks_ms()

try:
    while True:
        try:
            # Read sensor values
            voltage = ina.voltage()  # Bus voltage in volts
            current = ina.current()  # Current in milliamps
            power = ina.power()      # Power in milliwatts
            
            # Calculate time elapsed
            current_time = time.ticks_ms()
            time_diff = time.ticks_diff(current_time, last_time) / 3600000.0  # Convert to hours
            last_time = current_time
            
            # Calculate energy consumed
            total_energy += power * time_diff  # mWh
            
            # Display values
            print(f"Voltage:       {voltage:.3f} V")
            print(f"Current:       {current:.1f} mA")
            print(f"Power:         {power:.1f} mW")
            print(f"Total Energy:  {total_energy:.2f} mWh")
            print("-" * 40)
            
        except Exception as e:
            print(f"Error reading sensor: {e}")
        
        time.sleep(1)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
    print(f"Total energy consumed: {total_energy:.2f} mWh ({total_energy/1000:.4f} Wh)")
