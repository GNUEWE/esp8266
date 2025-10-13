# Examples

This directory contains MicroPython example scripts for the ESP8266 (Wemos D1 and D1 Mini).

## Directory Structure

- **basic/** - Beginner-friendly examples for learning fundamentals
- **intermediate/** - More complex examples using sensors and peripherals
- **advanced/** - Complete projects combining multiple concepts

## Getting Started

If you're new to MicroPython on the ESP8266:

1. Start with [basic/blink.py](basic/blink.py) - the classic LED blink
2. Try [basic/button.py](basic/button.py) - reading input
3. Explore [basic/pwm_led.py](basic/pwm_led.py) - controlling LED brightness
4. Learn ADC with [basic/adc_read.py](basic/adc_read.py)

## Running Examples

### Using Thonny IDE:
1. Open the example file
2. Save it to your ESP8266 (File → Save As → MicroPython device)
3. Run it (F5 or green Run button)

### Using mpremote:
```bash
mpremote connect /dev/ttyUSB0 run examples/basic/blink.py
```

### Using ampy:
```bash
ampy --port /dev/ttyUSB0 put examples/basic/blink.py
```

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
