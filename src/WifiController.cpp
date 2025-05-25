#include "WifiController.h"

#include <WiFi.h>

#include "UserConfig.h"

WifiController::WifiController(UserConfig *config) { this->config = config; }

bool WifiController::connectWifi() const {
  constexpr unsigned long timeout = 6000;  // 6 seconds timeout

  WiFi.begin(config->wifiSSID, config->wifiPassword);

  Serial.print("\nConnecting to Wifi: ");
  const unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    return true;
  }

  Serial.println("\nWiFi connection failed");
  return false;
}
