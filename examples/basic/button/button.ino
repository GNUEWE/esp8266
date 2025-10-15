/*
 * Button Input Example for ESP8266
 * =================================
 * This example demonstrates reading button input and controlling an LED.
 * 
 * Hardware:
 * - ESP8266 (Wemos D1 or D1 Mini)
 * - Push button connected to D5 (GPIO14)
 * - Optional: External LED and resistor on D6 (GPIO12)
 * 
 * Connections:
 * - Button: One side to D5, other side to GND
 * - LED (optional): Anode to D6 through 220Ω resistor, cathode to GND
 * 
 * The internal pull-up resistor is used, so button reads HIGH when not pressed.
 */

#define BUTTON_PIN 14  // D5 / GPIO14
#define LED_PIN 2      // D4 / GPIO2 (onboard LED, active LOW)

bool lastButtonState = HIGH;
bool ledState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED off initially (active LOW)
  
  Serial.println("\nButton Example");
  Serial.println("Press the button to toggle the LED");
}

void loop() {
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // Detect button press (transition from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastDebounceTime > debounceDelay) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? LOW : HIGH);  // Active LOW
      Serial.println(ledState ? "LED ON" : "LED OFF");
      lastDebounceTime = currentTime;
    }
  }
  
  lastButtonState = currentButtonState;
  delay(10);
}
