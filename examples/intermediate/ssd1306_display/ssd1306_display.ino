/*
 * SSD1306 OLED Display Example for ESP8266
 * =========================================
 * This example demonstrates using an SSD1306 OLED display via I2C.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - SSD1306 OLED Display (128x64 or 128x32)
 * 
 * Connections:
 * - Display SDA -> D2 (GPIO4)
 * - Display SCL -> D1 (GPIO5)
 * - Display VCC -> 3.3V
 * - Display GND -> GND
 * 
 * Library Requirements:
 * - Adafruit GFX Library
 * - Adafruit SSD1306 Library
 * 
 * Install via: Tools -> Manage Libraries -> Search for "Adafruit SSD1306"
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  
  Serial.println("\nSSD1306 OLED Display Example");
  
  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);  // Don't proceed, loop forever
  }
  
  Serial.println("Display initialized successfully");
  
  // Clear display
  display.clearDisplay();
  display.display();
  delay(500);
}

void loop() {
  // Demo 1: Text display
  textDemo();
  delay(2000);
  
  // Demo 2: Shapes
  shapesDemo();
  delay(2000);
  
  // Demo 3: Scrolling text
  scrollingTextDemo();
  delay(1000);
  
  // Demo 4: Counter
  counterDemo();
  delay(3000);
}

void textDemo() {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello, ESP8266!");
  display.println();
  display.println("SSD1306 OLED");
  display.setTextSize(2);
  display.println("Display");
  
  display.display();
}

void shapesDemo() {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Drawing Shapes");
  
  // Draw rectangles
  display.drawRect(10, 20, 30, 20, SSD1306_WHITE);
  display.fillRect(50, 20, 30, 20, SSD1306_WHITE);
  
  // Draw lines
  display.drawLine(0, 50, 127, 50, SSD1306_WHITE);
  display.drawLine(64, 0, 64, 63, SSD1306_WHITE);
  
  display.display();
}

void scrollingTextDemo() {
  String text = "ESP8266 + SSD1306 = Awesome!";
  int textWidth = text.length() * 6;
  
  for (int x = SCREEN_WIDTH; x > -textWidth; x -= 2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(x, 28);
    display.print(text);
    display.display();
    delay(20);
  }
}

void counterDemo() {
  for (int count = 0; count < 10; count++) {
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("Counter:");
    
    display.setTextSize(3);
    display.setCursor(40, 30);
    display.println(count);
    
    display.display();
    delay(500);
  }
}
