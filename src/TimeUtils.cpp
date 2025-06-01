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
  return {timeString};
}

String TimeUtils::shortNowString() {
  const time_t now = time(nullptr);
  const tm *nowInfo = localtime(&now);

  char day[3];
  strftime(day, 3, "%d", nowInfo);
  if (day[0] == '0') {
    day[0] = day[1];
    day[1] = '\0';  // Remove leading zero
  }

  char month[5];
  strftime(month, 5, "%b", nowInfo);

  char hour[3];
  strftime(hour, 3, "%I", nowInfo);
  if (hour[0] == '0') {
    hour[0] = hour[1];
    hour[1] = '\0';  // Remove leading zero
  }

  char mins[3];
  strftime(mins, 3, "%M", nowInfo);

  char amPm[3];
  strftime(amPm, 3, "%p", nowInfo);
  if (amPm[0] == 'A') {
    amPm[0] = 'a';
  } else {
    amPm[0] = 'p';
  }
  amPm[1] = 'm';

  char timeString[20];
  snprintf(timeString, sizeof(timeString), "%s %s  %s:%s%s", day, month, hour, mins, amPm);
  return {timeString};
}
