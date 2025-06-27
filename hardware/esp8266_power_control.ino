#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "jesus";
const char* password = "jesus1234";

// API Endpoint
const char* apiUrl = "http://192.168.1.104/tenant_power_control_system/backend/hardware_api.php";

// Relay Pins for each tenant
const int relayPins[3] = {5, 18, 19};  // GPIO5, GPIO18, GPIO19

// Sensor Pins (ADC)
const int sensorPins[3] = {34, 35, 32}; // GPIO34, GPIO35, GPIO32

// Tenant IDs
const int tenantIds[3] = {1, 2, 3};

void setup() {
  Serial.begin(115200);

  // Setup relays
  for (int i = 0; i < 3; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);
  }

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  for (int i = 0; i < 3; i++) {
    int sensorValue = analogRead(sensorPins[i]);  // 0–4095 on ESP32
    float voltage = sensorValue * (3.3 / 4095.0);
    float current = voltage; // Placeholder — use real formula if needed

    String status;
    if (current < 0.1) {
      digitalWrite(relayPins[i], LOW);
      status = "disconnected";
    } else {
      digitalWrite(relayPins[i], HIGH);
      status = "connected";
    }

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(apiUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String postData = "tenant_id=" + String(tenantIds[i]) +
                        "&status=" + status +
                        "&current_kw=" + String(current, 3);

      Serial.print("Tenant ");
      Serial.print(tenantIds[i]);
      Serial.print(" → Current: ");
      Serial.print(current);
      Serial.print(" kW → Status: ");
      Serial.println(status);

      int httpCode = http.POST(postData);
      if (httpCode > 0) {
        String response = http.getString();
        Serial.print("Response: ");
        Serial.println(response);
      } else {
        Serial.println("Failed to POST");
      }
      http.end();
    }
    delay(1000); // Short delay between tenant updates
  }

  delay(5000); // Wait before the next cycle
}
