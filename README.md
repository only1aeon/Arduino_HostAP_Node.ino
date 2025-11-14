## ESP32 Central Controller – Host Node (AP Mode + Dashboard + Cloud Sync)

# 🚀 Overview

The `Host Node` is the central controller in the smart waste monitoring system. It:

- Creates a Wi-Fi Access Point
- Receives bin data from all Client Nodes
- Hosts a real-time Local Dashboard

## 📌 Features
- 🛜 ESP32 Soft-AP mode (independent Wi-Fi network)
- 🌐 Local dashboard at http://192.168.4.1
- 📥 REST API for receiving bin data
- ✔️ Supports multiple client nodes
- 📊 Route suggestion logic
- 🔧 Modular and easily expandable

## ⚙️ Configuration
Update these in the firmware:

Wi-Fi Access Point

`const char* ap_ssid = "SmartBinHost";`

`const char* ap_password = "12345678";`

## 🧩 API Endpoint

POST → /update-bin

Client sends JSON:

`{`

  `"node_id": 2,`

  `"distance": 15.7,`

  `"fill_level": 83`

`}`


---

## 🖥️ Local Dashboard

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