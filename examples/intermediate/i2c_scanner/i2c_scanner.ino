/*
 * I2C Scanner with Optional SSD1306 Display
 * ==========================================
 * This utility scans the I2C bus for connected devices and displays
 * the results on both Serial Monitor and optionally on an SSD1306 OLED display.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - Optional: SSD1306 OLED Display
 * 
 * Connections:
 * - SDA -> D2 (GPIO4)
 * - SCL -> D1 (GPIO5)
 * - VCC -> 3.3V
 * - GND -> GND
 * 
 * Library Requirements:
 * - Wire (built-in)
 * - Adafruit GFX Library (if using display)
 * - Adafruit SSD1306 Library (if using display)
 * 
 * Install via: Tools -> Manage Libraries -> Search for "Adafruit SSD1306"
 */

#include <Wire.h>

// Uncomment the following lines if you have an SSD1306 display attached
// #define USE_DISPLAY
#ifdef USE_DISPLAY
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 64
  #define OLED_RESET -1
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

#define SDA_PIN 4  // D2 / GPIO4
#define SCL_PIN 5  // D1 / GPIO5

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=================================");
  Serial.println("I2C Scanner for ESP8266");
  Serial.println("=================================\n");
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  #ifdef USE_DISPLAY
    // Try to initialize display
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("SSD1306 display found!");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("I2C Scanner");
      display.println("ESP8266");
      display.display();
      delay(2000);
    } else {
      Serial.println("SSD1306 display not found");
      Serial.println("Continuing with Serial only...");
    }
  #endif
  
  Serial.println("Scanning I2C bus...\n");
}

void loop() {
  byte error, address;
  int devicesFound = 0;
  
  Serial.println("Scanning...");
  
  #ifdef USE_DISPLAY
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("I2C Scan:");
    display.display();
  #endif
  
  // Scan addresses 1 to 127
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      // Device found
      devicesFound++;
      
      // Print to Serial
      Serial.print("Device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.print(" (");
      Serial.print(address);
      Serial.print(")");
      
      // Identify common devices
      String deviceName = identifyDevice(address);
      if (deviceName != "") {
        Serial.print(" - ");
        Serial.print(deviceName);
      }
      Serial.println();
      
      // Display on OLED if available
      #ifdef USE_DISPLAY
        display.print("0x");
        if (address < 16) display.print("0");
        display.print(address, HEX);
        if (deviceName != "") {
          display.print(" ");
          display.println(deviceName);
        } else {
          display.println();
        }
        display.display();
      #endif
      
      delay(10);
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  // Summary
  Serial.println("\n--- Scan Complete ---");
  Serial.print("Devices found: ");
  Serial.println(devicesFound);
  
  if (devicesFound == 0) {
    Serial.println("\nNo I2C devices found!");
    Serial.println("Check your connections:");
    Serial.println("  SDA -> D2 (GPIO4)");
    Serial.println("  SCL -> D1 (GPIO5)");
    
    #ifdef USE_DISPLAY
      display.println("\nNo devices!");
      display.display();
    #endif
  } else {
    #ifdef USE_DISPLAY
      display.print("\nTotal: ");
      display.println(devicesFound);
      display.display();
    #endif
  }
  
  Serial.println("\nWaiting 5 seconds before next scan...\n");
  delay(5000);
}

// Helper function to identify common I2C devices
String identifyDevice(byte address) {
  switch (address) {
    case 0x3C:
    case 0x3D:
      return "SSD1306";
    case 0x76:
    case 0x77:
      return "BME280/BME680";
    case 0x40:
      return "INA219";
    case 0x48:
      return "ADS1115";
    case 0x68:
      return "DS1307/MPU6050";
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
      return "EEPROM";
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
      return "PCF8574/LCD";
    default:
      return "";
  }
}
