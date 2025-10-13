# Basic Examples

This directory contains beginner-friendly examples for getting started with MicroPython on the ESP8266.

## Examples

### 1. blink.py
The classic "Hello World" of microcontrollers. Blinks the onboard LED.

**What you'll learn:**
- Using the `machine` module
- Controlling GPIO pins
- Basic timing with `time.sleep()`
- Understanding active LOW LEDs on ESP8266

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini) only

---

### 2. button.py
Reading button input and controlling an LED with debouncing.

**What you'll learn:**
- Reading digital input
- Using pull-up resistors
- Button debouncing
- Event-driven programming basics

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- Push button
- Optional: External LED and resistor

---

### 3. pwm_led.py
Fading an LED using Pulse Width Modulation (PWM).

**What you'll learn:**
- PWM basics
- Controlling LED brightness
- Duty cycle concepts
- ESP8266 PWM range (0-1023)

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- External LED and 220Ω resistor

---

### 4. adc_read.py
Reading analog values from the ADC pin.

**What you'll learn:**
- Analog to Digital Conversion
- Reading sensors
- Voltage calculations
- ESP8266 ADC limitations (0-1.0V, single pin)

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- Optional: Potentiometer or analog sensor

---

## How to Use

1. Connect your ESP8266 to your computer
2. Open Thonny IDE or your preferred MicroPython editor
3. Copy the example code to your ESP8266
4. Run the script and observe the output
5. Modify the code to experiment and learn!

## Next Steps

Once you're comfortable with these basic examples, check out the `intermediate/` directory for I2C sensors and more complex projects.

## ESP8266 Pin Notes

- Onboard LED: GPIO2 (D4) - Active LOW
- I2C Default: SCL=GPIO5 (D1), SDA=GPIO4 (D2)
- ADC: A0 (0-1.0V range)
- Available GPIO: D0-D8 (but some have special functions)
