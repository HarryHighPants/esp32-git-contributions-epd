#pragma once
#include <Arduino.h>

#include "DeviceConfig.h"
#include "UserConfig.h"

class ContributionsApi {
 public:
  struct Data {
    int week[WEEKS * 7] = {};
  };

  explicit ContributionsApi(UserConfig* config);
  bool fetchContributionsData(Data* contributions) const;

 private:
  UserConfig* config;
  String fetchHttpResponse() const;
};
