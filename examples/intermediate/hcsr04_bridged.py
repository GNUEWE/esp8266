"""
HC-SR04 Ultrasonic Sensor Example (Bridged Echo/Trigger)
==========================================================
This example demonstrates measuring distance using an HC-SR04 ultrasonic sensor
with the Echo and Trigger pins soldered/bridged together.

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- HC-SR04 Ultrasonic Sensor (with Echo and Trigger bridged)

Connections:
- Sensor VCC -> 5V (or 3.3V for 3.3V compatible modules)
- Sensor GND -> GND
- Sensor TRIG/ECHO (bridged) -> D7 (GPIO13)

Note: When Echo and Trigger are bridged, the same pin is used for both
sending the trigger pulse and receiving the echo response.
This saves one GPIO pin but requires careful timing.
"""

from machine import Pin
import time

class HCSR04Bridged:
    """
    Driver for HC-SR04 ultrasonic distance sensor with bridged echo/trigger.
    """
    def __init__(self, pin):
        self.pin_num = pin
        self.pin = None
        
    def measure_distance(self):
        """
        Measure distance in centimeters.
        Returns None if measurement fails.
        """
        # Set pin as output for trigger
        self.pin = Pin(self.pin_num, Pin.OUT)
        
        # Send trigger pulse
        self.pin.low()
        time.sleep_us(2)
        self.pin.high()
        time.sleep_us(10)
        self.pin.low()
        
        # Quickly switch to input mode to receive echo
        self.pin = Pin(self.pin_num, Pin.IN)
        
        # Wait for echo pulse
        timeout = 30000  # 30ms timeout
        start = time.ticks_us()
        
        # Wait for echo to go high
        while self.pin.value() == 0:
            if time.ticks_diff(time.ticks_us(), start) > timeout:
                return None
        pulse_start = time.ticks_us()
        
        # Wait for echo to go low
        while self.pin.value() == 1:
            if time.ticks_diff(time.ticks_us(), pulse_start) > timeout:
                return None
        pulse_end = time.ticks_us()
        
        # Calculate distance (speed of sound = 343 m/s)
        pulse_duration = time.ticks_diff(pulse_end, pulse_start)
        distance = (pulse_duration * 0.0343) / 2
        
        return distance

# Initialize sensor with bridged echo/trigger on GPIO13 (D7)
sensor = HCSR04Bridged(pin=13)

print("HC-SR04 Ultrasonic Distance Sensor (Bridged Mode)")
print("Measuring distance...")
print("Press Ctrl+C to stop")

try:
    while True:
        distance = sensor.measure_distance()
        
        if distance is not None:
            print(f"Distance: {distance:.1f} cm ({distance/2.54:.1f} inches)")
            
            # Visual distance indicator
            if distance < 10:
                print("*** VERY CLOSE ***")
            elif distance < 20:
                print("** CLOSE **")
            elif distance < 50:
                print("* Near *")
            elif distance < 100:
                print("- Medium -")
            else:
                print("  Far")
        else:
            print("Measurement failed - out of range or timeout")
        
        print("-" * 40)
        time.sleep(0.5)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
