#pragma once

#include <Arduino.h>
#include <DeviceConfig.h>

class DisplayController {
 public:
  struct Colors {
    explicit Colors(bool darkMode);
    uint16_t fromLevel(int level) const;

    uint16_t background;
    uint16_t lightForeground;
    uint16_t darkForeground;
    uint16_t foreground;
  };

  explicit DisplayController(bool darkMode);
  void drawBackground();
  void drawText(const String &text, int x, int y);
  void drawText(const String &text, int x, int y, int textSize);
  void drawText(const String &text, int x, int y, int textSize, uint16_t color);
  void drawText(const String &text, int x, int y, const GFXfont *font);
  void drawText(const String &text, int x, int y, int textSize, uint16_t color, const GFXfont *font);
  uint16_t getTextWidth(const String &text, const GFXfont *font);
  void drawBatteryIcon(int x, int y, int percentage);
  void displayAndHibernate();

  GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display;
  Colors colors;
};
