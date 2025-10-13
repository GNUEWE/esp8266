# Tutorial 2: GPIO Basics

## Introduction

GPIO (General Purpose Input/Output) pins allow your ESP8266 to interact with the physical world. In this tutorial, you'll learn to control LEDs, read buttons, and use PWM.

## Prerequisites

- Completed [Tutorial 1: Getting Started](01_getting_started.md)
- ESP8266 with MicroPython installed
- Basic understanding of Python

## Hardware for This Tutorial

### Required
- ESP8266 (Wemos D1 or D1 Mini)

### Optional (for experiments)
- LED + 220Ω resistor
- Push button
- Breadboard
- Jumper wires

## Understanding GPIO Pins

### ESP8266 GPIO Limitations

Not all pins are created equal on ESP8266!

**Safe pins for general use:**
- D5 (GPIO14)
- D6 (GPIO12)
- D7 (GPIO13)

**Special considerations:**
- D0 (GPIO16): No PWM or I2C
- D3 (GPIO0): Must be HIGH at boot
- D4 (GPIO2): Must be HIGH at boot (onboard LED)
- D8 (GPIO15): Must be LOW at boot

**Reserved for I2C:**
- D1 (GPIO5): SCL
- D2 (GPIO4): SDA

See [Pin Reference](../docs/pin_reference.md) for complete details.

## Part 1: Controlling an LED (Digital Output)

### Using Onboard LED

The onboard LED is the easiest to start with:

```python
from machine import Pin
import time

# GPIO2 = onboard LED (active LOW!)
led = Pin(2, Pin.OUT)

# Blink 5 times
for i in range(5):
    led.off()  # Turn ON (active LOW)
    print("LED ON")
    time.sleep(0.5)
    led.on()   # Turn OFF (active LOW)
    print("LED OFF")
    time.sleep(0.5)
```

**Key point:** ESP8266's onboard LED is "active LOW":
- `led.off()` → LED turns ON
- `led.on()` → LED turns OFF

### Using External LED

Connect external LED:
```
D6 (GPIO12) → 220Ω resistor → LED anode (+)
LED cathode (-) → GND
```

Code:
```python
from machine import Pin
import time

# External LED on D6
led = Pin(12, Pin.OUT)

for i in range(5):
    led.on()   # Turn ON (normal polarity)
    time.sleep(0.5)
    led.off()  # Turn OFF
    time.sleep(0.5)
```

External LEDs are "active HIGH" (normal polarity).

## Part 2: Reading a Button (Digital Input)

### Circuit
```
D5 (GPIO14) → One side of button
Other side of button → GND
```

### Code with Pull-up Resistor

```python
from machine import Pin
import time

# Button on D5 with internal pull-up
button = Pin(14, Pin.IN, Pin.PULL_UP)
led = Pin(2, Pin.OUT)
led.on()  # LED off initially

print("Press the button!")

while True:
    if button.value() == 0:  # Pressed (LOW)
        led.off()  # LED on
        print("Button pressed!")
        time.sleep(0.5)  # Debounce
    else:  # Not pressed (HIGH)
        led.on()   # LED off
    
    time.sleep(0.01)  # Small delay
```

**How it works:**
- Pull-up resistor keeps pin HIGH when button not pressed
- Pressing button connects pin to GND (LOW)
- `button.value()` returns 0 when pressed, 1 when released

### Button with Debouncing

Real buttons "bounce" - they produce multiple signals. Here's better code:

```python
from machine import Pin
import time

button = Pin(14, Pin.IN, Pin.PULL_UP)
led = Pin(2, Pin.OUT)

last_state = 1
led_state = True

while True:
    current_state = button.value()
    
    # Detect button press (HIGH → LOW transition)
    if last_state == 1 and current_state == 0:
        time.sleep(0.05)  # Debounce delay
        if button.value() == 0:  # Confirm still pressed
            led_state = not led_state
            if led_state:
                led.on()   # Off
                print("LED OFF")
            else:
                led.off()  # On
                print("LED ON")
    
    last_state = current_state
    time.sleep(0.01)
```

## Part 3: PWM (Pulse Width Modulation)

PWM lets you control LED brightness, motor speed, etc.

### LED Fading

```python
from machine import Pin, PWM
import time

# Create PWM on D6
led = PWM(Pin(12))
led.freq(1000)  # 1000 Hz frequency

print("Fading LED...")

while True:
    # Fade in
    for duty in range(0, 1024, 8):
        led.duty(duty)
        time.sleep(0.01)
    
    # Fade out
    for duty in range(1023, -1, -8):
        led.duty(duty)
        time.sleep(0.01)
```

**PWM parameters:**
- `freq()`: PWM frequency (Hz) - usually 1000 Hz for LEDs
- `duty()`: Duty cycle (0-1023)
  - 0 = always off
  - 512 = 50% on
  - 1023 = always on

