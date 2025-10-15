/*
 * Environmental Monitor with OLED Display
 * ========================================
 * This example combines multiple sensors with an SSD1306 OLED display
 * to create a comprehensive environmental monitoring station.
 * 
 * Features:
 * - Real-time temperature, humidity, and pressure display
 * - Distance measurement from ultrasonic sensor
 * - Current and power monitoring
 * - Scrolling UI with multiple screens
 * - Visual indicators
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - SSD1306 OLED Display (128x64)
 * - BME280 or BME680 Environmental Sensor
 * - HC-SR04 Ultrasonic Sensor (bridged)
 * - INA219 Current Sensor (optional)
 * 
 * Connections:
 * - I2C Devices (SSD1306, BME280/BME680, INA219):
 *   - SDA -> D2 (GPIO4)
 *   - SCL -> D1 (GPIO5)
 *   - VCC -> 3.3V
 *   - GND -> GND
 * - HC-SR04 (bridged):
 *   - TRIG/ECHO -> D7 (GPIO13)
 *   - VCC -> 5V
 *   - GND -> GND
 * 
 * Library Requirements:
 * - Adafruit GFX Library
 * - Adafruit SSD1306 Library
 * - Adafruit BME280 Library (or BME680)
 * - Adafruit INA219 Library (optional)
 * - Adafruit Unified Sensor Library
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// Uncomment for BME680: #include "Adafruit_BME680.h"
#include <Adafruit_INA219.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define TRIG_ECHO_PIN 13  // D7 / GPIO13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
// Uncomment for BME680: Adafruit_BME680 bme;
Adafruit_INA219 ina219;

bool hasBME = false;
bool hasINA = false;

int currentScreen = 0;
unsigned long lastScreenChange = 0;
const unsigned long SCREEN_DURATION = 3000;  // ms

// Temperature history for graphing
const int MAX_HISTORY = 32;
int tempHistory[MAX_HISTORY];
int historyIndex = 0;

void setup() {
  Serial.begin(115200);
  
  Serial.println("\nEnvironmental Monitor");
  Serial.println("=====================");
  Serial.println("Initializing sensors...\n");
  
  // Initialize I2C
  Wire.begin(4, 5);  // SDA=GPIO4, SCL=GPIO5
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  
  // Initialize BME280/BME680
  if (bme.begin(0x76) || bme.begin(0x77)) {
    hasBME = true;
    Serial.println("BME sensor initialized");
  } else {
    Serial.println("BME sensor not found");
  }
  
  // Initialize INA219
  if (ina219.begin()) {
    hasINA = true;
    Serial.println("INA219 initialized");
  } else {
    Serial.println("INA219 not found");
  }
  
  // Initialize temperature history
  for (int i = 0; i < MAX_HISTORY; i++) {
    tempHistory[i] = 0;
  }
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Ready!");
  display.display();
  delay(1000);
  
  Serial.println("\nStarting main loop...");
}

void loop() {
  // Draw current screen
  switch (currentScreen) {
    case 0:
      drawEnvironmentalScreen();
      break;
    case 1:
      drawDistanceScreen();
      break;
    case 2:
      if (hasINA) {
        drawPowerScreen();
      } else {
        currentScreen = 3;  // Skip if no sensor
      }
      break;
    case 3:
      if (hasBME) {
        drawGraphScreen();
      } else {
        currentScreen = 0;  // Skip if no sensor
      }
      break;
  }
  
  // Check if it's time to change screen
  if (millis() - lastScreenChange > SCREEN_DURATION) {
    currentScreen = (currentScreen + 1) % 4;
    lastScreenChange = millis();
  }
  
  delay(500);
}

void drawEnvironmentalScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println("ENV MONITOR");
  
  if (hasBME) {
    float temp = bme.readTemperature();
    float hum = bme.readHumidity();
    float pres = bme.readPressure() / 100.0;
    
    // Store temperature for graph
    tempHistory[historyIndex] = (int)temp;
    historyIndex = (historyIndex + 1) % MAX_HISTORY;
    
    display.setCursor(0, 15);
    display.print("T:");
    display.print(temp, 1);
    display.print("C");
    
    display.setCursor(70, 15);
    display.print("H:");
    display.print(hum, 0);
    display.print("%");
    
    display.setCursor(0, 28);
    display.print("P:");
    display.print(pres, 0);
    display.print("hPa");
  } else {
    display.setCursor(0, 15);
    display.println("No BME Sensor");
  }
  
  display.display();
}

void drawDistanceScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 0);
  display.println("DISTANCE");
  
  float distance = measureDistance();
  
  if (distance > 0 && distance < 400) {
    display.setTextSize(2);
    display.setCursor(25, 20);
    display.print(distance, 0);
    display.println(" cm");
    
    // Draw distance bar
    int barWidth = map(distance, 0, 100, 0, 128);
    barWidth = constrain(barWidth, 0, 128);
    display.fillRect(0, 40, barWidth, 10, SSD1306_WHITE);
    
    // Visual indicator
    display.setTextSize(1);
    display.setCursor(0, 55);
    if (distance < 10) {
      display.println("VERY CLOSE!");
    } else if (distance < 30) {
      display.println("   CLOSE");
    } else if (distance < 50) {
      display.println("   NEAR");
    } else {
      display.println("    FAR");
    }
  } else {
    display.setTextSize(1);
    display.setCursor(15, 28);
    display.println("OUT OF RANGE");
  }
  
  display.display();
}

void drawPowerScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(40, 0);
  display.println("POWER");
  
  float voltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();
  float power = ina219.getPower_mW();
  
  display.setCursor(0, 15);
  display.print("V:");
  display.print(voltage, 2);
  display.print("V");
  
  display.setCursor(0, 28);
  display.print("I:");
  display.print(current, 0);
  display.print("mA");
  
  display.setCursor(0, 41);
  display.print("P:");
  display.print(power, 0);
  display.print("mW");
  
  // Power bar indicator
  int barWidth = map(power, 0, 1000, 0, 128);
  barWidth = constrain(barWidth, 0, 128);
  display.fillRect(0, 54, barWidth, 10, SSD1306_WHITE);
  
  display.display();
}

void drawGraphScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25, 0);
  display.println("TEMP GRAPH");
  
  // Find min and max
  int minTemp = tempHistory[0];
  int maxTemp = tempHistory[0];
  for (int i = 1; i < MAX_HISTORY; i++) {
    if (tempHistory[i] < minTemp) minTemp = tempHistory[i];
    if (tempHistory[i] > maxTemp) maxTemp = tempHistory[i];
  }
  
  int tempRange = maxTemp - minTemp;
  if (tempRange == 0) tempRange = 1;
  
  // Draw graph
  for (int i = 0; i < MAX_HISTORY - 1; i++) {
    int x1 = i * 128 / MAX_HISTORY;
    int x2 = (i + 1) * 128 / MAX_HISTORY;
    int y1 = 60 - ((tempHistory[i] - minTemp) * 40 / tempRange);
    int y2 = 60 - ((tempHistory[(i + 1) % MAX_HISTORY] - minTemp) * 40 / tempRange);
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
  
  // Show range
  display.setCursor(0, 10);
  display.print(maxTemp);
  display.print("C");
  
  display.setCursor(0, 52);
  display.print(minTemp);
  display.print("C");
  
  display.display();
}

float measureDistance() {
  pinMode(TRIG_ECHO_PIN, OUTPUT);
  digitalWrite(TRIG_ECHO_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_ECHO_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_ECHO_PIN, LOW);
  
  pinMode(TRIG_ECHO_PIN, INPUT);
  long duration = pulseIn(TRIG_ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) return -1;
  
  return duration * 0.01715;
}
