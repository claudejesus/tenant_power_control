#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "jesus";
const char* password = "jesus1234";

// API endpoints
const char* apiURL_GET = "http://192.168.1.102/tenant_power_control_system/backend/api_get_power.php";
const char* apiURL_POST = "http://192.168.1.102/tenant_power_control_system/backend/hardware_api.php";

// Tenant settings
struct Tenant {
  int id;
  int relayPin;
  int ledPin;
  int currentPin;
  int voltagePin;
  float current_kw;
};

Tenant tenants[] = {
  {1, 18, 15, 13, 34, 0.0},  // Tenant 1
  {2, 19, 2, 35, 5, 0.0}     // Tenant 2
};

const int tenantCount = sizeof(tenants) / sizeof(tenants[0]);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  for (int i = 0; i < tenantCount; i++) {
    pinMode(tenants[i].relayPin, OUTPUT);
    pinMode(tenants[i].ledPin, OUTPUT);
    digitalWrite(tenants[i].relayPin, HIGH); // Relay ON
    digitalWrite(tenants[i].ledPin, LOW);    // LED OFF
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String(apiURL_GET));
    int code = http.GET();

    if (code == 200) {
      String payload = http.getString();
      Serial.println("GET Payload: " + payload);

      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        JsonArray arr = doc.as<JsonArray>();

        for (int i = 0; i < tenantCount; i++) {
          for (JsonObject obj : arr) {
            if (obj["tenant_id"] == tenants[i].id) {
              tenants[i].current_kw = obj["current_kw"];
              break;
            }
          }

          // Read sensors
          float rawCurrent = analogRead(tenants[i].currentPin);
          float rawVoltage = analogRead(tenants[i].voltagePin);
          float current = rawCurrent * (3.3 / 4095.0); // placeholder
          float voltage = rawVoltage * (3.3 / 4095.0); // placeholder
          float powerUsed = voltage * current;

          Serial.printf("Tenant %d → V: %.2f V, I: %.2f A, P: %.2f kW\n", tenants[i].id, voltage, current, powerUsed);

          // Update usage
          if (tenants[i].current_kw > 0) {
            tenants[i].current_kw -= powerUsed;
            if (tenants[i].current_kw < 0) tenants[i].current_kw = 0;
            sendUsageUpdate(tenants[i].id, powerUsed);
          }

          // Relay and LED control
          if (tenants[i].current_kw <= 0) {
            digitalWrite(tenants[i].relayPin, LOW); // Cut power
            digitalWrite(tenants[i].ledPin, HIGH);  // LED ON (disconnected)
            Serial.printf("Relay OFF for Tenant %d\n", tenants[i].id);
          } else {
            digitalWrite(tenants[i].relayPin, HIGH); // Power ON
            digitalWrite(tenants[i].ledPin, LOW);    // LED OFF
            Serial.printf("Relay ON for Tenant %d\n", tenants[i].id);
          }

          delay(1000); // short delay between tenants
        }

      } else {
        Serial.println("JSON Parse Error!");
      }
    } else {
      Serial.printf("GET error: %d\n", code);
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected.");
  }

  delay(5000);
}

void sendUsageUpdate(int tenantId, float usedKw) {
  HTTPClient http;
  http.begin(apiURL_POST);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "tenant_id=" + String(tenantId) + "&used_kw=" + String(usedKw, 3);
  int httpCode = http.POST(postData);
  String response = http.getString();

  Serial.printf("POST tenant %d (%.2f kW) → Code: %d, Response: %s\n", tenantId, usedKw, httpCode, response.c_str());

  http.end();
}
