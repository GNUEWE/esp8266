#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Roosevelt Lake Diversion Dam - Self Tour</title>
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
      font-size: 2em;
    }
    h2 {
      color: #2C5F2D;
      margin-top: 25px;
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
    .logo {
      text-align: center;
      margin: 20px 0;
    }
    .welcome {
      font-size: 1.1em;
      line-height: 1.8;
      color: #333;
    }
    .highlight {
      background-color: #E8F5E9;
      padding: 15px;
      border-left: 4px solid #4A7C59;
      margin: 15px 0;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🏞️ Roosevelt Lake & Diversion Dam</h1>
    <nav>
      <a href="/">Home</a>
      <a href="/about">Activities & Wildlife</a>
      <a href="/weather">Weather</a>
    </nav>
    <div class="logo">
      <img src="/logo.svg" alt="Roosevelt Lake" width="220">
    </div>
    <div class="welcome">
      <h2>Welcome to Your Self-Guided Tour</h2>
      <p>Welcome to Roosevelt Lake, Arizona's largest reservoir and a jewel of the Tonto National Forest. You are near the historic Diversion Dam area, a place where desert beauty meets recreational adventure.</p>
      
      <div class="highlight">
        <strong>📍 Location:</strong> Roosevelt Lake, Gila County, Arizona<br>
        <strong>🌡️ Elevation:</strong> 2,100 feet above sea level<br>
        <strong>💧 Lake Size:</strong> 21,493 acres at full capacity
      </div>
      
      <h2>About the Diversion Dam</h2>
      <p>The Roosevelt Diversion Dam, located upstream from the main Roosevelt Dam, plays a crucial role in Arizona's water management system. Built in the early 1900s as part of the Salt River Project, this dam was constructed to assist in the building of the main Roosevelt Dam and represents an important piece of Arizona's agricultural and settlement history.</p>
      
      <p>The dam diverts water from the Salt River into canals that help irrigate the Phoenix valley, making the desert bloom and supporting one of the nation's most productive agricultural regions.</p>
      
      <h2>Natural Beauty</h2>
      <p>The area around Diversion Dam showcases the stunning Sonoran Desert landscape, with iconic saguaro cacti standing sentinel over the turquoise waters. The lake is surrounded by rugged mountains that glow brilliant shades of red and gold at sunset, creating breathtaking photo opportunities.</p>
      
      <p><strong>Explore the Activities & Wildlife page to discover what you can do here!</strong></p>
    </div>
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
  <title>Activities & Wildlife - Roosevelt Lake</title>
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
    h2 {
      color: #2C5F2D;
      margin-top: 25px;
      font-size: 1.5em;
    }
    h3 {
      color: #4A7C59;
      margin-top: 15px;
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
    .info {
      line-height: 1.7;
    }
    .activity-box {
      background-color: #F1F8F4;
      padding: 15px;
      margin: 15px 0;
      border-radius: 8px;
      border-left: 5px solid #4A7C59;
    }
    ul {
      line-height: 1.8;
    }
    .wildlife-section {
      background-color: #FFF9E6;
      padding: 15px;
      border-radius: 8px;
      margin: 15px 0;
      border-left: 5px solid #D4A574;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🎣 Activities & Wildlife</h1>
    <nav>
      <a href="/">Home</a>
      <a href="/about">Activities & Wildlife</a>
      <a href="/weather">Weather</a>
    </nav>
    <div class="info">
      
      <h2>🚣 Recreation Activities</h2>
      
      <div class="activity-box">
        <h3>🎣 Fishing</h3>
        <p>Roosevelt Lake is renowned as one of Arizona's premier bass fishing destinations. The lake holds the state record for largemouth bass and offers year-round fishing opportunities.</p>
        <p><strong>Common catches:</strong></p>
        <ul>
          <li>Largemouth Bass (state record holder!)</li>
          <li>Smallmouth Bass</li>
          <li>Crappie</li>
          <li>Catfish (channel and flathead)</li>
          <li>Carp</li>
        </ul>
        <p><em>Arizona fishing license required. Check current regulations and seasons.</em></p>
      </div>
      
      <div class="activity-box">
        <h3>🥾 Hiking</h3>
        <p>The area around Diversion Dam offers numerous hiking opportunities through pristine Sonoran Desert terrain.</p>
        <p><strong>Popular trails and areas:</strong></p>
        <ul>
          <li>Shoreline trails with lake views</li>
          <li>Desert canyon explorations</li>
          <li>Roosevelt Dam viewing areas</li>
          <li>Historic sites and old mining paths</li>
        </ul>
        <p><em>Remember: Bring plenty of water (1 gallon per person per day), wear sun protection, and tell someone your plans. Summer temperatures can exceed 110°F.</em></p>
      </div>
      
      <div class="activity-box">
        <h3>🛶 Kayaking & Rafting</h3>
        <p>The calm waters near the dam create perfect conditions for paddling sports. Launch your kayak or raft to explore hidden coves and enjoy the stunning canyon scenery.</p>
        <p><strong>What to know:</strong></p>
        <ul>
          <li>Multiple boat ramps available around the lake</li>
          <li>Calm waters ideal for beginners</li>
          <li>Explore numerous coves and inlets</li>
          <li>Watch for changing water levels (dam-controlled)</li>
          <li>Life jackets required by law</li>
        </ul>
      </div>
      
      <div class="activity-box">
        <h3>⛺ Camping</h3>
        <p>Several campgrounds dot the Roosevelt Lake shoreline, offering everything from developed sites to primitive camping.</p>
        <ul>
          <li>Cholla Campground (developed, with amenities)</li>
          <li>Dispersed camping along shoreline</li>
          <li>RV sites available at multiple locations</li>
        </ul>
      </div>
      
      <h2>🦅 Wildlife</h2>
      
      <div class="wildlife-section">
        <h3>Birds</h3>
        <p>Roosevelt Lake is a birdwatcher's paradise with over 200 species recorded:</p>
        <ul>
          <li><strong>Bald Eagles</strong> - Winter residents (November-March)</li>
          <li><strong>Ospreys</strong> - Fishing along the lake</li>
          <li><strong>Great Blue Herons</strong> - Common along shorelines</li>
          <li><strong>Roadrunners</strong> - Desert trails</li>
          <li><strong>Various hawks and owls</strong></li>
        </ul>
      </div>
      
      <div class="wildlife-section">
        <h3>Mammals</h3>
        <ul>
          <li><strong>Desert Bighorn Sheep</strong> - On rocky hillsides (early morning/evening)</li>
          <li><strong>Mule Deer</strong> - Common near water sources</li>
          <li><strong>Javelinas</strong> - Travel in small herds</li>
          <li><strong>Coyotes</strong> - Often heard at dawn and dusk</li>
          <li><strong>Bobcats</strong> - Elusive, occasionally spotted</li>
        </ul>
      </div>
      
      <div class="wildlife-section">
        <h3>Reptiles</h3>
        <p><strong>⚠️ Watch your step!</strong> Several venomous species inhabit the area:</p>
        <ul>
          <li>Western Diamondback Rattlesnakes</li>
          <li>Gila Monsters (venomous lizard)</li>
          <li>Desert Spiny Lizards (harmless)</li>
          <li>Desert Tortoises (protected species)</li>
        </ul>
        <p><em>Never approach or handle wildlife. Give rattlesnakes a wide berth and they will leave you alone.</em></p>
      </div>
      
      <h2>🌵 Desert Plants</h2>
      <p>The Sonoran Desert landscape features iconic vegetation:</p>
      <ul>
        <li><strong>Saguaro Cacti</strong> - The symbol of the Southwest, can live 200+ years</li>
        <li><strong>Palo Verde Trees</strong> - Green bark allows photosynthesis</li>
        <li><strong>Ocotillo</strong> - Dramatic red blooms after rain</li>
        <li><strong>Cholla Cacti</strong> - Beautiful but very sharp spines!</li>
        <li><strong>Mesquite Trees</strong> - Provide shade and food for wildlife</li>
      </ul>
      
      <h2>🌡️ Best Times to Visit</h2>
      <ul>
        <li><strong>Spring (March-May):</strong> Perfect weather, wildflowers bloom</li>
        <li><strong>Fall (October-November):</strong> Comfortable temperatures, great fishing</li>
        <li><strong>Winter (December-February):</strong> Cool weather, excellent for hiking, eagle watching</li>
        <li><strong>Summer (June-September):</strong> Very hot (100°F+), best for water activities, come early morning</li>
      </ul>
      
      <p style="margin-top: 30px; padding: 15px; background-color: #E8F5E9; border-radius: 5px;">
        <strong>🛈 Safety Reminder:</strong> Always bring more water than you think you need, let someone know your plans, check weather conditions, and respect wildlife by observing from a distance.
      </p>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif
