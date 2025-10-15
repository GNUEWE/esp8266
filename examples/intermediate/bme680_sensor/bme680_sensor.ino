/*
 * BME680 Sensor Example for ESP8266
 * ==================================
 * This example demonstrates reading temperature, humidity, pressure, and
 * gas resistance from a BME680 sensor via I2C.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - BME680 Environmental Sensor (Air Quality)
 * 
 * Connections:
 * - Sensor SDA -> D2 (GPIO4)
 * - Sensor SCL -> D1 (GPIO5)
 * - Sensor VCC -> 3.3V
 * - Sensor GND -> GND
 * 
 * Library Requirements:
 * - Adafruit BME680 Library
 * - Adafruit Unified Sensor Library
 * 
 * Install via: Tools -> Manage Libraries -> Search for "Adafruit BME680"
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\nBME680 Sensor Example");
  Serial.println("=====================\n");
  
  // Initialize BME680
  if (!bme.begin(0x76)) {  // Try address 0x76 first
    if (!bme.begin(0x77)) {  // Try address 0x77
      Serial.println("Could not find BME680 sensor!");
      Serial.println("Check wiring and I2C address");
      while (1) delay(10);
    }
  }
  
  Serial.println("BME680 sensor found and initialized");
  
  // Configure sensor
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320°C for 150 ms
  
  Serial.println("Reading temperature, humidity, pressure, and gas resistance...");
  Serial.println("Note: Gas readings stabilize after a few minutes\n");
}

void loop() {
  // Perform a reading
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading");
    delay(2000);
    return;
  }
  
  // Read sensor values
  float temperature = bme.temperature;
  float humidity = bme.humidity;
  float pressure = bme.pressure / 100.0;
  float gasResistance = bme.gas_resistance / 1000.0;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  // Display values
  Serial.println("--- BME680 Readings ---");
  Serial.print("Temperature:    ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity:       ");
  Serial.print(humidity);
  Serial.println(" %");
  
  Serial.print("Pressure:       ");
  Serial.print(pressure);
  Serial.println(" hPa");
  
  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" KOhms");
  
  Serial.print("Altitude:       ");
  Serial.print(altitude);
  Serial.println(" m");
  
  // Calculate simple air quality index
  if (gasResistance > 50) {
    float aqi = min(100.0, (gasResistance / 50.0) * 100.0);
    Serial.print("Air Quality:    ");
    Serial.print(aqi);
    Serial.println("% (approx)");
  }
  
  Serial.println("------------------------\n");
  
  delay(2000);
}
