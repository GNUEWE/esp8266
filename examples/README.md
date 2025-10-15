# Examples

This directory contains Arduino sketch examples for the ESP8266 (Wemos D1 and D1 Mini).

## Directory Structure

- **basic/** - Beginner-friendly examples for learning fundamentals
- **intermediate/** - More complex examples using sensors and peripherals
- **advanced/** - Complete projects combining multiple concepts

## Getting Started

If you're new to Arduino on the ESP8266:

1. Start with [basic/blink](basic/blink/blink.ino) - the classic LED blink
2. Try [basic/button](basic/button/button.ino) - reading input
3. Explore [basic/pwm_led](basic/pwm_led/pwm_led.ino) - controlling LED brightness
4. Learn ADC with [basic/adc_read](basic/adc_read/adc_read.ino)

## Running Examples

### Using Arduino IDE:
1. Open the .ino file
2. Select your board: Tools → Board → ESP8266 Boards → LOLIN(WEMOS) D1 R2 & mini
3. Select your port: Tools → Port → (your COM port)
4. Click Upload button or press Ctrl+U

### Serial Monitor:
- Open Serial Monitor: Tools → Serial Monitor
- Set baud rate to 115200
- View output from Serial.print() statements

## Hardware Requirements

Most basic examples only need:
- ESP8266 (Wemos D1 or D1 Mini)
- USB cable

Some examples may require:
- LEDs and resistors
- Buttons/switches
- Breadboard and jumper wires
- Sensors (see individual example documentation)

## Example Categories

### Basic Examples
- GPIO control (LED, buttons)
- PWM for LED fading
- ADC for reading analog sensors
- Timing and delays

### Intermediate Examples
- I2C communication (OLED, sensors)
- Distance measurement (ultrasonic)
- Environmental sensors (BME280, BME680)
- Power monitoring (INA219)
- **I2C Scanner Tool** - Scan and identify I2C devices

### Advanced Examples
- Multi-sensor projects
- Real-time displays with UI
- Data visualization
- Environmental monitoring station

## Supported Hardware

### I2C Devices
- **SSD1306** - OLED Display (128x64, 128x32)
- **BME280** - Temperature, Humidity, Pressure Sensor
- **BME680** - Air Quality + Environmental Sensor
- **INA219** - Current and Power Sensor

### Other Devices
- **HC-SR04** - Ultrasonic Distance Sensor (with bridged echo/trigger)

## Required Libraries

Most intermediate and advanced examples require external libraries. Install them via Arduino IDE:

**Tools → Manage Libraries → Search and Install:**
- Adafruit GFX Library
- Adafruit SSD1306
- Adafruit BME280 Library
- Adafruit BME680 Library
- Adafruit INA219
- Adafruit Unified Sensor

## Learning Path

1. **Week 1**: Basic examples → Understand GPIO and timing
2. **Week 2**: Intermediate examples → Learn I2C and sensors
3. **Week 3**: Advanced examples → Build complete projects
4. **Week 4**: Create your own projects!

## Need Help?

- Check the [tutorials](../tutorials/) directory
- Read the [documentation](../docs/)
- Open an issue on GitHub

## Contributing

Found a bug? Have an example to share? Pull requests are welcome!
