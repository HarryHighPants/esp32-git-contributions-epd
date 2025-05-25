#pragma once
#include <Arduino.h>

#include "DeviceConfig.h"
#include "UserConfig.h"

struct CommitGraphContributions {
  int week[WEEKS * 7] = {};
};

class ContributionsApi {
 public:
  explicit ContributionsApi(UserConfig* config);
  bool fetchContributionsData(CommitGraphContributions* contributions) const;

 private:
  UserConfig* config;
  String fetchHttpResponse() const;
};
