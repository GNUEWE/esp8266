# ESP8266 Arduino Sketches

Welcome to the ESP8266 Arduino sketches repository! This repository contains tutorials, examples, and sketches for programming ESP8266 boards (Wemos D1, D1 Mini) using the Arduino IDE.

## 🎯 About

The ESP8266 is a low-cost Wi-Fi microcontroller chip with a full TCP/IP stack. This repository provides:

- **Example Sketches**: Ready-to-use Arduino sketches (.ino files) for common tasks
- **Tutorials**: Step-by-step guides for learning Arduino programming on the ESP8266
- **Libraries**: Helper libraries and utilities for ESP8266 development
- **Documentation**: Comprehensive guides and resources

## 🚀 Getting Started

### Prerequisites

- ESP8266 board (Wemos D1 or D1 Mini)
- Micro USB cable
- Computer with USB port
- Arduino IDE (1.8.x or 2.x)

### Quick Start

1. **Install Arduino IDE and ESP8266 Board Support**
   - Download and install Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs": `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Go to Tools → Board → Boards Manager
   - Search for "esp8266" and install "esp8266 by ESP8266 Community"

2. **Configure Arduino IDE for ESP8266**
   - Go to Tools → Board → ESP8266 Boards → select your board (e.g., "LOLIN(WEMOS) D1 R2 & mini")
   - Select your COM port under Tools → Port
   - Set upload speed to 115200 (or 921600 for faster uploads)

3. **Run your first program**
   - Try the examples in the `examples/basic/` directory
   - Start with `blink/blink.ino` to test your setup

## 📁 Project Structure

```
esp8266/
├── examples/           # Example scripts
│   ├── basic/         # Basic examples for beginners
│   ├── intermediate/  # Intermediate level examples
│   └── advanced/      # Advanced projects
├── tutorials/         # Step-by-step tutorials
├── libs/              # Helper libraries and utilities
├── docs/              # Documentation
├── utils/             # Utility scripts and tools
└── README.md          # This file
```

## 💡 Examples

Check out the `examples/` directory for ready-to-use Arduino sketches:

- **Basic**: LED blinking, button input, PWM, ADC readings
- **Intermediate**: I2C sensors (SSD1306, BME280, BME680, INA219), ultrasonic sensor, I2C scanner tool
- **Advanced**: Complete projects with UI combining multiple components

## 📚 Hardware Support

This repository includes examples for:

### Boards
- Wemos D1
- Wemos D1 Mini

### I2C Devices
- **SSD1306** - OLED Display (128x64, 128x32)
- **BME280** - Temperature, Humidity, Pressure Sensor
- **BME680** - Air Quality, Temperature, Humidity, Pressure Sensor
- **INA219** - Current and Power Sensor

### Non-I2C Devices
- **HC-SR04** - Ultrasonic Distance Sensor (with bridged echo/trigger option)

### Utility Tools
- **I2C Scanner** - Scan and identify I2C devices, with optional SSD1306 display output

## 📖 Documentation

Detailed documentation is available in the `docs/` directory:

- Hardware specifications
- Pin diagrams and references
- Troubleshooting guides
- Best practices

See [docs/README.md](docs/README.md) for a complete documentation index.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests, report bugs, or suggest new features.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

## 🔗 Useful Links

- [ESP8266 Arduino Core Documentation](https://arduino-esp8266.readthedocs.io/)
- [Arduino ESP8266 Libraries](https://github.com/esp8266/Arduino/tree/master/libraries)
- [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- [Wemos D1 Mini Pinout](https://www.wemos.cc/en/latest/d1/d1_mini.html)

## 💬 Support

For questions and support:
- Check the documentation in the `docs/` directory
- Browse existing issues
- Open a new issue if needed

Happy coding! 🎉
