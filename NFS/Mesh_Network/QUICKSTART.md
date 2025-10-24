# Quick Start Guide

Get your Roosevelt Lake mesh network up and running in 30 minutes!

## 📋 Prerequisites

### Hardware Checklist
- [ ] 4x ESP8266 Wemos D1 R2 boards
- [ ] 3x BME280 sensors (or BMP280) - *optional, use what you have*
- [ ] 1x BME680 sensor - *optional*
- [ ] 2x SSD1306 OLED displays (128x64, I2C) - *optional*
- [ ] USB cables for programming (4x micro USB)
- [ ] Optional: Enclosures, power supplies, mounting hardware

**Note:** The issue mentions having 4 ESP8266 boards available. You can configure them as:
- **Option A:** 1 Gateway + 2 BME280 + 1 BME680 (focus on sensors)
- **Option B:** 1 Gateway + 2 BME280 + 1 Display (mix sensors and display)
- **Option C:** 1 Gateway + 1 BME280 + 1 BME680 + 1 Display (one of each)

Choose the combination that best fits your deployment needs!

### Software Setup
- [ ] Arduino IDE installed (v1.8.x or v2.x)
- [ ] ESP8266 board support installed
- [ ] Required libraries installed (see below)

---

## 🚀 Step 1: Install Arduino Libraries (10 minutes)

Open Arduino IDE → Tools → Manage Libraries, search and install:

**For ALL nodes:**
1. `painlessMesh` by Coopdis
2. `ArduinoJson` by Benoit Blanchon
3. `TaskScheduler` by Anatoli Arkhipenko

**For BME280 nodes:**
4. `BME280` by Tyler Glenn

**For BME680 node:**
5. `Adafruit BME680 Library`
6. `Adafruit Unified Sensor`

**For Display nodes:**
7. `Adafruit SSD1306`
8. `Adafruit GFX Library`

✅ All libraries installed? Move to Step 2!

---

## 🔧 Step 2: Wire Your Sensors (5 minutes)

All sensors use the **same I2C wiring**:

```
Sensor/Display Pin → ESP8266 Pin
──────────────────────────────────
VCC or VIN        → 3.3V
GND               → GND
SDA               → GPIO4 (D2)
SCL               → GPIO5 (D1)
```

**Wire:**
- 3x BME280 sensors to their ESP8266 boards
- 1x BME680 sensor to its ESP8266 board
- 2x OLED displays to their ESP8266 boards

🔍 **Double-check:** No loose connections? Correct pins?

---

## 💾 Step 3: Program Your Nodes (10 minutes)

### Node 1: Gateway (No sensor needed)

1. Open `Mesh_Network.ino` in Arduino IDE
2. Select: Tools → Board → "LOLIN(WEMOS) D1 R2 & mini"
3. Select: Tools → Port → (Your COM port)
4. Click: Upload button ➡️
5. Open Serial Monitor (115200 baud)
6. **Note the Node ID** printed in Serial Monitor

✅ Upload successful? You should see "Mesh Gateway Node Initialized"

---

### Nodes 2-4: BME280 Sensors

For **each of the 3 BME280 sensors**:

1. Connect ESP8266 with wired BME280
2. Open `Sensor_Node_BME280.ino`
3. Select correct board and port
4. Upload ➡️
5. Check Serial Monitor: Should see "BME280 sensor found"

✅ All 3 BME280 nodes programmed?

---

### Node 5: BME680 Sensor

1. Connect ESP8266 with wired BME680
2. Open `Sensor_Node_BME680.ino`
3. Select correct board and port
4. Upload ➡️
5. Check Serial Monitor: Should see "BME680 sensor found"

✅ BME680 node ready?

---

### Nodes 6-7: OLED Displays

For **each of the 2 OLED displays**:

1. Connect ESP8266 with wired OLED
2. Open `Display_Node_OLED.ino`
3. Select correct board and port
4. Upload ➡️
5. Display should show "Roosevelt Lake Mesh Network"

✅ Both displays working?

---

## 🌐 Step 4: Form the Mesh (5 minutes)

1. **Power on Gateway first**
   - Serial Monitor shows "Mesh Gateway Node Initialized"
   - Note the Node ID

2. **Power on other nodes one by one**
   - Each should print "Connections changed"
   - Wait 10 seconds between nodes

3. **Check Gateway Serial Monitor**
   - Should show "New Connection: [NodeID]" for each node
   - Should show increasing node count

4. **Check OLED Displays**
   - Should show "CONNECTING TO MESH NETWORK"
   - Then show sensor data when received

✅ All nodes connected? With the basic 4-node setup, you should see the gateway plus sensor nodes connecting!

**Note:** The example shows a 7-node deployment (gateway + 3 BME280 + 1 BME680 + 2 displays). With 4 ESP8266 boards, you might configure:
- 1 Gateway (no sensor)
- 2 BME280 sensor nodes
- 1 OLED display node

Or choose a different combination based on your needs.

