#include "UserConfig.h"

#include <Arduino.h>

bool UserConfig::isSet() const { return strlen(wifiSSID) > 0 && strlen(username) > 0; };
