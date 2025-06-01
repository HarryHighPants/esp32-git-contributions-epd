#include "TimeUtils.h"

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP udp;
NTPClient ntp(udp);
long TimeUtils::timeSetAt;

void TimeUtils::setDeviceTime(const WifiController *wifiController, const UserConfig *config) {
  if (timeSetAt != 0 && millis() - timeSetAt < 3600000) {  // 1 hour cooldown
    Serial.println("Device time was already set recently, skipping");
    return;
  }

  if (!wifiController->connectWifi()) {
    Serial.println("Not connected to WiFi, can't set device time");
    return;
  }

  if (!ntp.update()) {
    Serial.println("Failed to obtain time from NTP");
    return;
  }

  Serial.println("Time obtained from NTP");
  const time_t epochTime = ntp.getEpochTime();
  timeval tv = {epochTime, 0};
  // TODO: Figure out if this is maintained through reboots
  settimeofday(&tv, nullptr);
  // TODO: Figure out if re-running this causes the time offset to be cooked
  setenv("TZ", config->display.timezone, 1);
  tzset();
  timeSetAt = millis();
  Serial.println("Device time set to: " + nowString());
}

String TimeUtils::nowString() {
  char timeString[30];
  const time_t now = time(nullptr);
  const tm *nowInfo = localtime(&now);
  strftime(timeString, 30, "%Y-%m-%d %I:%M:%S %p", nowInfo);
  return String(timeString);
}
