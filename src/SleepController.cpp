#include "SleepController.h"

#include <Arduino.h>

#include "DeviceConfig.h"
#include "InputUtils.h"

SleepController::SleepController(UserConfig *config) { this->config = config; }

void SleepController::deepSleep() const {
  // Set up the additional IO button as a wakeup source on LOW signal (when the
  // button is pressed)
  esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BUTTON_PIN), LOW);

  Serial.println("Going to sleep for " + String(config->syncInterval) + " hours");
  esp_deep_sleep(static_cast<unsigned long long>(3600e6) * config->syncInterval);  // syncInterval * 1 hour
}

WakeReason SleepController::getWakeReason() {
  const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
    // Wakeup caused by GPIO (external wakeup)
    Serial.println("Wakeup caused by external signal using GPIO (button press)");
    if (InputUtils::isButtonHeld(2000)) {
      return BUTTON_HOLD;
    }
    return BUTTON_PRESS;
  }

  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    // Wakeup caused by the timer
    Serial.println("Wakeup caused by timer (deep sleep duration complete)");
    return TIMER;
  }

  Serial.printf("Wakeup not caused by deep sleep: %d\n", wakeup_reason);
  return UNKNOWN;  // Unknown wakeup reason
}
