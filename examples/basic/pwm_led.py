"""
PWM LED Example
===============
This example demonstrates using PWM to fade an LED in and out.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- External LED and 220Ω resistor on D6 (GPIO12)

Connections:
- LED anode to D6 through 220Ω resistor
- LED cathode to GND

Note: The onboard LED (GPIO2) also supports PWM if you prefer to use it.
"""

from machine import Pin, PWM
import time

# Initialize PWM on GPIO12 (D6)
# PWM frequency: 1000 Hz (1 kHz)
led = PWM(Pin(12), freq=1000)

print("PWM LED Fade Example")
print("Press Ctrl+C to stop")

try:
    while True:
        # Fade in
        for duty in range(0, 1024, 8):
            led.duty(duty)
            time.sleep(0.01)
        
        # Fade out
        for duty in range(1023, -1, -8):
            led.duty(duty)
            time.sleep(0.01)
            
except KeyboardInterrupt:
    print("\nProgram stopped")
    led.duty(0)  # Turn off LED
    led.deinit()  # Clean up PWM
