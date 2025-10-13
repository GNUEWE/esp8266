"""
Blink LED Example
==================
This example demonstrates the classic "Hello World" of microcontrollers:
blinking the onboard LED on the ESP8266.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)

The onboard LED is connected to GPIO2 (D4 on Wemos D1 Mini).
Note: The LED is active LOW (0 = on, 1 = off)
"""

from machine import Pin
import time

# Initialize the onboard LED (GPIO2 / D4)
# Note: On ESP8266, the onboard LED is active LOW
led = Pin(2, Pin.OUT)

print("Starting LED blink...")
print("Press Ctrl+C to stop")

try:
    while True:
        led.off()     # Turn LED on (active LOW)
        time.sleep(0.5)  # Wait 500ms
        led.on()      # Turn LED off (active LOW)
        time.sleep(0.5)  # Wait 500ms
except KeyboardInterrupt:
    print("\nProgram stopped")
    led.on()  # Ensure LED is off (active LOW) when exiting
