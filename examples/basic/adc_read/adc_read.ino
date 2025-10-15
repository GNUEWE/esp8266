/*
 * ADC Read Example for ESP8266
 * =============================
 * This example demonstrates reading analog values from the ADC pin.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - Optional: Potentiometer or analog sensor on A0
 * 
 * Connections (if using potentiometer):
 * - Potentiometer: Outer pins to 3.3V and GND, middle pin to A0
 * 
 * Note: ESP8266 has only one ADC pin (A0) with range 0-1.0V (0-1023 digital)
 * For higher voltages, use a voltage divider.
 */

void setup() {
  Serial.begin(115200);
  
  Serial.println("\nADC Read Example");
  Serial.println("Reading values from A0 pin");
  Serial.println("Range: 0-1023 (0-1.0V)");
}

void loop() {
  // Read ADC value (0-1023)
  int value = analogRead(A0);
  
  // Convert to voltage (0-1.0V)
  float voltage = value / 1023.0;
  
  // Display values
  Serial.print("ADC: ");
  Serial.print(value);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.println("V");
  
  delay(500);
}
