#include "DisplayController.h"

#include <Arduino.h>
#include <DeviceConfig.h>

#include "assets/muMatrix8ptRegular.h"

DisplayController::Colors::Colors(const bool darkMode) {
  background = darkMode ? GxEPD_BLACK : GxEPD_WHITE;
  lightForeground = darkMode ? GxEPD_DARKGREY : GxEPD_LIGHTGREY;
  darkForeground = darkMode ? GxEPD_LIGHTGREY : GxEPD_DARKGREY;
  foreground = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
}

uint16_t DisplayController::Colors::fromLevel(const int level) const {
  if (level <= 0) return background;
  if (level == 1) return lightForeground;
  if (level == 2) return darkForeground;
  return foreground;
}

DisplayController::DisplayController(bool darkMode)
    : display(GxEPD2_DRIVER_CLASS(EPD_PIN_CS, EPD_PIN_DC, EPD_PIN_RST, EPD_PIN_BUSY)), colors(darkMode) {
  display.init(115200, true, 2, false);
  display.setRotation(1);  // Landscape
  display.setTextColor(colors.foreground);
  display.setFont(&muMatrix8ptRegular);
  display.setTextSize(1);
}

void DisplayController::drawBackground() {
  // Fill the background with the configured color
  display.fillRect(0, 0, display.width(), display.height(), colors.background);
}

void DisplayController::drawText(const String &text, const int x, const int y) { drawText(text, x, y, 1); }

void DisplayController::drawText(const String &text, const int x, const int y, const int textSize) {
  display.setTextSize(textSize);
  display.setCursor(x, y);
  display.print(text);
}

void DisplayController::displayAndHibernate() {
  // Display the current buffer and then hibernate
  display.display(true);
  display.hibernate();
}
