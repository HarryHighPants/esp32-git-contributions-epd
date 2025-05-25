#include <Arduino.h>
#include <DeviceConfig.h>
#include <WiFi.h>

#include "CaptiveConfigServer.h"
#include "ContributionsApi.h"
#include "DisplayController.h"
#include "SleepController.h"
#include "UserConfig.h"
#include "WifiController.h"
#include "screens/CommitGraphScreen.h"
#include "screens/ConfigModeScreen.h"

// RTC memory - Clears when the device is powered off but is retained on sleep
RTC_DATA_ATTR UserConfig config;
RTC_DATA_ATTR CommitGraphScreenState commitGraphScreenState;

// Controllers
DisplayController displayController(config.darkMode);
SleepController sleepController(&config);
WifiController wifiController(&config);

// Tools
CaptiveConfigServer captiveConfigServer(&config);
ContributionsApi contributionsApi(&config);

// Screens
ConfigModeScreen configModeScreen(&displayController);
CommitGraphScreen commitGraphScreen(&displayController, &wifiController, &contributionsApi, &config,
                                    &commitGraphScreenState);

// Reset all screen states
void resetScreenStates() { commitGraphScreen.resetState(); }

// Load config and restart the device
void loadConfigAndRestart() {
  Serial.println("Starting config server");
  configModeScreen.draw();

  captiveConfigServer.begin();
  if (captiveConfigServer.getConfig()) {
    Serial.println("Successfully loaded new config");
    resetScreenStates();
  } else {
    Serial.println("Failed to load new config");
  }
  captiveConfigServer.end();

  Serial.println("restarting");
  esp_deep_sleep(1e6);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);

  const WakeReason wakeReason = SleepController::getWakeReason();
  if (wakeReason == BUTTON_HOLD) {
    Serial.println("Button held, entering config mode");
    loadConfigAndRestart();
    // This shouldn't be reached, as loadConfigAndRestart() will restart the device
    return;
  }

  if (!config.isSet()) {
    Serial.println("Missing critical details, entering setup");
    loadConfigAndRestart();
    // This shouldn't be reached, as loadConfigAndRestart() will restart the device
    return;
  }

  commitGraphScreen.fetchAndDraw();
}

void loop() { sleepController.deepSleep(); }
