#pragma once
#include <Arduino.h>

#include "UserConfig.h"

class CaptiveConfigServer {
 public:
  explicit CaptiveConfigServer(UserConfig* config);
  void begin();
  bool getConfig() const;
  static void end();

 private:
  UserConfig* config;
  String configPageHtml;
  bool loaded;
};
