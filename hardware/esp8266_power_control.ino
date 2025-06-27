#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "jesus";          // Your WiFi SSID
const char* password = "jesus1234";  // Your WiFi Password

const char* apiUrl = "http://192.168.1.104/tenant_power_control_system/backend/hardware_api.php";

#define RELAY_PIN 5           // GPIO5
#define CURRENT_SENSOR 34     // Analog pin (ADC1 channel)

int tenant_id = 1;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  int sensorValue = analogRead(CURRENT_SENSOR); // 0–4095 on ESP32
  float voltage = sensorValue * (3.3 / 4095.0);  // Convert to voltage
  float current = voltage;  // Placeholder (custom calibration formula here)

  String status;
  if (current < 0.1) {
    digitalWrite(RELAY_PIN, LOW);
    status = "disconnected";
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    status = "connected";
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "tenant_id=" + String(tenant_id) +
                      "&status=" + status +
                      "&current_kw=" + String(current, 4);

    Serial.print("Sending → ");
    Serial.println(postData);

    int httpResponseCode = http.POST(postData);
    Serial.print("Tenant ");
    Serial.print(tenant_id);
    Serial.print(" → Status: ");
    Serial.print(httpResponseCode);
    Serial.print(" | Response: ");

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.println("Failed to connect.");
    }

    http.end();
  }

  delay(5000); // Send every 5 sec
}
