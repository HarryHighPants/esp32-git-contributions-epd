#pragma once
#include "DeviceConfig.h"

struct Contributions {
  int week[WEEKS * 7] = {};
};

struct State {
  Contributions contributions;
  bool showingWifiError = false;
  bool showingFetchError = false;

  bool storeContributionIfChanged(const Contributions* newContributions);
  void clearContributions();
};
