#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// BME280 sensor instance
Adafruit_BME280 bme;

// Sensor reading variables
float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float altitude = 0.0;
bool sensorAvailable = false;

// Initialize BME280 sensor
bool initBME280() {
  // Try to initialize with I2C address 0x76 (default)
  if (bme.begin(0x76)) {
    sensorAvailable = true;
    Serial.println("BME280 sensor found at address 0x76");
    return true;
  }
  
  // Try alternate I2C address 0x77
  if (bme.begin(0x77)) {
    sensorAvailable = true;
    Serial.println("BME280 sensor found at address 0x77");
    return true;
  }
  
  sensorAvailable = false;
  Serial.println("BME280 sensor not found - weather page will show unavailable");
  return false;
}

// Read sensor data
void readBME280() {
  if (!sensorAvailable) {
    return;
  }
  
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F; // Convert to hPa
  altitude = bme.readAltitude(1013.25); // Sea level pressure
}

// Convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
  return (celsius * 9.0 / 5.0) + 32.0;
}

// Generate weather page HTML with current readings
String generateWeatherHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Current Weather - Roosevelt Lake</title>
  <style>
    body {
      font-family: Georgia, serif;
      max-width: 900px;
      margin: 0 auto;
      padding: 20px;
      background: linear-gradient(to bottom, #87CEEB 0%, #E0F6FF 100%);
    }
    .container {
      background-color: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
    }
    h1 {
      color: #2C5F2D;
      border-bottom: 3px solid #4A7C59;
      padding-bottom: 10px;
    }
    nav {
      margin: 20px 0;
      background-color: #4A7C59;
      padding: 10px;
      border-radius: 5px;
    }
    nav a {
      color: white;
      text-decoration: none;
      margin-right: 15px;
      padding: 8px 15px;
      border-radius: 5px;
      transition: background-color 0.3s;
      display: inline-block;
    }
    nav a:hover {
      background-color: #2C5F2D;
    }
    .weather-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 20px;
      margin: 30px 0;
    }
    .weather-card {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      padding: 25px;
      border-radius: 15px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
      text-align: center;
    }
    .weather-card.temp {
      background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
    }
    .weather-card.humidity {
      background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
    }
    .weather-card.pressure {
      background: linear-gradient(135deg, #43e97b 0%, #38f9d7 100%);
    }
    .weather-value {
      font-size: 3em;
      font-weight: bold;
      margin: 15px 0;
    }
    .weather-label {
      font-size: 1.2em;
      opacity: 0.9;
    }
    .weather-unit {
      font-size: 0.6em;
      opacity: 0.8;
    }
    .info-box {
      background-color: #E8F5E9;
      padding: 20px;
      border-left: 4px solid #4A7C59;
      margin: 20px 0;
      border-radius: 5px;
    }
    .refresh-note {
      text-align: center;
      color: #666;
      font-style: italic;
      margin-top: 20px;
    }
    .unavailable {
      background: linear-gradient(135deg, #666 0%, #999 100%);
      padding: 30px;
      border-radius: 15px;
      color: white;
      text-align: center;
      margin: 30px 0;
    }
  </style>
  <script>
    // Auto-refresh every 30 seconds
    setTimeout(function() {
      location.reload();
    }, 30000);
  </script>
</head>
<body>
  <div class="container">
    <h1>🌡️ Current Weather Conditions</h1>
    <nav>
      <a href="/">Home</a>
      <a href="/about">Activities & Wildlife</a>
      <a href="/weather">Weather</a>
    </nav>
)rawliteral";

  if (sensorAvailable) {
    float tempF = celsiusToFahrenheit(temperature);
    
    html += R"rawliteral(
    <div class="info-box">
      <strong>📍 Location:</strong> Diversion Dam Area, Roosevelt Lake<br>
      <strong>🕒 Readings:</strong> Live sensor data from BME280
    </div>
    
    <div class="weather-grid">
      <div class="weather-card temp">
        <div class="weather-label">Temperature</div>
        <div class="weather-value">)rawliteral";
    html += String(tempF, 1);
    html += R"rawliteral(<span class="weather-unit">°F</span></div>
        <div style="font-size: 0.9em; opacity: 0.8;">)rawliteral";
    html += String(temperature, 1);
    html += R"rawliteral( °C</div>
      </div>
      
      <div class="weather-card humidity">
        <div class="weather-label">Humidity</div>
        <div class="weather-value">)rawliteral";
    html += String(humidity, 1);
    html += R"rawliteral(<span class="weather-unit">%</span></div>
        <div style="font-size: 0.9em; opacity: 0.8;">Relative Humidity</div>
      </div>
      
      <div class="weather-card pressure">
        <div class="weather-label">Pressure</div>
        <div class="weather-value">)rawliteral";
    html += String(pressure, 1);
    html += R"rawliteral(<span class="weather-unit">hPa</span></div>
        <div style="font-size: 0.9em; opacity: 0.8;">)rawliteral";
    html += String(pressure * 0.02953, 2);
    html += R"rawliteral( inHg</div>
      </div>
    </div>
    
    <div class="info-box">
      <h3 style="margin-top: 0; color: #2C5F2D;">🌤️ What This Means for Your Visit</h3>
      <p><strong>Temperature:</strong> )rawliteral";
    
    if (tempF < 50) {
      html += "Cool conditions - bring warm layers!";
    } else if (tempF < 70) {
      html += "Pleasant weather - perfect for outdoor activities!";
    } else if (tempF < 85) {
      html += "Warm and comfortable - great day to be outside!";
    } else if (tempF < 100) {
      html += "Hot weather - bring plenty of water and sun protection!";
    } else {
      html += "Very hot - limit outdoor activities to early morning or evening!";
    }
    
    html += R"rawliteral(</p>
      <p><strong>Humidity:</strong> )rawliteral";
    
    if (humidity < 30) {
      html += "Dry desert air - stay hydrated and use moisturizer.";
    } else if (humidity < 60) {
      html += "Comfortable humidity levels.";
    } else {
      html += "Higher humidity - may feel warmer than actual temperature.";
    }
    
    html += R"rawliteral(</p>
      <p><strong>Pressure:</strong> )rawliteral";
    
    if (pressure < 1010) {
      html += "Lower pressure - weather may be changing, possible storms.";
    } else if (pressure > 1020) {
      html += "High pressure - stable, clear weather expected.";
    } else {
      html += "Normal pressure - stable conditions.";
    }
    
    html += R"rawliteral(</p>
    </div>
    
    <p class="refresh-note">📊 Page automatically refreshes every 30 seconds with live data</p>
)rawliteral";
  } else {
    html += R"rawliteral(
    <div class="unavailable">
      <h2>⚠️ Weather Sensor Unavailable</h2>
      <p>The BME280 weather sensor is not currently connected or not functioning.</p>
      <p>Check sensor wiring and I2C connection (default pins: SDA=GPIO4/D2, SCL=GPIO5/D1)</p>
    </div>
    
    <div class="info-box">
      <h3 style="margin-top: 0; color: #2C5F2D;">📋 General Roosevelt Lake Weather</h3>
      <p>Roosevelt Lake is located at approximately 2,100 feet elevation in the Sonoran Desert.</p>
      <ul>
        <li><strong>Spring (March-May):</strong> 70-90°F - Perfect weather</li>
        <li><strong>Summer (June-September):</strong> 95-115°F - Very hot, early morning visits recommended</li>
        <li><strong>Fall (October-November):</strong> 70-85°F - Excellent conditions</li>
        <li><strong>Winter (December-February):</strong> 50-65°F - Cool and comfortable</li>
      </ul>
      <p>Always check current weather forecasts before your visit and bring adequate water (1 gallon per person per day in summer).</p>
    </div>
)rawliteral";
  }
  
  html += R"rawliteral(
  </div>
</body>
</html>
)rawliteral";
  
  return html;
}

#endif
