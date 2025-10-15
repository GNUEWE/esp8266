# Hardware Overview

## ESP8266 Specifications

The ESP8266 is a low-cost Wi-Fi microcontroller with integrated TCP/IP stack.

### Key Features
- **CPU**: Tensilica L106 32-bit RISC processor @ 80/160 MHz
- **RAM**: 160 KB
- **Flash**: 512 KB to 4 MB (depends on module)
- **WiFi**: 802.11 b/g/n (2.4 GHz)
- **GPIO**: 16 digital pins (some with limitations)
- **ADC**: 1 analog input (0-1.0V)
- **Interfaces**: SPI, I2C, UART, PWM

### Power Requirements
- **Operating Voltage**: 3.3V
- **Current Draw**: 
  - Active: ~80-170mA
  - Deep sleep: ~20µA
  - Modem sleep: ~15mA

## Wemos D1 / D1 Mini

### Wemos D1 Mini Specifications
- **Microcontroller**: ESP-8266EX
- **Clock Speed**: 80MHz/160MHz
- **USB to Serial**: CH340G
- **Flash Memory**: 4MB
- **Digital I/O**: 11 pins
- **Analog Input**: 1 pin (3.2V max input)
- **Dimensions**: 34.2mm x 25.6mm
- **Weight**: 3g

### Pin Layout

```
         D1 Mini
      +---------+
RST  -|         |- TX
A0   -|         |- RX
D0   -|         |- D1 (SCL)
D5   -|         |- D2 (SDA)
D6   -|         |- D3
D7   -|         |- D4 (LED)
D8   -|         |- GND
3V3  -|         |- 5V
      +---------+
```

### Special Pins
- **D0 (GPIO16)**: Used for wake from deep sleep, no PWM or I2C
- **D1 (GPIO5)**: I2C SCL (default)
- **D2 (GPIO4)**: I2C SDA (default)
- **D3 (GPIO0)**: Flash mode (must be HIGH at boot)
- **D4 (GPIO2)**: Onboard LED (active LOW), must be HIGH at boot
- **D8 (GPIO15)**: Must be LOW at boot

## Supported I2C Devices

### SSD1306 OLED Display
- **Resolution**: 128x64 or 128x32 pixels
- **Colors**: Monochrome (white, blue, or yellow)
- **Interface**: I2C
- **I2C Address**: 0x3C or 0x3D
- **Power**: 3.3V-5V
- **Current**: ~20mA

**Features:**
- High contrast display
- Wide viewing angle
- Low power consumption
- No backlight needed

### BME280 Environmental Sensor
- **Measures**: Temperature, Humidity, Pressure
- **Interface**: I2C or SPI
- **I2C Address**: 0x76 or 0x77
- **Power**: 1.71V-3.6V
- **Current**: 3.6µA @ 1Hz (typical)

**Specifications:**
- Temperature: -40 to +85°C (±1°C accuracy)
- Humidity: 0-100% RH (±3% accuracy)
- Pressure: 300-1100 hPa (±1 hPa accuracy)

### BME680 Environmental + Air Quality Sensor
- **Measures**: Temperature, Humidity, Pressure, Gas/VOC
- **Interface**: I2C or SPI
- **I2C Address**: 0x76 or 0x77
- **Power**: 1.71V-3.6V
- **Current**: ~3.7mA (gas measurement)

**Specifications:**
- Temperature: -40 to +85°C (±1°C accuracy)
- Humidity: 0-100% RH (±3% accuracy)
- Pressure: 300-1100 hPa (±1 hPa accuracy)
- Gas: 0-500°C (for air quality)

**Air Quality Features:**
- VOC (Volatile Organic Compounds) detection
- Gas resistance measurement
- Indoor air quality estimation

### INA219 Current/Power Sensor
- **Measures**: Current, Voltage, Power
- **Interface**: I2C
- **I2C Address**: 0x40-0x4F (configurable)
- **Power**: 3.0V-5.5V
- **Max Voltage**: ±26V

**Specifications:**
- Current measurement: up to ±3.2A
- Voltage measurement: 0-26V
- Shunt resistor: typically 0.1Ω
- 12-bit resolution

**Applications:**
- Battery monitoring
- Solar panel monitoring
- Power consumption measurement
- Motor current monitoring

## Non-I2C Devices

### HC-SR04 Ultrasonic Sensor
- **Measures**: Distance
- **Interface**: Digital (trigger/echo)
- **Power**: 5V (some 3.3V compatible)
- **Current**: 15mA (typical)

**Specifications:**
- Range: 2cm to 400cm
- Accuracy: ±3mm
- Measuring angle: 15 degrees
- Trigger pulse: 10µs
- Echo pulse: 150µs to 25ms

**Bridged Mode:**
When trigger and echo pins are soldered together:
- Saves one GPIO pin
- Requires careful timing
- Pin switches between output and input modes
- Slightly less reliable than separate pins

## Power Considerations

### Powering Your ESP8266
1. **USB**: Most convenient for development (5V → 3.3V regulator)
2. **3.3V directly**: Ensure current capacity (>250mA)
3. **Battery**: Use voltage regulator and consider deep sleep

### I2C Power
- Most I2C devices: 3.3V (from ESP8266 3V3 pin)
- Current draw: Usually <50mA total
- HC-SR04: Needs 5V (use 5V pin on Wemos)

### Pull-up Resistors
I2C requires pull-up resistors (typically 4.7kΩ):
- Many modules have built-in pull-ups
- ESP8266 has weak internal pull-ups
- External 4.7kΩ recommended for reliability

## Limitations and Gotchas

### GPIO Limitations
- **Limited pins**: Only ~11 usable GPIO
- **Boot modes**: Some pins must be in specific states at boot
- **No true analog**: Only one ADC pin (0-1.0V)

### Memory Constraints
- **RAM**: Only ~36KB free for user code
- **Flash**: Usually 4MB, but fragmented
- **Stack**: Can overflow easily with deep recursion

### WiFi Impact
- High current draw when transmitting
- Can cause voltage drops
- May reset ESP8266 if power supply inadequate

### Best Practices
- Use adequate power supply (>500mA capacity)
- Add decoupling capacitors (100µF + 0.1µF)
- Avoid floating pins
- Use level shifters for 5V devices if needed
- Test with stable power before adding battery

## Pinout Diagram

For detailed pinout diagrams, see [pin_reference.md](pin_reference.md).
