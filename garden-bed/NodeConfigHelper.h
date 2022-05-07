void getHarveyNodeConfig() {
  String currentPath = serverName + "/api/node/" + nodeId + "/config";

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
