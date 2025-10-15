/*
 * INA219 Current Sensor Example for ESP8266
 * ==========================================
 * This example demonstrates reading voltage, current, and power
 * from an INA219 sensor via I2C.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - INA219 Current/Power Sensor
 * 
 * Connections:
 * - Sensor SDA -> D2 (GPIO4)
 * - Sensor SCL -> D1 (GPIO5)
 * - Sensor VCC -> 3.3V
 * - Sensor GND -> GND
 * - Sensor VIN+ -> Positive side of load/power supply
 * - Sensor VIN- -> Negative side of load
 * 
 * Library Requirements:
 * - Adafruit INA219 Library
 * 
 * Install via: Tools -> Manage Libraries -> Search for "Adafruit INA219"
 */

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

float totalEnergy = 0.0;  // mWh
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\nINA219 Current Sensor Example");
  Serial.println("==============================\n");
  
  // Initialize INA219
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    Serial.println("Check wiring and I2C address");
    while (1) delay(10);
  }
  
  Serial.println("INA219 sensor found and initialized");
  Serial.println("Reading voltage, current, and power...\n");
  
  lastTime = millis();
}

void loop() {
  float shuntVoltage = ina219.getShuntVoltage_mV();
  float busVoltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();
  float power = ina219.getPower_mW();
  float loadVoltage = busVoltage + (shuntVoltage / 1000);
  
  // Calculate time elapsed
  unsigned long currentTime = millis();
  float timeDiff = (currentTime - lastTime) / 3600000.0;  // Convert to hours
  lastTime = currentTime;
  
  // Calculate energy consumed
  totalEnergy += power * timeDiff;  // mWh
  
  // Display values
  Serial.println("--- INA219 Readings ---");
  Serial.print("Bus Voltage:    ");
  Serial.print(busVoltage);
  Serial.println(" V");
  
  Serial.print("Shunt Voltage:  ");
  Serial.print(shuntVoltage);
  Serial.println(" mV");
  
  Serial.print("Load Voltage:   ");
  Serial.print(loadVoltage);
  Serial.println(" V");
  
  Serial.print("Current:        ");
  Serial.print(current);
  Serial.println(" mA");
  
  Serial.print("Power:          ");
  Serial.print(power);
  Serial.println(" mW");
  
  Serial.print("Total Energy:   ");
  Serial.print(totalEnergy);
  Serial.print(" mWh (");
  Serial.print(totalEnergy / 1000.0, 4);
  Serial.println(" Wh)");
  
  Serial.println("------------------------\n");
  
  delay(1000);
}
