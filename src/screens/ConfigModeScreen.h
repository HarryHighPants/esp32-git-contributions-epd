#pragma once
#include "DisplayController.h"

class ConfigModeScreen {
 public:
  explicit ConfigModeScreen(DisplayController *displayController);
  void draw() const;

 private:
  DisplayController *displayController;
};
