/*
 * HC-SR04 Ultrasonic Sensor Example (Bridged Echo/Trigger)
 * =========================================================
 * This example demonstrates measuring distance using an HC-SR04 ultrasonic sensor
 * with the Echo and Trigger pins soldered/bridged together.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - HC-SR04 Ultrasonic Sensor (with Echo and Trigger bridged)
 * 
 * Connections:
 * - Sensor VCC -> 5V (or 3.3V for 3.3V compatible modules)
 * - Sensor GND -> GND
 * - Sensor TRIG/ECHO (bridged) -> D7 (GPIO13)
 * 
 * Note: When Echo and Trigger are bridged, the same pin is used for both
 * sending the trigger pulse and receiving the echo response.
 * This saves one GPIO pin but requires careful timing.
 */

#define TRIG_ECHO_PIN 13  // D7 / GPIO13 (bridged trigger and echo)

void setup() {
  Serial.begin(115200);
  
  Serial.println("\nHC-SR04 Ultrasonic Sensor (Bridged Mode)");
  Serial.println("=========================================");
  Serial.println("Measuring distance...\n");
}

void loop() {
  float distance = measureDistance();
  
  if (distance > 0 && distance < 400) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm (");
    Serial.print(distance / 2.54);
    Serial.println(" inches)");
    
    // Visual distance indicator
    if (distance < 10) {
      Serial.println("*** VERY CLOSE ***");
    } else if (distance < 20) {
      Serial.println("** CLOSE **");
    } else if (distance < 50) {
      Serial.println("* Near *");
    } else if (distance < 100) {
      Serial.println("- Medium -");
    } else {
      Serial.println("  Far");
    }
  } else {
    Serial.println("Out of range or timeout");
  }
  
  Serial.println("------------------------");
  delay(500);
}

float measureDistance() {
  // Set pin as output for trigger
  pinMode(TRIG_ECHO_PIN, OUTPUT);
  
  // Send trigger pulse
  digitalWrite(TRIG_ECHO_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_ECHO_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_ECHO_PIN, LOW);
  
  // Quickly switch to input mode to receive echo
  pinMode(TRIG_ECHO_PIN, INPUT);
  
  // Measure echo pulse duration
  long duration = pulseIn(TRIG_ECHO_PIN, HIGH, 30000);  // 30ms timeout
  
  if (duration == 0) {
    return -1;  // Timeout
  }
  
  // Calculate distance (speed of sound = 343 m/s)
  // Distance = (duration * 0.0343) / 2
  float distance = duration * 0.01715;
  
  return distance;
}
