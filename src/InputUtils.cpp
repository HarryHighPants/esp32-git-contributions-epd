#include "InputUtils.h"

#include <Arduino.h>

#include "DeviceConfig.h"

bool InputUtils::isButtonPressed() { return digitalRead(BUTTON_PIN) == LOW; }

bool InputUtils::isButtonHeld(const long duration) {
  const auto now = millis();
  while (isButtonPressed()) {
    if (millis() - now > duration) {
      return true;
    }
    delay(100);  // Small delay to avoid busy waiting
  }
  return false;  // Button released before the duration
}
