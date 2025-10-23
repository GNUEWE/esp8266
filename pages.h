#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP8266 Home</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 800px;
      margin: 0 auto;
      padding: 20px;
      background-color: #f0f0f0;
    }
    .container {
      background-color: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
      border-bottom: 2px solid #0066cc;
      padding-bottom: 10px;
    }
    nav {
      margin: 20px 0;
    }
    nav a {
      color: #0066cc;
      text-decoration: none;
      margin-right: 15px;
      padding: 5px 10px;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    nav a:hover {
      background-color: #e6f2ff;
    }
    .logo {
      text-align: center;
      margin: 20px 0;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Welcome to ESP8266 Web Server</h1>
    <nav>
      <a href="/">Home</a>
      <a href="/about">About</a>
    </nav>
    <div class="logo">
      <img src="/logo.svg" alt="Logo" width="200">
    </div>
    <p>This is the home page of your ESP8266 web server.</p>
    <p>The server is running and ready to handle requests!</p>
  </div>
</body>
</html>
)rawliteral";

const char about_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>About - ESP8266</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 800px;
      margin: 0 auto;
      padding: 20px;
      background-color: #f0f0f0;
    }
    .container {
      background-color: white;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
      border-bottom: 2px solid #0066cc;
      padding-bottom: 10px;
    }
    nav {
      margin: 20px 0;
    }
    nav a {
      color: #0066cc;
      text-decoration: none;
      margin-right: 15px;
      padding: 5px 10px;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    nav a:hover {
      background-color: #e6f2ff;
    }
    .info {
      line-height: 1.6;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>About This Project</h1>
    <nav>
      <a href="/">Home</a>
      <a href="/about">About</a>
    </nav>
    <div class="info">
      <h2>ESP8266 Web Server</h2>
      <p>This is a web server running on an ESP8266 microcontroller.</p>
      <p><strong>Features:</strong></p>
      <ul>
        <li>WiFi station mode with automatic AP fallback</li>
        <li>HTTP web server on port 80</li>
        <li>Multiple pages (Home, About)</li>
        <li>SVG logo support</li>
        <li>Responsive design</li>
      </ul>
      <p><strong>Technical Details:</strong></p>
      <ul>
        <li>Microcontroller: ESP8266</li>
        <li>WiFi Library: ESP8266WiFi</li>
        <li>Web Server: ESP8266WebServer</li>
        <li>Connection Timeout: 8 seconds</li>
      </ul>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif
