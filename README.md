# ESP8266 MicroPython Projects

Welcome to the ESP8266 MicroPython project repository! This repository contains tutorials, examples, and scripts for programming ESP8266 boards (Wemos D1, D1 Mini) using MicroPython.

## 🎯 About

The ESP8266 is a low-cost Wi-Fi microcontroller chip with a full TCP/IP stack. This repository provides:

- **Example Scripts**: Ready-to-use MicroPython scripts for common tasks
- **Tutorials**: Step-by-step guides for learning MicroPython on the ESP8266
- **Libraries**: Helper libraries and utilities for ESP8266 development
- **Documentation**: Comprehensive guides and resources

## 🚀 Getting Started

### Prerequisites

- ESP8266 board (Wemos D1 or D1 Mini)
- Micro USB cable
- Computer with USB port
- MicroPython firmware for ESP8266

### Quick Start

1. **Install MicroPython on your ESP8266**
   - Download the latest MicroPython firmware from [micropython.org](https://micropython.org/download/esp8266/)
   - Install esptool: `pip install esptool`
   - Erase flash: `esptool.py --port /dev/ttyUSB0 erase_flash`
   - Flash firmware: `esptool.py --port /dev/ttyUSB0 --baud 460800 write_flash --flash_size=detect 0 esp8266-*.bin`

2. **Connect to your ESP8266**
   - Use Thonny IDE (recommended for beginners) or any serial terminal
   - Select the MicroPython (ESP8266) interpreter
   - Connect via the USB serial port (usually /dev/ttyUSB0 on Linux, COM port on Windows)

3. **Run your first program**
   - Try the examples in the `examples/basic/` directory
   - Start with `blink.py` to test your setup

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

Check out the `examples/` directory for ready-to-use scripts:

- **Basic**: LED blinking, button input, PWM, ADC readings
- **Intermediate**: I2C sensors (SSD1306, BME280, BME680, INA219), ultrasonic sensor
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

## 📖 Documentation

Detailed documentation is available in the `docs/` directory:

- Hardware specifications
- Pin diagrams and references
- API documentation
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

- [ESP8266 Documentation](https://docs.micropython.org/en/latest/esp8266/quickref.html)
- [MicroPython Documentation](https://docs.micropython.org/en/latest/)
- [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- [Wemos D1 Mini Pinout](https://www.wemos.cc/en/latest/d1/d1_mini.html)

## 💬 Support

For questions and support:
- Check the documentation in the `docs/` directory
- Browse existing issues
- Open a new issue if needed

Happy coding! 🎉
