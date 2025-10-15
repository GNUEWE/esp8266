/*
 * PWM LED Example for ESP8266
 * ============================
 * This example demonstrates using PWM to fade an LED in and out.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - External LED and 220Ω resistor on D6 (GPIO12)
 * 
 * Connections:
 * - LED anode to D6 through 220Ω resistor
 * - LED cathode to GND
 * 
 * Note: The onboard LED (GPIO2) also supports PWM if you prefer to use it.
 */

#define LED_PIN 12  // D6 / GPIO12

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("\nPWM LED Fade Example");
}

void loop() {
  // Fade in
  for (int brightness = 0; brightness <= 1023; brightness += 8) {
    analogWrite(LED_PIN, brightness);
    delay(10);
  }
  
  // Fade out
  for (int brightness = 1023; brightness >= 0; brightness -= 8) {
    analogWrite(LED_PIN, brightness);
    delay(10);
  }
}
