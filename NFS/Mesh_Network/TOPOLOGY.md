# Mesh Network Topology Diagrams

## Example 1: Linear Chain Topology

This topology is ideal for extending coverage along a trail or road:

```
[Display Node] ←→ [BME280 Node] ←→ [Gateway/Web] ←→ [BME680 Node] ←→ [Display Node]
    OLED #1         Sensor #1          Portal          Sensor #2          OLED #2
   Trailhead      Mile Marker 1     Visitor Center    Dam Overlook       Campground
```

**Coverage**: ~200-400m linear distance  
**Advantage**: Simple to deploy along existing infrastructure  
**Disadvantage**: If middle node fails, ends become isolated

---

## Example 2: Star Topology

This topology provides redundancy with all nodes connecting to central gateway:

```
                    [BME280 #1]
                         ↑
                         |
    [Display #1] ←→ [Gateway/Web] ←→ [BME680]
                         |
                         ↓
                    [BME280 #2]
                         |
                    [Display #2]
```

**Coverage**: ~100-200m radius from gateway  
**Advantage**: Simple routing, direct communication  
**Disadvantage**: Limited range extension, gateway is single point of failure

---

## Example 3: Mesh Topology (Recommended)

This provides maximum reliability with multiple paths:

```
        [Gateway/Web] ←→ [BME280 #1]
             ↑ ↓              ↓
             ↓ ↑              ↓
        [Display #1] ←→ [BME680] ←→ [BME280 #2]
                              ↓          ↓
                              ↓          ↓
                         [Display #2] ←→ [BME280 #3]
```

**Coverage**: Maximum area coverage with redundant paths  
**Advantage**: Self-healing, multiple routes, best reliability  
**Disadvantage**: More complex to visualize

---

## Example 4: Roosevelt Lake Deployment

Actual deployment around Roosevelt Lake Diversion Dam area:

```
                    [Parking Lot]
                    Display OLED
                         |
                         |
                   [Visitor Center]
                    Gateway + Web
                      /    |    \
                     /     |     \
                    /      |      \
        [Dam Overlook]  [Trail]  [Campground]
         BME680 Sensor  BME280    Display OLED
              |            |           |
              |            |           |
         [Picnic Area] [Restroom]  [Boat Launch]
          BME280       BME280      (future node)
```

---

## Network Discovery Process

When nodes power on:

1. **Initial Scan**: Each node scans for other mesh nodes
   ```
   Node: "Is anyone there?"
   ```

2. **Neighbor Discovery**: Nodes respond and establish connections
   ```
   Gateway: "I'm here! (ID: 3847562910)"
   BME280:  "I'm here! (ID: 1234567890)"
   ```

3. **Routing Table**: Each node builds a routing table
   ```
   Gateway knows: BME280, BME680, Display
   BME280 knows:  Gateway, BME680
   Display knows: Gateway, BME280
   ```

4. **Message Routing**: Messages hop through intermediate nodes
   ```
   [BME280] → [Gateway] → [Display]
   "Temperature: 25.5°C"
   ```

---

## Signal Strength Considerations

### Good Placement (Strong Signals)
```
[Node A] ←-50m-→ [Node B] ←-50m-→ [Node C]
  ████              ████              ████
 Strong           Strong            Strong
```

### Poor Placement (Weak Signals)
```
[Node A] ←----150m----→ [Node B]
  ████                    ▓▓▓▓
 Strong                  Weak
```

**Recommendations:**
- Keep nodes within 50-100m of each other outdoors
- Consider obstacles: buildings, hills, dense vegetation
- Metal structures significantly reduce range
- Higher placement improves range

---

## Power Distribution

### Solar Powered Mesh (Recommended for Remote Areas)
```
    ☀️ [Solar Panel]
        ↓ (charge)
    🔋 [Battery]
        ↓ (5V)
    📡 [ESP8266 + Sensor]
```

