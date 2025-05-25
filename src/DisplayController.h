#pragma once

#include <Arduino.h>
#include <DeviceConfig.h>

struct DisplayColors {
  explicit DisplayColors(bool darkMode);
  uint16_t fromLevel(int level) const;

  uint16_t background;
  uint16_t lightForeground;
  uint16_t darkForeground;
  uint16_t foreground;
};

class DisplayController {
 public:
  explicit DisplayController(bool darkMode);
  void drawBackground();
  void drawText(const String &text, int x, int y);
  void drawText(const String &text, int x, int y, int textSize);
  void displayAndHibernate();

  GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display;
  DisplayColors colors;
};
