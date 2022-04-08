
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "LocalConfig.h"

unsigned long lastTime = 0;        // Holds value for millis() for timing API calls
unsigned long apiCallDelay = 5000; // Delay between calls to API

void setup() {
  Serial.begin(115200);

  connectToNetwork();
}

void loop() {
  if ((millis() - lastTime) > apiCallDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      getGardenBedConfig();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void connectToNetwork() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void getGardenBedConfig() {
  String currentPath = serverName + "/api/esp32config/gardenBed";

  String payload = httpGETRequest(currentPath);

  StaticJsonDocument<300> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  updateApiCallDelay(doc);

}

String httpGETRequest(String requestPath) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, requestPath.c_str());
  int respCode = http.GET();

  String payload = "{}";

  if (respCode > 0) {
    payload = http.getString();
  }

  http.end();

  logHttpResponse("GET", requestPath, respCode, payload);

  return payload;
}

void logHttpResponse(String requestType, String requestPath, int responseCode, String payload) {
  Serial.print("HTTP Request to   : ");
  Serial.println(requestType + " " + requestPath);

  if (responseCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(responseCode);
    Serial.print("HTTP Response Body: ");
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(responseCode);
  }
}

void updateApiCallDelay(StaticJsonDocument<300> doc) {
  if (!doc["apiCallDelay"]) {
    Serial.println("No apiCallDelay configuration found");
    return;
  }

  if (apiCallDelay != doc["apiCallDelay"]) {
    apiCallDelay = doc["apiCallDelay"];
    Serial.print("apiCallDelay updated to: ");
    Serial.println(apiCallDelay);
  }
}
