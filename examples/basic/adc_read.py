"""
ADC Read Example
================
This example demonstrates reading analog values from the ADC pin.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- Optional: Potentiometer or analog sensor on A0

Connections (if using potentiometer):
- Potentiometer: Outer pins to 3.3V and GND, middle pin to A0

Note: ESP8266 has only one ADC pin (A0) with range 0-1.0V (0-1023 digital)
For higher voltages, use a voltage divider.
"""

from machine import ADC
import time

# Initialize ADC (A0 is the only analog pin on ESP8266)
adc = ADC(0)

print("ADC Read Example")
print("Reading values from A0 pin")
print("Range: 0-1023 (0-1.0V)")
print("Press Ctrl+C to stop")

try:
    while True:
        # Read ADC value (0-1023)
        value = adc.read()
        
        # Convert to voltage (0-1.0V)
        voltage = value / 1023.0
        
        # Display values
        print(f"ADC: {value:4d} | Voltage: {voltage:.3f}V")
        
        time.sleep(0.5)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
