/*
 * BME280 Sensor Example for ESP8266
 * ==================================
 * This example demonstrates reading temperature, humidity, and pressure
 * from a BME280 sensor via I2C.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - BME280 Environmental Sensor
 * 
 * Connections:
 * - Sensor SDA -> D2 (GPIO4)
 * - Sensor SCL -> D1 (GPIO5)
 * - Sensor VCC -> 3.3V
 * - Sensor GND -> GND
 * 
 * Library Requirements:
 * - Adafruit BME280 Library
 * - Adafruit Unified Sensor Library
 * 
 * Install via: Tools -> Manage Libraries -> Search for "Adafruit BME280"
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\nBME280 Sensor Example");
  Serial.println("=====================\n");
  
  // Initialize BME280
  if (!bme.begin(0x76)) {  // Try address 0x76 first
    if (!bme.begin(0x77)) {  // Try address 0x77
      Serial.println("Could not find BME280 sensor!");
      Serial.println("Check wiring and I2C address");
      while (1) delay(10);
    }
  }
  
  Serial.println("BME280 sensor found and initialized");
  Serial.println("Reading temperature, humidity, and pressure...\n");
  
  // Configure sensor
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X2,  // Temperature
                  Adafruit_BME280::SAMPLING_X16, // Pressure
                  Adafruit_BME280::SAMPLING_X1,  // Humidity
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_500);
}

void loop() {
  // Read sensor values
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  // Display values
  Serial.println("--- BME280 Readings ---");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity:    ");
  Serial.print(humidity);
  Serial.println(" %");
  
  Serial.print("Pressure:    ");
  Serial.print(pressure);
  Serial.println(" hPa");
  
  Serial.print("Altitude:    ");
  Serial.print(altitude);
  Serial.println(" m");
  
  Serial.println("------------------------\n");
  
  delay(2000);
}
