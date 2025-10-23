#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "pages.h"
#include "svg.h"

// Replace with your network (or use the AP fallback below)
const char* ssid = "WEEFEE";
const char* password = "12345678";

// AP fallback
const char* ap_ssid = "Offline-Selftour";
const char* ap_password = "12345678";

ESP8266WebServer server(80);

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}
void handleAbout() {
  server.send_P(200, "text/html", about_html);
}
void handleLogo() {
  server.send_P(200, "image/svg+xml", logo_svg);
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Station connect failed — starting AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  setupWiFi();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/about", HTTP_GET, handleAbout);
  server.on("/logo.svg", HTTP_GET, handleLogo);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
