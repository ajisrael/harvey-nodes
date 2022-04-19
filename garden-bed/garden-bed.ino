
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "LocalConfig.h"
#include "HttpHelper.h"

const int jsonDocSize = 300;       // size of static JSON document

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
      sendGardenBedData();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void connectToNetwork() {
  const int connectionDelay = 500;   // Delay for output when connecting to wifi

  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(connectionDelay);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void getGardenBedConfig() {
  String currentPath = serverName + "/api/esp32config/gardenBed";

  String payload = httpGETRequest(currentPath);

  StaticJsonDocument<jsonDocSize> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  updateApiCallDelay(doc);
}


void updateApiCallDelay(StaticJsonDocument<jsonDocSize> doc) {
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

void sendGardenBedData() {
  String currentPath = serverName + "/api/data/gardenBed";

  String gardenData = "{\"testKey\":\"testValue\"}";

  StaticJsonDocument<jsonDocSize> doc;
  // Add values in the document
  //
  doc["airTemp"] = 34;
  doc["soilTemp"] = 38;
  doc["light"] = 0.50;
  doc["moisture"] = 0.45;
  doc["humidity"] = 0.60;

  JsonArray data = doc.createNestedArray("testArray");
  data.add(48.756080);
  data.add(2.302038);

  String requestBody;
  serializeJson(doc, requestBody);

  int respCode = httpPOSTRequest(currentPath, requestBody);
}
