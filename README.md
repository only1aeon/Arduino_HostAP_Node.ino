ESP32 Smart Waste Bin – Client Node Firmware


---

🚀 Overview

The Client Node is an ESP32-based sensor module that measures bin fill level using an HC-SR04 ultrasonic sensor and sends data wirelessly to the Host Node. Each bin acts as an intelligent IoT device within a Wireless Sensor Network (WSN).

This node functions as a remote sensor client that publishes real-time bin metrics.


---

📌 Features

📡 Ultrasonic distance measurement

📤 JSON-formatted POST requests to Host Node

🔁 Auto-retry on failed transmissions

🆔 Unique Node ID for multi-bin systems

💡 Status LED feedback

🔋 Low-resource, low-cost design

🔧 Easy scalability (just copy & change node_id)



---

🧰 Hardware Requirements

Component	Description

ESP32 Dev Board	Client microcontroller
HC-SR04	Ultrasonic distance sensor
Jumper wires	For connections
Power supply	USB or battery



---

🔌 Wiring Diagram

HC-SR04 Pin	ESP32 Pin

VCC	5V
GND	GND
TRIG	GPIO 5
ECHO	GPIO 18



---

🛠️ Firmware Configuration

Update these constants before uploading:

#define NODE_ID 1
const char* WIFI_SSID = "SmartBinHost";
const char* WIFI_PASSWORD = "12345678";
const char* HOST_URL = "http://192.168.4.1/update-bin";
const int MAX_BIN_HEIGHT = 40; // cm


---

🧪 How It Works

1. Measures distance using HC-SR04


2. Converts distance → fill level %


3. Packages data as JSON


4. Sends POST request to Host Node


5. Repeats periodically




---

📤 Example JSON Payload

{
  "node_id": 1,
  "distance": 22.3,
  "fill_level": 68
}


---

▶️ Uploading the Firmware

Open Arduino IDE → Select:

Board: ESP32 Dev Module
Port: (Your USB port)
Baud rate: 115200

Click Upload.


---

🧩 Client Node Workflow

[Power On]
      ↓
[Connect to Host AP]
      ↓
[Measure Distance]
      ↓
[Compute Fill Level %]
      ↓
[POST JSON to Host]
      ↓
[Success?] → Yes → Wait & Repeat
       ↓
      No → Retry


---

📌 Troubleshooting

Issue	Cause	Solution

Always 0 cm	Wrong trig/echo pins	Verify wiring
Cannot connect to AP	Wrong credentials	Check SSID & password
Fill level wrong	Wrong bin height	Update MAX_BIN_HEIGHT



---

📄 License

Free for educational and research use.


---

<br><br>

✅ 📄 README.md (HOST NODE)

ESP32 Central Controller – Host Node (AP Mode + Dashboard + Cloud Sync)


---

🚀 Overview

The Host Node is the central controller in the smart waste monitoring system. It:

Creates a Wi-Fi Access Point

Receives bin data from all Client Nodes

Hosts a real-time Local Dashboard

Mirrors readings to:

Blynk

ThingSpeak



It acts as the sink node in the Wireless Sensor Network.


---

📌 Features

🛜 ESP32 Soft-AP mode (independent Wi-Fi network)

🌐 Local dashboard at http://192.168.4.1

📥 REST API for receiving bin data

☁️ Cloud mirroring:

Blynk Virtual Pins

ThingSpeak Channels


✔️ Supports multiple client nodes

📊 Route suggestion logic

🔧 Modular and easily expandable


⚙️ Configuration

Update these in the firmware:

Wi-Fi Access Point

const char* ap_ssid = "SmartBinHost";
const char* ap_password = "12345678";


🧩 API Endpoint

POST → /update-bin

Client sends JSON:

{
  "node_id": 2,
  "distance": 15.7,
  "fill_level": 83
}


🖥️ Local Dashboard

Access via:

👉 http://192.168.4.1

Dashboard displays:

Node ID

Distance (cm)

Fill level (%)

Last update time

Status indicator (Empty → Critical)

Suggested collection priority



---

🧪 Testing the Host

1. Power Host ESP32


2. Connect laptop/phone to:



SSID: SmartBinHost
Password: 12345678

3. Open browser → visit:



http://192.168.4.1

4. Power Client node(s)


5. Watch dashboard update in real time




---

🔁 Host Node Workflow

[Power On]
      ↓
[Start Wi-Fi Access Point]
      ↓
[Start HTTP Server]
      ↓
[Listen for POST /update-bin]
      ↓
[Parse JSON]
      ↓
[Save Bin Data]
      ↓
[Update Local Dashboard]
      ↓
[Push to Blynk + ThingSpeak]
      ↓
[Loop]


---

🛠️ Troubleshooting

Issue	Cause	Solution

Dashboard empty	No clients	Ensure client nodes are powered
Cloud not updating	No internet	Connect Host to router
Client cannot join AP	AP limit exceeded	Increase max STA connections



---

📄 License

Free for academic and research purposes.
