#ifndef SVG_H
#define SVG_H

#include <Arduino.h>

const char logo_svg[] PROGMEM = R"rawliteral(
<svg width="220" height="220" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="skyGrad" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#87CEEB;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#E0F6FF;stop-opacity:1" />
    </linearGradient>
    <linearGradient id="waterGrad" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#4A90E2;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#2E5C8A;stop-opacity:1" />
    </linearGradient>
    <linearGradient id="mountainGrad" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#8B6F47;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#654321;stop-opacity:1" />
    </linearGradient>
  </defs>
  
  <!-- Sky background -->
  <rect width="220" height="220" fill="url(#skyGrad)" rx="10"/>
  
  <!-- Sun -->
  <circle cx="180" cy="40" r="25" fill="#FFD700" opacity="0.9"/>
  
  <!-- Mountains in background -->
  <path d="M 0,120 L 50,70 L 100,100 L 150,60 L 220,110 L 220,220 L 0,220 Z" 
        fill="url(#mountainGrad)" opacity="0.7"/>
  
  <!-- Closer mountains -->
  <path d="M 0,140 L 40,100 L 80,120 L 120,90 L 160,115 L 220,130 L 220,220 L 0,220 Z" 
        fill="url(#mountainGrad)"/>
  
  <!-- Lake water -->
  <ellipse cx="110" cy="165" rx="100" ry="25" fill="url(#waterGrad)" opacity="0.8"/>
  
  <!-- Water reflection shimmer -->
  <ellipse cx="110" cy="165" rx="95" ry="20" fill="#6BB6FF" opacity="0.3"/>
  
  <!-- Saguaro cactus left -->
  <g transform="translate(30, 140)">
    <rect x="-3" y="0" width="6" height="35" fill="#4A7C59" rx="3"/>
    <rect x="-3" y="10" width="12" height="5" fill="#4A7C59" rx="2"/>
    <rect x="9" y="10" width="5" height="15" fill="#4A7C59" rx="2"/>
  </g>
  
  <!-- Saguaro cactus right -->
  <g transform="translate(185, 135)">
    <rect x="-3" y="0" width="6" height="40" fill="#4A7C59" rx="3"/>
    <rect x="-15" y="12" width="12" height="5" fill="#4A7C59" rx="2"/>
    <rect x="-15" y="12" width="5" height="18" fill="#4A7C59" rx="2"/>
  </g>
  
  <!-- Dam structure -->
  <rect x="95" y="155" width="30" height="8" fill="#8B8680" opacity="0.8"/>
  <rect x="98" y="163" width="24" height="3" fill="#696969" opacity="0.6"/>
  
  <!-- Title -->
  <text x="110" y="205" font-family="Georgia, serif" font-size="18" 
        font-weight="bold" fill="#2C5F2D" text-anchor="middle">Roosevelt Lake</text>
  <text x="110" y="218" font-family="Georgia, serif" font-size="11" 
        fill="#4A7C59" text-anchor="middle">Diversion Dam Area</text>
</svg>
)rawliteral";

#endif
