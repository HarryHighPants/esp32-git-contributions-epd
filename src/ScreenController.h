#pragma once

#include <Arduino.h>

#include "State.h"
#include "UserConfig.h"

struct Colors {
  uint16_t background;
  uint16_t lightForeground;
  uint16_t darkForeground;
  uint16_t foreground;

  uint16_t fromLevel(int level) const;
  Colors(bool darkMode);
};

class ScreenController {
 public:
  ScreenController(UserConfig* config, State* state);
  void drawCommitGraph();
  void drawConfigModeScreen();

 private:
  void initScreen();
  void drawHeader();
  UserConfig* config;
  State* state;
  Colors colors;
};
