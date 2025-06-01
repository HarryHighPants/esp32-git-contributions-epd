#include "ContributionsApi.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "DeviceConfig.h"

WiFiClientSecure client;
HTTPClient http;

ContributionsApi::ContributionsApi(UserConfig* config) { this->config = config; }

String ContributionsApi::fetchHttpResponse() const {
  const String url =
      String(config->commitGraph.apiUrl) + String(config->commitGraph.username) + "?weeks=" + String(WEEKS);

  Serial.println("Fetching contributions data from: " + url);
  client.setInsecure();
  http.begin(client, url);
  const int responseCode = http.GET();
  Serial.printf("\nHTTP Response code: %d\n", responseCode);
  String payload = http.getString();
  http.end();

  Serial.println("Got response:");
  Serial.println(payload);

  return payload;
}

bool ContributionsApi::fetchContributionsData(Data* contributions) const {
  JsonDocument doc;
  String payload = fetchHttpResponse();
  // Parse response
  // 2 weeks = [1, 0, 3, 1, 4, 0, 0, 1, 0, 3, 1, 4, 0, -1, -1]
  const DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return false;
  }

  const JsonArray jsonArray = doc.as<JsonArray>();

  if (jsonArray.size() != 7 * WEEKS) {
    Serial.println("Invalid response, expected " + String(7 * WEEKS) + " items, got " + String(jsonArray.size()));
    return false;
  }

  for (int i = 0; i < 7 * WEEKS; i++) {
    contributions->week[i] = jsonArray[i].as<int>();
  }

  return true;
}
