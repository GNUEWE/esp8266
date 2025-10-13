"""
Button Input Example
====================
This example demonstrates reading button input and controlling an LED.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- Push button connected to D5 (GPIO14)
- Optional: External LED and resistor on D6 (GPIO12)

Connections:
- Button: One side to D5, other side to GND
- LED (optional): Anode to D6 through 220Ω resistor, cathode to GND

The internal pull-up resistor is used, so button reads HIGH when not pressed.
"""

from machine import Pin
import time

# Initialize button with pull-up resistor (HIGH when not pressed)
button = Pin(14, Pin.IN, Pin.PULL_UP)  # D5

# Use onboard LED (GPIO2) or external LED on GPIO12 (D6)
led = Pin(2, Pin.OUT)  # Onboard LED (active LOW)
# led = Pin(12, Pin.OUT)  # Uncomment for external LED on D6

print("Button Example")
print("Press the button to toggle the LED")
print("Press Ctrl+C to stop")

# Track button state for debouncing
last_state = button.value()
led_state = False

try:
    while True:
        current_state = button.value()
        
        # Detect button press (transition from HIGH to LOW)
        if last_state == 1 and current_state == 0:
            time.sleep(0.05)  # Debounce delay
            if button.value() == 0:  # Confirm button is still pressed
                led_state = not led_state
                if led_state:
                    led.off()  # Turn on (active LOW for onboard LED)
                    print("LED ON")
                else:
                    led.on()   # Turn off (active LOW for onboard LED)
                    print("LED OFF")
        
        last_state = current_state
        time.sleep(0.01)  # Small delay to reduce CPU usage
        
except KeyboardInterrupt:
    print("\nProgram stopped")
    led.on()  # Turn off LED
