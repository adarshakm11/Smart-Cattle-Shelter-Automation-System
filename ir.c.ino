#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

// ===== WiFi Credentials =====
const char* ssid = "OPPO K12x 5G";
const char* password = "kicchaforever11";

// ===== Pin Definitions =====
#define LDR_PIN A1
#define LIGHT_LED 8
#define RAIN_SENSOR 14
#define ROOF_SERVO 11
#define FAN_LED 16
#define FAN_RELAY 5
#define DHTPIN 7

// ===== Objects =====
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
Servo roofServo;
WebServer server(80);

// ===== Thresholds =====
int lightThreshold = 2000;
int roofThreshold  = 5000;
float thresholdTemp = 30.0;

// ===== States =====
bool roofIsClosed = false;
bool fanState = false;
bool lightState = false;
float temperature = 0;
int ldrValue = 0;
bool rainDetected = false;

// ===== HTML Page (Dynamic Table) =====
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Smart Shelter Dashboard</title>
  <meta charset="UTF-8">
  <style>
    body { font-family: Arial; background: #f4f4f9; text-align: center; margin: 0; padding: 0; }
    h1 { color: #333; padding: 10px; }
    table {
      margin: 10px auto;
      border-collapse: collapse;
      width: 95%;
      background: white;
      box-shadow: 0px 4px 8px rgba(0,0,0,0.2);
      border-radius: 12px;
      overflow: hidden;
      font-size: 14px;
    }
    th, td { padding: 10px; border: 1px solid #ddd; text-align: center; }
    th { background: #4CAF50; color: white; }
    tr:nth-child(even) { background: #f2f2f2; }
  </style>
</head>
<body>
  <h1>Smart Shelter Dashboard</h1>
  <table id="dataTable">
    <tr>
      <th>Timestamp</th>
      <th>Temperature (°C)</th>
      <th>LDR Value</th>
      <th>Atmosphere Light</th>
      <th>Rain</th>
      <th>Roof</th>
      <th>Fan</th>
      <th>Bulb/LED</th>
    </tr>
  </table>
  <script>
    function updateData() {
      fetch("/data")
        .then(response => response.json())
        .then(data => {
          let table = document.getElementById("dataTable");
          let row = table.insertRow(-1);
          let now = new Date().toLocaleTimeString();
          row.insertCell(0).innerText = now;
          row.insertCell(1).innerText = data.temperature + " °C";
          row.insertCell(2).innerText = data.ldrValue;
          row.insertCell(3).innerText = data.lightState ? "Dark" : "Bright";
          row.insertCell(4).innerText = data.rainDetected ? "YES" : "NO";
          row.insertCell(5).innerText = data.roofIsClosed ? "Closed" : "Open";
          row.insertCell(6).innerText = data.fanState ? "ON" : "OFF";
          row.insertCell(7).innerText = data.lightState ? "ON" : "OFF";

          // Keep last 20 rows to prevent table overflow
          while (table.rows.length > 21) {
            table.deleteRow(1);
          }
        })
        .catch(err => console.error("Fetch error:", err));
    }
    setInterval(updateData, 7000); // Refresh every 7 seconds
  </script>
</body>
</html>
)rawliteral";

// ===== JSON Data Endpoint =====
void handleData() {
  StaticJsonDocument<200> doc;
  doc["temperature"] = isnan(temperature) ? 0.0 : temperature;
  doc["ldrValue"] = ldrValue;
  doc["lightState"] = lightState;
  doc["rainDetected"] = rainDetected;
  doc["roofIsClosed"] = roofIsClosed;
  doc["fanState"] = fanState;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);

  pinMode(LDR_PIN, INPUT);
  pinMode(LIGHT_LED, OUTPUT);
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(FAN_LED, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  digitalWrite(LIGHT_LED, LOW);
  digitalWrite(FAN_LED, LOW);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", []() { server.send_P(200, "text/html", htmlPage); });
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();

  // ==== SENSOR READINGS ====
  ldrValue = analogRead(LDR_PIN);
  rainDetected = (digitalRead(RAIN_SENSOR) == LOW);
  temperature = dht.readTemperature();

  // ==== LIGHT CONTROL ====
  if (ldrValue > lightThreshold) {
    digitalWrite(LIGHT_LED, HIGH);
    lightState = true;
  } else {
    digitalWrite(LIGHT_LED, LOW);
    lightState = false;
  }

  // ==== ROOF CONTROL ====
  if ((rainDetected || ldrValue <= roofThreshold) && !roofIsClosed) {
    roofServo.attach(ROOF_SERVO);
    roofServo.write(90); delay(1000);
    roofServo.detach();
    roofIsClosed = true;
  } else if ((!rainDetected && ldrValue > roofThreshold) && roofIsClosed) {
    roofServo.attach(ROOF_SERVO);
    roofServo.write(0); delay(1000);
    roofServo.detach();
    roofIsClosed = false;
  }

  // ==== FAN CONTROL ====
  if (!isnan(temperature)) {
    if (temperature > thresholdTemp) {
      digitalWrite(FAN_LED, HIGH);
      digitalWrite(FAN_RELAY, HIGH);
      fanState = true;
    } else {
      digitalWrite(FAN_LED, LOW);
      digitalWrite(FAN_RELAY, LOW);
      fanState = false;
    }
  }
}
