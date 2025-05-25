#pragma once
#include "UserConfig.h"

class WifiController {
 public:
  explicit WifiController(UserConfig *config);
  bool connectWifi() const;

 private:
  UserConfig *config;
};
