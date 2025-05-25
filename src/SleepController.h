#pragma once
#include "UserConfig.h"

enum WakeReason {
  BUTTON_PRESS,
  BUTTON_HOLD,
  TIMER,
  UNKNOWN,
};

class SleepController {
 public:
  explicit SleepController(UserConfig *config);
  void deepSleep() const;
  static WakeReason getWakeReason();

 private:
  UserConfig *config;
};
