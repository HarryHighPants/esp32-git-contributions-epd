#pragma once
#include "UserConfig.h"

class SleepController {
 public:
  enum WakeReason {
    BUTTON_PRESS,
    BUTTON_HOLD,
    TIMER,
    UNKNOWN,
  };

  explicit SleepController(UserConfig *config);
  void deepSleep() const;
  static WakeReason getWakeReason();

 private:
  UserConfig *config;
};
