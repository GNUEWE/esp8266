/*
 * Blink LED Example for ESP8266
 * ==============================
 * This example demonstrates the classic "Hello World" of microcontrollers:
 * blinking the onboard LED on the ESP8266.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * 
 * The onboard LED is connected to GPIO2 (D4 on Wemos D1 Mini).
 * Note: The LED is active LOW (LOW = on, HIGH = off)
 */

#define LED_PIN 2  // GPIO2 / D4 (onboard LED)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("\nStarting LED blink...");
  Serial.println("Press Reset to stop");
}

void loop() {
  digitalWrite(LED_PIN, LOW);   // Turn LED on (active LOW)
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(LED_PIN, HIGH);  // Turn LED off (active LOW)
  Serial.println("LED OFF");
  delay(500);
}
