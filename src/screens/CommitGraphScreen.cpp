#include "CommitGraphScreen.h"

#include "assets/BrokenLinkIcon.h"
#include "assets/GithubIcon.h"
#include "assets/WifiErrorIcon.h"

// Override the contributions and return true if any changed
bool CommitGraphScreen::State::storeContributionIfChanged(const ContributionsApi::Data *newContributions) {
  bool changed = false;

  for (int i = 0; i < 7 * WEEKS; i++) {
    if (contributions.week[i] != newContributions->week[i]) {
      contributions.week[i] = newContributions->week[i];
      changed = true;
    }
  }

  return changed;
}

CommitGraphScreen::CommitGraphScreen(DisplayController *displayController, WifiController *wifiController,
                                     ContributionsApi *contributionsApi, UserConfig *config, State *state) {
  this->displayController = displayController;
  this->wifiController = wifiController;
  this->contributionsApi = contributionsApi;
  this->config = config;
  this->state = state;
}

int getBatteryPercentage() {
  const int adcValue = analogRead(BAT_TEST_PIN);
  const float voltage = adcValue / 4095.0 * 3.3 * 2;
  return static_cast<int>(map(voltage * 1000, 3200, 3900, 0, 100));
}

void CommitGraphScreen::draw() const {
  displayController->drawBackground();

  // Draw Github icon and username
  displayController->display.drawBitmap(7, 8, GithubIcon, 16, 16, displayController->colors.foreground);
  displayController->drawText("/" + String(config->commitGraph.username), 29, 20);

  // Draw battery percentage
  displayController->drawText(String(getBatteryPercentage()) + "%", displayController->display.width() - 25, 20);

  // Draw error icons
  if (state->showingWifiError || state->showingFetchError) {
    displayController->display.drawBitmap(displayController->display.width() - 55, 8,
                                          state->showingWifiError ? WifiErrorIcon : BrokenLinkIcon, 16, 16,
                                          displayController->colors.foreground);
  }

  constexpr int columns = WEEKS;
  constexpr int rows = 7;
  constexpr int total = columns * rows;
  constexpr int squareMargin = 2;
  const int gridSize = displayController->display.width() / columns;
  const int squareSize = gridSize - squareMargin;
  constexpr int borderThickness = 3;
  constexpr int yOffset = 24;
  constexpr int xOffset = 5;

  // Draw squares
  for (int column = 0; column < columns; column++) {
    for (int row = 0; row < rows; row++) {
      const int squareX = column * gridSize + xOffset + squareMargin;
      const int squareY = row * gridSize + yOffset + squareMargin;
      const int index = rows * column + row;
      const int level = state->contributions.week[index];

      displayController->display.fillRoundRect(squareX, squareY, squareSize, squareSize, borderThickness,
                                               displayController->colors.fromLevel(level));
      displayController->display.drawRoundRect(squareX, squareY, squareSize, squareSize, borderThickness,
                                               displayController->colors.fromLevel(level + 1));

      const bool isToday = level != -1 && (index == total - 1 || state->contributions.week[index + 1] == -1);
      if (isToday) {
        displayController->display.drawRoundRect(squareX, squareY, squareSize, squareSize, borderThickness,
                                                 displayController->colors.foreground);
      }
    }
  }

  // Draw the updates
  displayController->displayAndHibernate();
}

void CommitGraphScreen::fetchAndDraw() const {
  // Fetch data from API
  const bool connected = wifiController->connectWifi();
  if (!connected) {
    Serial.println("Failed to connect to wifi");
    drawWifiError();
    return;
  }
  state->showingWifiError = false;

  ContributionsApi::Data newContributions;
  const bool fetchSuccessful = contributionsApi->fetchContributionsData(&newContributions);
  if (!fetchSuccessful) {
    Serial.println("Failed to fetch contributions data");
    drawFetchError();
    return;
  }
  state->showingFetchError = false;

  if (!state->storeContributionIfChanged(&newContributions)) {
    Serial.println("No changes in contributions");
    return;
  }

  draw();
}

void CommitGraphScreen::drawWifiError() const {
  // If not previously showing wifi failure, update screen
  if (!state->showingWifiError) {
    state->showingWifiError = true;
    draw();
  }
}

void CommitGraphScreen::drawFetchError() const {
  // If not previously showing wifi failure, update screen
  if (!state->showingFetchError) {
    state->showingFetchError = true;
    draw();
  }
}

void CommitGraphScreen::resetState() const {
  state->contributions = ContributionsApi::Data();
  state->showingWifiError = false;
  state->showingFetchError = false;
}
