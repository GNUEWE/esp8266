#ifndef SVG_H
#define SVG_H

#include <Arduino.h>

const char logo_svg[] PROGMEM = R"rawliteral(
<svg width="200" height="200" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="grad1" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" style="stop-color:#0066cc;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#00ccff;stop-opacity:1" />
    </linearGradient>
  </defs>
  
  <!-- Circuit board background -->
  <rect width="200" height="200" fill="#1a1a1a" rx="10"/>
  
  <!-- WiFi symbol -->
  <g transform="translate(100, 100)">
    <!-- Outer arc -->
    <path d="M -50,-20 A 70 70 0 0 1 50,-20" 
          fill="none" stroke="url(#grad1)" stroke-width="6" stroke-linecap="round"/>
    
    <!-- Middle arc -->
    <path d="M -35,-15 A 50 50 0 0 1 35,-15" 
          fill="none" stroke="url(#grad1)" stroke-width="6" stroke-linecap="round"/>
    
    <!-- Inner arc -->
    <path d="M -20,-10 A 30 30 0 0 1 20,-10" 
          fill="none" stroke="url(#grad1)" stroke-width="6" stroke-linecap="round"/>
    
    <!-- Center dot -->
    <circle cx="0" cy="0" r="8" fill="url(#grad1)"/>
  </g>
  
  <!-- ESP8266 text -->
  <text x="100" y="170" font-family="Arial, sans-serif" font-size="20" 
        font-weight="bold" fill="#00ccff" text-anchor="middle">ESP8266</text>
</svg>
)rawliteral";

#endif