### Breathing LED Effect

```python
from machine import Pin, PWM
import time
import math

led = PWM(Pin(12))
led.freq(1000)

def breathe():
    """Smooth breathing effect using sine wave"""
    while True:
        for i in range(360):
            # Convert angle to smooth sine wave
            duty = int((math.sin(math.radians(i)) + 1) * 511.5)
            led.duty(duty)
            time.sleep(0.01)

breathe()
```

## Part 4: Combining Input and Output

### Button-controlled Brightness

```python
from machine import Pin, PWM
import time

button_up = Pin(14, Pin.IN, Pin.PULL_UP)    # D5
button_down = Pin(12, Pin.IN, Pin.PULL_UP)  # D6
led = PWM(Pin(13))  # D7
led.freq(1000)

brightness = 512  # Start at 50%

print("Use buttons to control LED brightness")
print("Button 1 (D5): Increase")
print("Button 2 (D6): Decrease")

while True:
    # Button to increase brightness
    if button_up.value() == 0:
        brightness = min(1023, brightness + 50)
        led.duty(brightness)
        print(f"Brightness: {brightness}")
        time.sleep(0.1)
    
    # Button to decrease brightness
    if button_down.value() == 0:
        brightness = max(0, brightness - 50)
        led.duty(brightness)
        print(f"Brightness: {brightness}")
        time.sleep(0.1)
    
    time.sleep(0.01)
```

## Part 5: Multiple LEDs (Traffic Light)

### Circuit
```
D5 (GPIO14) → 220Ω → Red LED → GND
D6 (GPIO12) → 220Ω → Yellow LED → GND
D7 (GPIO13) → 220Ω → Green LED → GND
```

### Code

```python
from machine import Pin
import time

# Traffic light LEDs
red = Pin(14, Pin.OUT)
yellow = Pin(12, Pin.OUT)
green = Pin(13, Pin.OUT)

def all_off():
    red.off()
    yellow.off()
    green.off()

print("Traffic light sequence starting...")

while True:
    # Green light
    all_off()
    green.on()
    print("GREEN - Go!")
    time.sleep(3)
    
    # Yellow light
    all_off()
    yellow.on()
    print("YELLOW - Caution")
    time.sleep(1)
    
    # Red light
    all_off()
    red.on()
    print("RED - Stop!")
    time.sleep(3)
```

## Practice Exercises

### Exercise 1: SOS Signal
Create an SOS signal (... --- ...) using the onboard LED.

<details>
<summary>Solution</summary>

```python
from machine import Pin
import time

led = Pin(2, Pin.OUT)

def dot():
    led.off()
    time.sleep(0.2)
    led.on()
    time.sleep(0.2)

def dash():
    led.off()
    time.sleep(0.6)
    led.on()
    time.sleep(0.2)

def sos():
    # S (...)
    for _ in range(3):
        dot()
    time.sleep(0.4)
    
    # O (---)
    for _ in range(3):
        dash()
    time.sleep(0.4)
    
    # S (...)
    for _ in range(3):
        dot()
    time.sleep(2)

while True:
    sos()
```
</details>

### Exercise 2: Button Counter
Count button presses and blink LED that many times.

### Exercise 3: PWM Music
Use PWM to create different tones (change frequency).

## Common Issues and Solutions

### LED doesn't light up
- Check polarity (long leg = +, short = -)
- Verify resistor is 220Ω (not too high)
- Check GPIO pin number
- Use multimeter to test

### Button always reads pressed
- Add pull-up resistor: `Pin(14, Pin.IN, Pin.PULL_UP)`
- Check wiring

### PWM doesn't work on D0
- GPIO16 (D0) doesn't support PWM
- Use D5, D6, or D7 instead

### ESP8266 resets randomly
- Power supply issue
- Draw too much current
- Use external power supply

## Summary

You learned:
- ✓ Digital output (controlling LEDs)
- ✓ Digital input (reading buttons)
- ✓ Internal pull-up resistors
- ✓ Button debouncing
- ✓ PWM for LED brightness control
- ✓ Combining multiple inputs and outputs

## Next Steps

1. Try [Tutorial 3: I2C Communication](03_i2c_communication.md)
2. Experiment with the [basic examples](../examples/basic/)
3. Read about [analog input (ADC)](../examples/basic/adc_read.py)
4. Learn about [interrupts](https://docs.micropython.org/en/latest/esp8266/quickref.html#pins-and-gpio)

## Resources

- [MicroPython Pin Documentation](https://docs.micropython.org/en/latest/library/machine.Pin.html)
- [MicroPython PWM Documentation](https://docs.micropython.org/en/latest/library/machine.PWM.html)
- [Pin Reference Guide](../docs/pin_reference.md)
