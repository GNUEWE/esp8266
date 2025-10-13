# Pin Reference

## ESP8266 GPIO to Wemos D1 Mini Pin Mapping

| Wemos Pin | GPIO | Functions | Notes |
|-----------|------|-----------|-------|
| D0 | GPIO16 | Wake from deep sleep | No PWM, no I2C support |
| D1 | GPIO5 | I2C SCL | Default I2C clock |
| D2 | GPIO4 | I2C SDA | Default I2C data |
| D3 | GPIO0 | Flash button | Must be HIGH at boot, pulled up |
| D4 | GPIO2 | Onboard LED, TXD1 | Must be HIGH at boot, Active LOW |
| D5 | GPIO14 | SPI CLK | PWM capable |
| D6 | GPIO12 | SPI MISO | PWM capable |
| D7 | GPIO13 | SPI MOSI | PWM capable |
| D8 | GPIO15 | SPI CS | Must be LOW at boot, pulled down |
| TX | GPIO1 | UART TX | Used for serial/programming |
| RX | GPIO3 | UART RX | Used for serial/programming |
| A0 | ADC | Analog input | 0-1.0V range (0-1023 values) |

## Pin States at Boot

**MUST be HIGH at boot:**
- D3 (GPIO0)
- D4 (GPIO2)

**MUST be LOW at boot:**
- D8 (GPIO15)

**Don't care:**
- All other pins

## Safe Pins for General Use

Best pins for general GPIO (no boot mode restrictions):
- **D1 (GPIO5)** - I2C SCL default
- **D2 (GPIO4)** - I2C SDA default
- **D5 (GPIO14)** - Safe for any use
- **D6 (GPIO12)** - Safe for any use
- **D7 (GPIO13)** - Safe for any use

## I2C Configuration

### Default I2C Pins
```python
from machine import I2C, Pin
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)  # D1=SCL, D2=SDA
```

### Alternative I2C Pins
You can use any GPIO pins for software I2C:
```python
i2c = I2C(scl=Pin(14), sda=Pin(12), freq=100000)  # D5=SCL, D6=SDA
```

### Common I2C Addresses
- **SSD1306 OLED**: 0x3C or 0x3D
- **BME280**: 0x76 or 0x77
- **BME680**: 0x76 or 0x77
- **INA219**: 0x40 (default, configurable 0x40-0x4F)

### Scanning I2C Bus
```python
devices = i2c.scan()
print([hex(addr) for addr in devices])
```

## PWM Pins

PWM is available on most GPIO pins except GPIO16 (D0).

```python
from machine import Pin, PWM

# Create PWM on D6 (GPIO12)
pwm = PWM(Pin(12))
pwm.freq(1000)      # Set frequency to 1000 Hz
pwm.duty(512)       # Set duty cycle (0-1023)
```

**PWM capable pins:**
- D1, D2, D3, D4, D5, D6, D7, D8, TX, RX

**NOT PWM capable:**
- D0 (GPIO16)

## ADC (Analog Input)

ESP8266 has only ONE analog input pin.

```python
from machine import ADC

adc = ADC(0)        # A0 is the only ADC pin
value = adc.read()  # Returns 0-1023
```

**Important:**
- Range: 0-1.0V (NOT 3.3V!)
- Digital range: 0-1023
- Use voltage divider for higher voltages
- Example: For 3.3V range: R1=220kΩ, R2=100kΩ

## SPI Pins

### Hardware SPI
```python
from machine import SPI, Pin

# Default hardware SPI pins
spi = SPI(1, baudrate=10000000, polarity=0, phase=0)
# MISO = D6 (GPIO12)
# MOSI = D7 (GPIO13)
# SCK  = D5 (GPIO14)
# CS   = D8 (GPIO15) - manual control
```

## UART Pins

### Hardware UART
```python
from machine import UART

# UART0 - used by REPL/console (don't use)
# UART1 - transmit only
uart = UART(1, 9600)
uart.write('hello')
```

**Pins:**
- UART0 TX: GPIO1 (TX)
- UART0 RX: GPIO3 (RX)
- UART1 TX: GPIO2 (D4)

## Pin Usage Examples

