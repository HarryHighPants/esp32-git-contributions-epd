#pragma once
#include "ContributionsApi.h"
#include "DisplayController.h"
#include "UserConfig.h"
#include "WifiController.h"

class CommitGraphScreen {
 public:
  struct State {
    ContributionsApi::Data contributions;
    bool showingWifiError = false;
    bool showingFetchError = false;

    bool storeContributionIfChanged(const ContributionsApi::Data *newContributions);
  };

  explicit CommitGraphScreen(DisplayController *displayController, WifiController *wifiController,
                             ContributionsApi *contributionsApi, UserConfig *config, State *state);
  void fetchAndDraw() const;
  void resetState() const;

 private:
  void draw() const;
  void drawWifiError() const;
  void drawFetchError() const;
  DisplayController *displayController;
  WifiController *wifiController;
  ContributionsApi *contributionsApi;
  UserConfig *config;
  State *state;
};
