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

int httpPOSTRequest(String requestPath, String body) {
  HTTPClient http;

  http.begin(requestPath.c_str());

  http.addHeader("Content-Type", "application/json");

  int respCode = http.POST(body);

  String payload = "{}";

  if (respCode > 0) {
    payload = http.getString();
  }

  http.end();

  logHttpResponse("POST", requestPath, respCode, payload);

  return respCode;
}