### Example 1: Basic GPIO
```python
from machine import Pin

# Output
led = Pin(2, Pin.OUT)     # D4 (onboard LED)
led.off()                 # Turn on (active LOW)

# Input
button = Pin(14, Pin.IN, Pin.PULL_UP)  # D5
if button.value() == 0:
    print("Button pressed")
```

### Example 2: I2C Devices
```python
from machine import I2C, Pin

# Initialize I2C
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

# Connect multiple I2C devices
# - SSD1306 at 0x3C
# - BME280 at 0x76
# - INA219 at 0x40
# All share the same SDA/SCL lines
```

### Example 3: PWM LED Fading
```python
from machine import Pin, PWM
import time

led = PWM(Pin(12))  # D6
led.freq(1000)

for duty in range(0, 1024, 8):
    led.duty(duty)
    time.sleep(0.01)
```

### Example 4: HC-SR04 Ultrasonic (Bridged)
```python
from machine import Pin
import time

pin_num = 13  # D7

# Set as output for trigger
pin = Pin(pin_num, Pin.OUT)
pin.low()
time.sleep_us(2)
pin.high()
time.sleep_us(10)
pin.low()

# Switch to input for echo
pin = Pin(pin_num, Pin.IN)
# ... measure pulse width
```

## Pin Planning Template

When planning your project, consider:

| Purpose | Pin | GPIO | Notes |
|---------|-----|------|-------|
| I2C SDA | D2 | 4 | Multiple I2C devices |
| I2C SCL | D1 | 5 | Multiple I2C devices |
| Ultrasonic | D7 | 13 | Bridged trigger/echo |
| Button | D5 | 14 | With pull-up |
| LED | D6 | 12 | PWM capable |
| Spare | D0 | 16 | No PWM/I2C |
| Spare | D3 | 0 | Must be HIGH at boot |
| Spare | D8 | 15 | Must be LOW at boot |

## Common Connection Examples

### SSD1306 OLED Display
```
SSD1306    Wemos D1 Mini
-------    -------------
VCC    →   3.3V
GND    →   GND
SDA    →   D2 (GPIO4)
SCL    →   D1 (GPIO5)
```

### BME280/BME680 Sensor
```
BME280     Wemos D1 Mini
-------    -------------
VCC    →   3.3V
GND    →   GND
SDA    →   D2 (GPIO4)
SCL    →   D1 (GPIO5)
```

### INA219 Current Sensor
```
INA219     Wemos D1 Mini
-------    -------------
VCC    →   3.3V
GND    →   GND
SDA    →   D2 (GPIO4)
SCL    →   D1 (GPIO5)
VIN+   →   Power supply +
VIN-   →   Load +
```

### HC-SR04 (Bridged Mode)
```
HC-SR04    Wemos D1 Mini
-------    -------------
VCC    →   5V
GND    →   GND
TRIG/  →   D7 (GPIO13)
ECHO       (bridged together)
```

### Button with LED
```
Button     Wemos D1 Mini
-------    -------------
One side → D5 (GPIO14)
Other    → GND

LED        Wemos D1 Mini
-------    -------------
Anode  →   D6 (GPIO12) → 220Ω resistor
Cathode→   GND
```

## Troubleshooting Pin Issues

### Problem: Device won't boot
- Check D3 (GPIO0) is not pulled LOW
- Check D8 (GPIO15) is not pulled HIGH
- D4 (GPIO2) should be HIGH

### Problem: I2C not working
- Verify SDA=D2 (GPIO4), SCL=D1 (GPIO5)
- Add pull-up resistors (4.7kΩ)
- Reduce frequency to 100kHz
- Check connections and addresses

### Problem: PWM not working
- Don't use GPIO16 (D0) for PWM
- Check frequency and duty cycle ranges
- Verify pin is not used elsewhere

### Problem: ADC readings incorrect
- Ensure input voltage is 0-1.0V
- Use voltage divider for higher voltages
- Check for noise and filtering

## References

- [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- [Wemos D1 Mini Pinout](https://www.wemos.cc/en/latest/d1/d1_mini.html)
- [MicroPython Pin Reference](https://docs.micropython.org/en/latest/esp8266/quickref.html#pins-and-gpio)
