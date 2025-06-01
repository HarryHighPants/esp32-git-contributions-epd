#pragma once

#include <Arduino.h>

#include "UserConfig.h"
#include "WifiController.h"

class TimeUtils {
 public:
  static void setDeviceTime(const WifiController *wifiController, const UserConfig *config);
  static String nowString();

 private:
  static RTC_DATA_ATTR long timeSetAt;
};
