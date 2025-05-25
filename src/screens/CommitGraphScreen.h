#pragma once
#include "ContributionsApi.h"
#include "DisplayController.h"
#include "UserConfig.h"
#include "WifiController.h"

struct CommitGraphScreenState {
  CommitGraphContributions contributions;
  bool showingWifiError = false;
  bool showingFetchError = false;

  bool storeContributionIfChanged(const CommitGraphContributions *newContributions);
};

class CommitGraphScreen {
 public:
  explicit CommitGraphScreen(DisplayController *displayController, WifiController *wifiController,
                             ContributionsApi *contributionsApi, UserConfig *config, CommitGraphScreenState *state);
  void fetchAndDraw() const;
  void resetState() const;

 private:
  void draw() const;
  void drawWifiError() const;
  void drawFetchError() const;
  DisplayController *displayController;
  WifiController *wifiController;
  CommitGraphScreenState *state;
  ContributionsApi *contributionsApi;
  UserConfig *config;
};
