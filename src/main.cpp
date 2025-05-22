#include <Arduino.h>
#include <DeviceConfig.h>
#include <WiFi.h>

#include "CaptiveConfigServer.h"
#include "ContributionsApi.h"
#include "ScreenController.h"
#include "State.h"
#include "UserConfig.h"

// RTC memory - Clears when the device is powered off but is retained on sleep
RTC_DATA_ATTR UserConfig config;
RTC_DATA_ATTR State state;

CaptiveConfigServer captiveConfigServer(&config);
ContributionsApi contributionsApi(&config);
ScreenController screenController(&config, &state);

bool isHoldingButton() {
  if (digitalRead(BUTTON_PIN) != LOW) {
    return false;
  }
  delay(2000);
  return digitalRead(BUTTON_PIN) == LOW;
}

void loadConfigAndRestart() {
  Serial.println("Starting config server");
  screenController.drawConfigModeScreen();

  captiveConfigServer.begin();
  if (captiveConfigServer.getConfig()) {
    Serial.println("Successfully loaded new config");
    state.clearContributions();
  } else {
    Serial.println("Failed to load new config");
  }
  captiveConfigServer.end();

  Serial.println("restarting");
  esp_deep_sleep(1e6);
}

bool connectWifi() {
  constexpr unsigned long timeout = 6000;  // 6 seconds timeout
  WiFi.begin(config.wifiSSID, config.wifiPassword);
  Serial.print("\nConnecting to Wifi: ");
  const unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < timeout) {
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

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);

  const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:  // Wakeup caused by GPIO (external wakeup)
      Serial.println(
          "Wakeup caused by external signal using GPIO (button press)");
      if (isHoldingButton()) {
        loadConfigAndRestart();
        return;
      }
      break;
    case ESP_SLEEP_WAKEUP_TIMER:  // Wakeup caused by the timer
      Serial.println("Wakeup caused by timer (deep sleep duration complete)");
      break;
    default:  // Other wakeup reasons
      Serial.printf("Wakeup not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }

  if (strlen(config.wifiSSID) == 0 || strlen(config.username) == 0) {
    Serial.println("Missing critical details, entering setup");
    loadConfigAndRestart();
    return;
  }

  const bool connected = connectWifi();
  if (!connected) {
    Serial.println("Failed to connect to wifi");

    // If not previously showing wifi failure, update screen
    if (!state.showingWifiError) {
      state.showingWifiError = true;
      screenController.drawCommitGraph();
    }
    return;
  }
  state.showingWifiError = false;

  Contributions newContributions;
  const bool fetchSuccessful =
      contributionsApi.fetchContributionsData(&newContributions);
  if (!fetchSuccessful) {
    Serial.println("Failed to fetch contributions data");
    state.showingFetchError = true;
    screenController.drawCommitGraph();
    return;
  }
  state.showingFetchError = false;
  if (!state.storeContributionIfChanged(&newContributions)) {
    Serial.println("No changes in contributions");
    return;
  }

  screenController.drawCommitGraph();
}

void loop() {
  // Set up the additional IO button as a wakeup source on LOW signal (when the
  // button is pressed)
  esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BUTTON_PIN), LOW);

  Serial.println("Going to sleep for " + String(config.syncInterval) +
                 " hours");
  esp_deep_sleep(config.syncInterval * 3600e6);  // syncInterval * 1 hour
}
