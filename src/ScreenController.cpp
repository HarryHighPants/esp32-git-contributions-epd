#include "ScreenController.h"

#include <Arduino.h>
#include <DeviceConfig.h>

#include "assets/BrokenLinkIcon.h"
#include "assets/GithubIcon.h"
#include "assets/WifiErrorIcon.h"
#include "assets/WifiIcon.h"
#include "assets/muMatrix8ptRegular.h"

GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, GxEPD2_DRIVER_CLASS::HEIGHT> display(
    GxEPD2_DRIVER_CLASS(EPD_PIN_CS, EPD_PIN_DC, EPD_PIN_RST, EPD_PIN_BUSY));

Colors::Colors(const bool darkMode) {
  background = darkMode ? GxEPD_BLACK : GxEPD_WHITE;
  lightForeground = darkMode ? GxEPD_DARKGREY : GxEPD_LIGHTGREY;
  darkForeground = darkMode ? GxEPD_LIGHTGREY : GxEPD_DARKGREY;
  foreground = darkMode ? GxEPD_WHITE : GxEPD_BLACK;
}

uint16_t Colors::fromLevel(const int level) const {
  if (level <= 0) return this->background;
  if (level == 1) return this->lightForeground;
  if (level == 2) return this->darkForeground;
  return this->foreground;
}

ScreenController::ScreenController(UserConfig* config, State* state)
    : colors(config->darkMode) {
  this->config = config;
  this->state = state;
}

void ScreenController::initScreen() {
  display.init(115200, true, 2, false);
  display.setRotation(1);  // Landscape
  display.setTextColor(this->colors.foreground);
  display.setFont(&muMatrix8ptRegular);
  display.setTextSize(1);

  // Set background
  display.fillRect(0, 0, display.width(), display.height(),
                   this->colors.background);
}

void ScreenController::drawHeader() {
  // Draw Github icon
  display.drawBitmap(7, 8, GithubIcon, 16, 16, this->colors.foreground);

  // Draw username
  display.setCursor(29, 20);
  display.print("/" + String(this->config->username));

  // Draw battery percentage
  display.setCursor(display.width() - 25, 20);
  const int adcValue = analogRead(BAT_TEST_PIN);
  const float voltage = adcValue / 4095.0 * 3.3 * 2;
  long percentage =
      map(voltage * 1000, 3200, 3900, 0, 100);  // Use millivolts for mapping
  display.printf("%ld%%", percentage);

  // Draw error icons
  if (this->state->showingWifiError || this->state->showingFetchError) {
    display.drawBitmap(
        display.width() - 55, 8,
        this->state->showingWifiError ? WifiErrorIcon : BrokenLinkIcon, 16, 16,
        this->colors.foreground);
  }
}

void ScreenController::drawCommitGraph() {
  this->initScreen();
  this->drawHeader();

  int columns = WEEKS;
  constexpr int rows = 7;
  const int total = columns * rows;
  int squareMargin = 2;
  int gridSize = display.width() / columns;
  int squareSize = gridSize - squareMargin;
  int borderThickness = 3;
  int yOffset = 24;
  int xOffset = 5;

  // Draw squares
  for (int column = 0; column < columns; column++) {
    for (int row = 0; row < rows; row++) {
      int squareX = column * gridSize + xOffset + squareMargin;
      int squareY = row * gridSize + yOffset + squareMargin;
      const int index = rows * column + row;
      const int level = this->state->contributions.week[index];

      display.fillRoundRect(squareX, squareY, squareSize, squareSize,
                            borderThickness, this->colors.fromLevel(level));
      display.drawRoundRect(squareX, squareY, squareSize, squareSize,
                            borderThickness, this->colors.fromLevel(level + 1));

      const bool isToday =
          level != -1 && (index == total - 1 ||
                          this->state->contributions.week[index + 1] == -1);
      if (isToday) {
        display.drawRoundRect(squareX, squareY, squareSize, squareSize,
                              borderThickness, this->colors.foreground);
      }
    }
  }

  // Draw the updates
  display.displayWindow(0, 0, display.width(), display.height());
  display.hibernate();
}

void ScreenController::drawConfigModeScreen() {
  this->initScreen();

  int margin = 25;
  constexpr int lineHeight = 12;
  display.drawBitmap(display.width() - margin - 32, margin - 8, WifiIcon, 32,
                     32, this->colors.foreground);

  // Title
  display.setTextSize(2);
  display.setCursor(margin, margin + 14);
  display.print("Config Mode");

  // Details
  display.setTextSize(1);
  const int paragraphY = margin + 36;
  display.setCursor(margin, paragraphY);
  display.print("Connect to my wifi");
  display.setCursor(margin, paragraphY + lineHeight);
  display.print("'Contributions Screen'");
  display.setCursor(margin, paragraphY + lineHeight * 3);
  display.print("Then press 'sign in' to open");
  display.setCursor(margin, paragraphY + lineHeight * 4);
  display.print("the browser configuration");

  // Draw the updates
  display.displayWindow(0, 0, display.width(), display.height());
  display.hibernate();
}
