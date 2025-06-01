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
}

void DisplayController::drawBackground() {
  // Fill the background with the configured color
  display.fillRect(0, 0, display.width(), display.height(), colors.background);
}

void DisplayController::drawText(const String &text, const int x, const int y) { drawText(text, x, y, 1); }

void DisplayController::drawText(const String &text, const int x, const int y, const int textSize) {
  drawText(text, x, y, textSize, colors.foreground);
}

void DisplayController::drawText(const String &text, const int x, const int y, const int textSize,
                                 const uint16_t color) {
  drawText(text, x, y, textSize, color, &muMatrix8ptRegular);
}

void DisplayController::drawText(const String &text, const int x, const int y, const GFXfont *font) {
  drawText(text, x, y, 1, colors.foreground, font);
}

void DisplayController::drawText(const String &text, const int x, const int y, const int textSize, const uint16_t color,
                                 const GFXfont *font) {
  display.setTextColor(color);
  display.setFont(font);
  display.setTextSize(1);
  display.setTextSize(textSize);
  display.setCursor(x, y);
  display.print(text);
}

uint16_t DisplayController::getTextWidth(const String &text, const GFXfont *font) {
  uint16_t textWidth;
  uint16_t a;
  int16_t b;
  int16_t c;
  display.getTextBounds(text, 0, 0, &b, &c, &textWidth, &a);
  return textWidth;
}

void DisplayController::drawBatteryIcon(const int x, const int y, const int percentage) {
  // 1 column on left, 2 columns on right, 5 columns of battery body
  constexpr int batteryWidth = 8;
  constexpr int batteryHeight = 5;

  // Top line
  display.drawLine(x, y, x + batteryWidth - 2, y, colors.darkForeground);
  // Bottom line
  display.drawLine(x, y + batteryHeight - 1, x + batteryWidth - 2, y + batteryHeight - 1, colors.darkForeground);
  // Left line
  display.drawLine(x, y, x, y + batteryHeight - 1, colors.darkForeground);
  // Batter end
  display.drawLine(x + batteryWidth - 2, y, x + batteryWidth - 2, y + batteryHeight - 1, colors.darkForeground);
  display.drawLine(x + batteryWidth - 1, y + 1, x + batteryWidth - 1, y + batteryHeight - 2, colors.darkForeground);

  // The +1 is to round up, so that we always fill at least one pixel
  int filledWidth = percentage * (batteryWidth - 3) / 100 + 1;
  if (filledWidth > batteryWidth - 3) {
    filledWidth = batteryWidth - 3;  // Ensure we don't overflow
  }
  display.fillRect(x + 1, y + 1, filledWidth, batteryHeight - 2, colors.lightForeground);
}

void DisplayController::displayAndHibernate() {
  // Display the current buffer and then hibernate
  display.display(true);
  display.hibernate();
}
