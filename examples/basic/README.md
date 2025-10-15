# Basic Examples

This directory contains beginner-friendly Arduino sketches for getting started with ESP8266.

## Examples

### 1. blink
The classic "Hello World" of microcontrollers. Blinks the onboard LED.

**Folder:** [blink/](blink/)  
**File:** blink.ino

**What you'll learn:**
- Basic Arduino sketch structure (setup and loop)
- Controlling GPIO pins
- Using Serial Monitor
- Understanding active LOW LEDs on ESP8266

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini) only

---

### 2. button
Reading button input and controlling an LED with debouncing.

**Folder:** [button/](button/)  
**File:** button.ino

**What you'll learn:**
- Reading digital input
- Using pull-up resistors (INPUT_PULLUP)
- Button debouncing
- State management

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- Push button
- Optional: External LED and resistor

---

### 3. pwm_led
Fading an LED using PWM (analogWrite).

**Folder:** [pwm_led/](pwm_led/)  
**File:** pwm_led.ino

**What you'll learn:**
- PWM basics with analogWrite()
- Controlling LED brightness
- ESP8266 PWM range (0-1023)

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- External LED and 220Ω resistor

---

### 4. adc_read
Reading analog values from the ADC pin.

**Folder:** [adc_read/](adc_read/)  
**File:** adc_read.ino

**What you'll learn:**
- Analog to Digital Conversion with analogRead()
- Reading sensors
- Voltage calculations
- ESP8266 ADC limitations (0-1.0V, single pin)

**Hardware needed:**
- ESP8266 (Wemos D1 or D1 Mini)
- Optional: Potentiometer or analog sensor

---

## How to Use

1. Open Arduino IDE
2. Select your board: Tools → Board → ESP8266 Boards → LOLIN(WEMOS) D1 R2 & mini
3. Select your port: Tools → Port → (your COM port)
4. Open the sketch (.ino file)
5. Click Upload (or press Ctrl+U)
6. Open Serial Monitor (Tools → Serial Monitor) and set baud rate to 115200

## Next Steps

Once you're comfortable with these basic examples, check out the `intermediate/` directory for I2C sensors and more complex projects.

## ESP8266 Pin Notes

- Onboard LED: GPIO2 (D4) - Active LOW
- I2C Default: SCL=GPIO5 (D1), SDA=GPIO4 (D2)
- ADC: A0 (0-1.0V range)
- Available GPIO: D0-D8 (but some have special functions)
