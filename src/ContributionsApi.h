#pragma once
#include <Arduino.h>

#include "State.h"
#include "UserConfig.h"

class ContributionsApi {
 public:
  ContributionsApi(UserConfig* config);
  bool fetchContributionsData(Contributions* contributions);

 private:
  UserConfig* config;
  String fetchHttpResponse();
};
