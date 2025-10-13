"""
Environmental Monitor with OLED Display
========================================
This example combines multiple sensors with an SSD1306 OLED display
to create a comprehensive environmental monitoring station.

Features:
- Real-time temperature, humidity, and pressure display
- Distance measurement from ultrasonic sensor
- Current and power monitoring
- Scrolling UI with multiple screens
- Visual indicators and graphs

Hardware:
- ESP8266 (Wemos D1 or D1 Mini)
- SSD1306 OLED Display (128x64)
- BME280 or BME680 Environmental Sensor
- HC-SR04 Ultrasonic Sensor (bridged)
- INA219 Current Sensor (optional)

Connections:
- I2C Devices (SSD1306, BME280/BME680, INA219):
  - SDA -> D2 (GPIO4)
  - SCL -> D1 (GPIO5)
  - VCC -> 3.3V
  - GND -> GND
- HC-SR04 (bridged):
  - TRIG/ECHO -> D7 (GPIO13)
  - VCC -> 5V
  - GND -> GND

Required libraries:
- ssd1306.py
- bme280.py (or bme680.py)
- ina219.py (optional)
"""

from machine import I2C, Pin
import ssd1306
import time

# Import sensor libraries (comment out if not available)
try:
    import bme280
    BME280_AVAILABLE = True
except:
    BME280_AVAILABLE = False
    print("BME280 library not found")

try:
    import bme680
    BME680_AVAILABLE = True
except:
    BME680_AVAILABLE = False
    print("BME680 library not found")

try:
    from ina219 import INA219
    INA219_AVAILABLE = True
except:
    INA219_AVAILABLE = False
    print("INA219 library not found")

# HC-SR04 class for bridged mode
class HCSR04Bridged:
    def __init__(self, pin):
        self.pin_num = pin
        
    def measure_distance(self):
        pin = Pin(self.pin_num, Pin.OUT)
        pin.low()
        time.sleep_us(2)
        pin.high()
        time.sleep_us(10)
        pin.low()
        
        pin = Pin(self.pin_num, Pin.IN)
        timeout = 30000
        start = time.ticks_us()
        
        while pin.value() == 0:
            if time.ticks_diff(time.ticks_us(), start) > timeout:
                return None
        pulse_start = time.ticks_us()
        
        while pin.value() == 1:
            if time.ticks_diff(time.ticks_us(), pulse_start) > timeout:
                return None
        pulse_end = time.ticks_us()
        
        pulse_duration = time.ticks_diff(pulse_end, pulse_start)
        distance = (pulse_duration * 0.0343) / 2
        return distance

# Initialize I2C
i2c = I2C(scl=Pin(5), sda=Pin(4), freq=400000)

print("Environmental Monitor")
print("Initializing sensors...")

# Scan I2C bus
devices = i2c.scan()
print(f"I2C devices found: {[hex(addr) for addr in devices]}")

# Initialize OLED display
try:
    oled = ssd1306.SSD1306_I2C(128, 64, i2c)
    oled.fill(0)
    oled.text("Initializing...", 0, 0)
    oled.show()
except Exception as e:
    print(f"OLED init failed: {e}")
    exit()

# Initialize environmental sensor
env_sensor = None
if BME680_AVAILABLE:
    try:
        env_sensor = bme680.BME680_I2C(i2c=i2c)
        env_sensor.set_humidity_oversample(bme680.OS_2X)
        env_sensor.set_pressure_oversample(bme680.OS_4X)
        env_sensor.set_temperature_oversample(bme680.OS_8X)
        env_sensor.set_filter(bme680.FILTER_SIZE_3)
        print("BME680 initialized")
    except:
        pass

if env_sensor is None and BME280_AVAILABLE:
    try:
        env_sensor = bme280.BME280(i2c=i2c)
        print("BME280 initialized")
    except:
        pass

# Initialize ultrasonic sensor
ultrasonic = HCSR04Bridged(pin=13)

# Initialize INA219 (optional)
power_sensor = None
if INA219_AVAILABLE:
    try:
        power_sensor = INA219(0.1, i2c, max_expected_amps=0.4)
        power_sensor.configure()
        print("INA219 initialized")
    except:
        pass

# UI State
current_screen = 0
last_screen_change = time.ticks_ms()
SCREEN_DURATION = 3000  # ms

# Data storage for simple graphing
temp_history = []
MAX_HISTORY = 32