---

## 🌍 Step 5: Access the Web Portal

### From Your Phone or Laptop:

1. **Find Gateway WiFi Network**
   - Check Gateway Serial Monitor for network name
   - Default is something like "mesh-xxxxxxxx"

2. **Connect to Gateway WiFi**
   - No password needed (open network)

3. **Open Web Browser**
   - Go to: `http://192.168.4.1`
   - Or check Serial Monitor for exact IP

4. **View Sensor Data!**
   - Should see all sensor nodes
   - Temperature, pressure, humidity readings
   - Auto-refreshes every 30 seconds

✅ Can you see the web portal with sensor data?

---

## ✅ Verification Checklist

Your mesh network is working if:

- [ ] Gateway web portal is accessible
- [ ] Web portal shows data from BME280 nodes
- [ ] Web portal shows data from BME680 node
- [ ] OLED displays show temperature readings
- [ ] Data updates automatically
- [ ] All nodes show "ONLINE" status

---

## 🧪 Testing the Mesh

### Test 1: Range Test
1. Move nodes apart gradually
2. Monitor connection in Serial Monitor
3. Nodes should maintain connection up to 50-100m outdoors

### Test 2: Self-Healing
1. Power off one sensor node
2. Wait 60 seconds
3. Other nodes should still communicate
4. Power node back on - should reconnect automatically

### Test 3: Data Accuracy
1. Hold BME280 sensor - temperature should rise
2. Check web portal - should show increase
3. Check OLED display - should show same temperature

---

## 🐛 Troubleshooting

### Problem: Node not connecting to mesh

**Solutions:**
- Verify mesh credentials match in all firmware files
- Check `MESH_SSID`, `MESH_PASSWORD`, `MESH_PORT`
- Power cycle all nodes
- Move nodes closer together

---

### Problem: Sensor not detected

**Solutions:**
- Check I2C wiring: SDA→D2, SCL→D1
- Verify 3.3V power is connected
- Try I2C scanner sketch to detect address
- Some BME280 use 0x76, others use 0x77

---

### Problem: Display blank or garbled

**Solutions:**
- Check display I2C address (usually 0x3C)
- Verify display power (3.3V)
- Check I2C wiring
- Try adjusting display contrast in code

---

### Problem: Web portal not accessible

**Solutions:**
- Check Gateway Serial Monitor for IP address
- Verify you're connected to Gateway WiFi
- Try `http://192.168.4.1` and IP from Serial Monitor
- Disable cellular data on phone

---

### Problem: "Waiting for Sensor Data" on display

**Solutions:**
- Verify sensor nodes are powered on
- Check sensor nodes Serial Monitor for "Sent: T=..."
- Ensure mesh is formed (check Gateway connections)
- Move nodes closer together

---

## 📊 Monitor Your Network

### Serial Monitor Output

**Gateway should show:**
```
Mesh Gateway Node Initialized
Node ID: 3847562910
HTTP Server Started on Port 80
New Connection: 1234567890
Received from 1234567890: {"type":"BME280",...}
```

**Sensor nodes should show:**
```
BME280 sensor found
BME280 Mesh Sensor Node Initialized
Node ID: 1234567890
Sent: T=25.5°C, P=1013.2hPa
```

**Display nodes should show:**
```
OLED Display initialized
Display Mesh Node Initialized
Node ID: 9876543210
Received from 1234567890: {...}
```

---

## 🎯 Next Steps

Now that your mesh is running:

1. **Test the API:**
   - Visit `http://192.168.4.1/api/sensors`
   - See [API_EXAMPLES.md](API_EXAMPLES.md) for integration ideas

2. **Plan Deployment:**
   - Use [TOPOLOGY.md](TOPOLOGY.md) for layout ideas
   - Consider power options (USB, battery, solar)
   - Choose weatherproof enclosures

3. **Expand the Network:**
   - Add more BME280 sensor nodes
   - Add more OLED displays
   - Increase `MAX_NODES` in gateway firmware

4. **Customize:**
   - Adjust sensor reading intervals
   - Modify web portal design
   - Add new features!

---

## 📚 Additional Resources

- **Full Documentation:** [README.md](README.md)
- **Topology Examples:** [TOPOLOGY.md](TOPOLOGY.md)
- **API Usage:** [API_EXAMPLES.md](API_EXAMPLES.md)
- **Configuration:** [mesh_config.json](mesh_config.json)

---

## 🎉 Success!

Congratulations! You now have a working WiFi mesh network for Roosevelt Lake. Your visitors can access real-time environmental data from multiple locations across the area.

**Need help?** Check the troubleshooting section or review the full documentation.

---

## ⏱️ Time Breakdown

- Library installation: ~10 minutes
- Wiring sensors: ~5 minutes
- Programming nodes: ~10 minutes
- Testing mesh: ~5 minutes
- **Total: ~30 minutes**

(Plus any troubleshooting time)

---

*Happy meshing! 🌐*
