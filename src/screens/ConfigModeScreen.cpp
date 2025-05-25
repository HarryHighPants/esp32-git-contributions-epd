#include "ConfigModeScreen.h"

#include "assets/WifiIcon.h"

ConfigModeScreen::ConfigModeScreen(DisplayController *displayController) {
  this->displayController = displayController;
}

void ConfigModeScreen::draw() const {
  displayController->drawBackground();

  constexpr int margin = 25;
  constexpr int lineHeight = 12;
  displayController->display.drawBitmap(displayController->display.width() - margin - 32, margin - 8, WifiIcon, 32, 32,
                                        displayController->colors.foreground);

  // Title
  displayController->drawText("Config Mode", margin, margin + 14, 2);

  // Details
  constexpr int paragraphY = margin + 36;
  displayController->drawText("Connect to my wifi", margin, paragraphY);
  displayController->drawText("'Contributions Screen'", margin, paragraphY + lineHeight);
  displayController->drawText("Then press 'sign in' to open", margin, paragraphY + lineHeight * 3);
  displayController->drawText("the browser configuration", margin, paragraphY + lineHeight * 4);

  // Draw the updates
  displayController->displayAndHibernate();
}
