#pragma once
#include "UserConfig.h"

class PowerController {
 public:
  enum WakeReason {
    BUTTON_PRESS,
    BUTTON_HOLD,
    TIMER,
    UNKNOWN,
  };

  explicit PowerController(UserConfig *config);
  void deepSleep() const;
  static WakeReason getWakeReason();
  static int getBatteryPercentage();

 private:
  UserConfig *config;
};
