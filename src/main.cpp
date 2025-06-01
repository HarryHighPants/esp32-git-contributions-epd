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

UserConfig config;
RTC_DATA_ATTR CommitGraphScreen::State commitGraphScreenState;

// Controllers
DisplayController displayController(config.display.darkMode);
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
    config.save();
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

  // Load config from NVS
  delay(10);  // TODO: Would love to remove this, but it seems like config.load() hangs if there is no delay in here
  config.load();

  const SleepController::WakeReason wakeReason = SleepController::getWakeReason();
  if (wakeReason == SleepController::WakeReason::BUTTON_HOLD) {
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