def draw_screen_0():
    """Main environmental data screen"""
    oled.fill(0)
    oled.text("ENV MONITOR", 20, 0)
    
    if env_sensor:
        try:
            if BME680_AVAILABLE and isinstance(env_sensor, type(bme680.BME680_I2C)):
                if env_sensor.get_sensor_data():
                    temp = env_sensor.data.temperature
                    hum = env_sensor.data.humidity
                    pres = env_sensor.data.pressure
            else:
                temp, pres, hum = env_sensor.read_compensated_data()
                temp = temp / 100
                hum = hum / 1024
                pres = pres / 25600
            
            oled.text(f"T:{temp:.1f}C", 0, 15)
            oled.text(f"H:{hum:.1f}%", 70, 15)
            oled.text(f"P:{pres:.0f}hPa", 0, 28)
            
            # Store temperature for graph
            temp_history.append(int(temp))
            if len(temp_history) > MAX_HISTORY:
                temp_history.pop(0)
        except:
            oled.text("Sensor Error", 0, 15)
    else:
        oled.text("No Env Sensor", 0, 15)
    
    oled.show()

def draw_screen_1():
    """Distance measurement screen"""
    oled.fill(0)
    oled.text("DISTANCE", 30, 0)
    
    distance = ultrasonic.measure_distance()
    if distance:
        oled.text(f"{distance:.1f} cm", 25, 20)
        
        # Draw distance bar
        max_distance = 100
        bar_width = int(min(distance, max_distance) * 128 / max_distance)
        oled.fill_rect(0, 40, bar_width, 10, 1)
        
        # Visual indicator
        if distance < 10:
            oled.text("VERY CLOSE!", 20, 55)
        elif distance < 30:
            oled.text("CLOSE", 40, 55)
        elif distance < 50:
            oled.text("NEAR", 45, 55)
        else:
            oled.text("FAR", 50, 55)
    else:
        oled.text("OUT OF RANGE", 15, 28)
    
    oled.show()

def draw_screen_2():
    """Power monitoring screen"""
    oled.fill(0)
    oled.text("POWER", 40, 0)
    
    if power_sensor:
        try:
            voltage = power_sensor.voltage()
            current = power_sensor.current()
            power = power_sensor.power()
            
            oled.text(f"V:{voltage:.2f}V", 0, 15)
            oled.text(f"I:{current:.0f}mA", 0, 28)
            oled.text(f"P:{power:.0f}mW", 0, 41)
            
            # Power bar indicator
            max_power = 1000  # mW
            bar_width = int(min(power, max_power) * 128 / max_power)
            oled.fill_rect(0, 54, bar_width, 10, 1)
        except:
            oled.text("Sensor Error", 15, 28)
    else:
        oled.text("No INA219", 25, 28)
    
    oled.show()

def draw_screen_3():
    """Temperature graph screen"""
    oled.fill(0)
    oled.text("TEMP GRAPH", 25, 0)
    
    if len(temp_history) > 1:
        # Draw graph
        min_temp = min(temp_history)
        max_temp = max(temp_history)
        temp_range = max(max_temp - min_temp, 1)
        
        for i in range(len(temp_history) - 1):
            x1 = i * 128 // MAX_HISTORY
            x2 = (i + 1) * 128 // MAX_HISTORY
            y1 = 60 - int((temp_history[i] - min_temp) * 40 / temp_range)
            y2 = 60 - int((temp_history[i + 1] - min_temp) * 40 / temp_range)
            oled.line(x1, y1, x2, y2, 1)
        
        # Show range
        oled.text(f"{max_temp}C", 0, 10)
        oled.text(f"{min_temp}C", 0, 52)
    else:
        oled.text("Collecting", 25, 28)
        oled.text("Data...", 35, 38)
    
    oled.show()

# Main loop
print("Starting main loop...")
oled.fill(0)
oled.text("Ready!", 40, 28)
oled.show()
time.sleep(1)

screens = [draw_screen_0, draw_screen_1, draw_screen_2, draw_screen_3]

try:
    while True:
        # Draw current screen
        screens[current_screen]()
        
        # Check if it's time to change screen
        current_time = time.ticks_ms()
        if time.ticks_diff(current_time, last_screen_change) > SCREEN_DURATION:
            current_screen = (current_screen + 1) % len(screens)
            last_screen_change = current_time
        
        time.sleep(0.5)
        
except KeyboardInterrupt:
    print("\nProgram stopped")
    oled.fill(0)
    oled.text("Goodbye!", 35, 28)
    oled.show()
    time.sleep(1)
    oled.fill(0)
    oled.show()
