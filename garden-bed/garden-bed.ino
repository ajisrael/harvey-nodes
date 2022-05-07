
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "LocalConfig.h"
#include "HttpHelper.h"
#include "NodeConfigHelper.h"

const int jsonDocSize = 300;       // size of static JSON document
const String nodeId = "ESP32_01";  // ID of Harvey node

unsigned long lastConfigUpdateTime = 0;    // Holds value for millis() for timing config update API calls
unsigned long configUpdateDelay = 3600000; // Delay between config update calls

unsigned long lastApiCallTime = 0;   // Holds value for millis() for timing other API calls
unsigned long apiCallDelay = 60000;  // Delay between calls to API

void setup() {
  Serial.begin(115200);
  connectToNetwork();
  if (WiFi.status() == WL_CONNECTED) {
    getHarveyNodeConfig();
  }
}

void loop() {
  if ((millis() - lastTime) > apiCallDelay) {
  if ((millis() - lastConfigUpdateTime) > loginDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      getHarveyNodeConfig();
    }
    lastConfigUpdateTime = millis();
  }

  if ((millis() - lastApiCallTime) > apiCallDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      sendGardenBedData();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastApiCallTime = millis();
  }
}

void connectToNetwork() {
  const int connectionDelay = 500;   // Delay for output when connecting to wifi

  WiFi.begin(ssid, wifiPassword);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(connectionDelay);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


  StaticJsonDocument<jsonDocSize> doc;



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