Each remote node can be solar-powered:
- 5W solar panel
- 2000mAh+ LiPo battery
- Charge controller
- Waterproof enclosure

### Wired Power Mesh (Visitor Center Area)
```
    🔌 [USB Power Adapter]
        ↓ (5V)
    📡 [ESP8266 + Display]
```

Nodes near buildings use wall power or USB.

---

## Data Flow Example

### Sensor Reading → Gateway → Web Portal

1. **BME280 Sensor Node** reads sensor every 30 seconds:
   ```json
   {
     "type": "BME280",
     "temperature": 25.5,
     "pressure": 1013.2,
     "humidity": 0.0,
     "gas": 0.0
   }
   ```

2. **Broadcasts JSON** to mesh network

3. **Gateway Node** receives message:
   - Parses JSON
   - Stores in local cache
   - Updates web page data

4. **Web Portal** displays on visitor's device:
   ```
   🌡️ Temperature: 25.5°C (77.9°F)
   🔽 Pressure: 1013.2 hPa (29.92 inHg)
   📡 Node: 1234567890
   ⏱️ Updated: 5s ago
   ```

5. **Display Node** also receives broadcast:
   - Updates OLED screen
   - Shows temperature prominently
   - Refreshes every 5 seconds

---

## Failure Scenarios and Recovery

### Scenario 1: Sensor Node Offline
```
Before: [Gateway] ←→ [BME280] ←→ [Display]
After:  [Gateway] ←---X--→ [Display]

Gateway shows: "Node offline (60s+)"
Display shows: "Waiting for sensor data"
```

**Recovery**: When node comes back online, immediately rejoins mesh

---

### Scenario 2: Gateway Offline
```
Before: [BME280] ←→ [Gateway] ←→ [Display]
After:  [BME280] ←---X--→ [Display]
                     ↓
                [BME280] ←→ [Display]

Mesh continues: Sensor and display still communicate
Web portal unavailable until gateway returns
```

**Recovery**: Gateway automatically becomes root when back online

---

### Scenario 3: Middle Node Offline (Chain)
```
Before: [Node A] ←→ [Node B] ←→ [Node C]
After:  [Node A] ←---X--→ [Node C]

If A and C out of range: Network splits into two segments
If A and C in range: They connect directly, bypassing B
```

**Recovery**: Mesh automatically re-routes around failed node

---

## Scaling Beyond 4 Nodes

The mesh network can support many more nodes:

```
Current: 4 nodes (3 BME280 + 1 BME680)
Expanded: 10+ nodes

Example 10-node deployment:
- 1 Gateway (web portal)
- 5 BME280 sensors (different areas)
- 1 BME680 sensor (main station)
- 3 OLED displays (high-traffic areas)
```

**Gateway Firmware Adjustment:**
```cpp
// Change in Mesh_Network.ino
#define MAX_NODES 10  // Increase from 4 to 10
```

This allows tracking up to 10 sensor nodes simultaneously.

---

## Mesh Network Advantages

✅ **Extended Range**: Each node extends coverage  
✅ **Self-Healing**: Automatically routes around failures  
✅ **Scalable**: Easy to add more nodes  
✅ **Redundant**: Multiple paths for reliability  
✅ **Low Cost**: Inexpensive ESP8266 hardware  
✅ **Open Source**: Fully customizable  

## Traditional WiFi Limitations

❌ **Single Access Point**: Limited to ~100m range  
❌ **No Redundancy**: AP failure means total outage  
❌ **Fixed Coverage**: Can't extend without additional APs  
❌ **Complex Setup**: Multiple APs need coordination  

---

## Next Steps

1. **Build and test** one sensor node + gateway
2. **Verify range** between nodes at deployment site
3. **Add nodes incrementally** to extend coverage
4. **Monitor performance** via serial output and web portal
5. **Document actual node locations** for maintenance

For full setup instructions, see [README.md](README.md)
