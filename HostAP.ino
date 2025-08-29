// Host Node Arduino Code with Routing
#include <WiFi.h>

const char* AP_SSID = "WASTE_AP";
const char* AP_PASS = "wastepass";
WiFiServer server(80);

struct NodeState {
  float dist;        // last measured distance (cm), -1 = no reading
  float height;      // bin height (cm)
  unsigned long lastSeen; // millis()
};

NodeState nodes[4]; // index 1..3 used

// Distances between bins in meters
float dist12 = 100.0;
float dist23 = 80.0;
float dist31 = 120.0;

String getParam(String params, String name) {
  int idx = params.indexOf(name + "=");
  if (idx == -1) return "";
  int start = idx + name.length() + 1;
  int amp = params.indexOf('&', start);
  if (amp == -1) amp = params.length();
  return params.substring(start, amp);
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP started. IP: ");
  Serial.println(IP);
  server.begin();

  for (int i = 0; i < 4; ++i) {
    nodes[i].dist = -1.0;
    nodes[i].height = 0.0;
    nodes[i].lastSeen = 0;
  }
}

float computeFillPercent(int idx) {
  if (idx < 1 || idx > 3) return 0.0;
  if (nodes[idx].dist <= 0 || nodes[idx].height <= 0) return -1.0;
  float p = (1.0f - nodes[idx].dist / nodes[idx].height) * 100.0f;
  if (p < 0.0) p = 0.0;
  if (p > 100.0) p = 100.0;
  return p;
}

String calculateRoute() {
  int binsToCollect[3];
  int count = 0;
  for (int i = 1; i <= 3; i++) {
    float pct = computeFillPercent(i);
    if (pct >= 50) { // Only consider >= 50% full
      binsToCollect[count++] = i;
    }
  }
  if (count == 0) return "No bins need collection.";
  String route = "Route: ";
  for (int i = 0; i < count; i++) {
    route += "Bin " + String(binsToCollect[i]);
    if (i < count - 1) route += " -> ";
  }
  return route;
}

void serveStatusPage(WiFiClient &client) {
  String html = "<!doctype html><html><head><meta charset='utf-8'><title>Waste Bins</title></head><body>";
  html += "<h2>Waste bin status</h2>";
  html += "<table border='1' cellpadding='6'><tr><th>Node</th><th>Distance (cm)</th><th>Bin Height (cm)</th><th>Fill %</th><th>Last Seen (s)</th></tr>";
  unsigned long now = millis();
  for (int i = 1; i <= 3; ++i) {
    html += "<tr><td>" + String(i) + "</td>";
    if (nodes[i].dist < 0) html += "<td>--</td>";
    else html += "<td>" + String(nodes[i].dist,1) + "</td>";
    if (nodes[i].height <= 0) html += "<td>--</td>";
    else html += "<td>" + String(nodes[i].height,1) + "</td>";
    float pct = computeFillPercent(i);
    if (pct < 0) html += "<td>--</td>";
    else html += "<td>" + String(pct,1) + "%</td>";
    if (nodes[i].lastSeen == 0) html += "<td>--</td>";
    else html += "<td>" + String((now - nodes[i].lastSeen) / 1000) + "</td>";
    html += "</tr>";
  }
  html += "</table>";

  html += "<h3>Current Route</h3><p>" + calculateRoute() + "</p>";

  // Distance editing form
  html += "<h3>Set Bin Distances (m)</h3>";
  html += "<form action='/setdist' method='get'>";
  html += "Bin1→Bin2: <input type='number' step='0.1' name='d12' value='" + String(dist12) + "'><br>";
  html += "Bin2→Bin3: <input type='number' step='0.1' name='d23' value='" + String(dist23) + "'><br>";
  html += "Bin3→Bin1: <input type='number' step='0.1' name='d31' value='" + String(dist31) + "'><br>";
  html += "<input type='submit' value='Update'>";
  html += "</form>";

  html += "<p>Connect to SSID: " + String(AP_SSID) + "</p></body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String requestLine = client.readStringUntil('\r');
    client.readStringUntil('\n');

    while (client.connected()) {
      String h = client.readStringUntil('\r');
      client.readStringUntil('\n');
      if (h.length() == 0) break;
    }

    if (requestLine.startsWith("GET /update?")) {
      int start = requestLine.indexOf("/update?") + 8;
      int end = requestLine.indexOf(' ', start);
      String q = requestLine.substring(start, end);

      String snode = getParam(q, "node");
      String sdist  = getParam(q, "dist");
      String sheight = getParam(q, "height");

      int node = snode.length() ? snode.toInt() : -1;
      float dist = sdist.length() ? sdist.toFloat() : -1.0;
      float height = sheight.length() ? sheight.toFloat() : 0.0;

      if (node >= 1 && node <= 3) {
        nodes[node].dist = dist;
        if (height > 0) nodes[node].height = height;
        nodes[node].lastSeen = millis();
      }

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println("OK");
    }
    else if (requestLine.startsWith("GET /setdist?")) {
      int start = requestLine.indexOf("/setdist?") + 9;
      int end = requestLine.indexOf(' ', start);
      String q = requestLine.substring(start, end);

      String sd12 = getParam(q, "d12");
      String sd23 = getParam(q, "d23");
      String sd31 = getParam(q, "d31");

      if (sd12.length()) dist12 = sd12.toFloat();
      if (sd23.length()) dist23 = sd23.toFloat();
      if (sd31.length()) dist31 = sd31.toFloat();

      client.println("HTTP/1.1 303 See Other");
      client.println("Location: /");
      client.println();
    }
    else {
      serveStatusPage(client);
    }

    delay(1);
    client.stop();
  }
}