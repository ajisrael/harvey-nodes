
#include <WiFi.h>
#include <HTTPClient.h>
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
  HTTPClient http;
  
  String currentPath = serverName + "/api/esp32config/gardenBed";

  http.begin(currentPath.c_str());
  int respCode = http.GET();

  logHttpResponse("GET", currentPath, respCode, http.getString());

  http.end();
}

void logHttpResponse(String requestType, String requestPath, int responseCode, String payload) {
  Serial.print("Request made to: ");
  Serial.println(requestType + " " + requestPath);

  if (responseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(responseCode);
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(responseCode);
  }
}
