#include "State.h"

// Override the contributions and return true if any changed
bool State::storeContributionIfChanged(const Contributions* newContributions) {
  bool changed = false;

  for (int i = 0; i < 7 * WEEKS; i++) {
    if (this->contributions.week[i] != newContributions->week[i]) {
      this->contributions.week[i] = newContributions->week[i];
      changed = true;
    }
  }

  return changed;
}

void State::clearContributions() { this->contributions = Contributions(); }
