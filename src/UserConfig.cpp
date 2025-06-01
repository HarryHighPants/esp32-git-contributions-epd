#include "UserConfig.h"

#include <Arduino.h>
#include <Preferences.h>

const char* UserConfig::PREFERENCES_NAMESPACE = "user_config";

bool UserConfig::Wifi::isSet() const { return strlen(ssid) > 0; }

bool UserConfig::CommitGraph::isSet() const { return strlen(username) > 0 && strlen(apiUrl) > 0; }

bool UserConfig::isSet() const { return wifi.isSet() && commitGraph.isSet(); }

UserConfig UserConfig::load() {
  UserConfig config;
  Preferences prefs;

  if (!prefs.begin(PREFERENCES_NAMESPACE, true)) {  // true = read-only
    Serial.println("Failed to open preferences for reading");
    return config;  // Return default config
  }

  // Load WiFi config
  prefs.getString("wifi_ssid", config.wifi.ssid, sizeof(config.wifi.ssid));
  prefs.getString("wifi_pass", config.wifi.password, sizeof(config.wifi.password));

  // Load Commit Graph config
  prefs.getString("commit_user", config.commitGraph.username, sizeof(config.commitGraph.username));
  prefs.getString("commit_url", config.commitGraph.apiUrl, sizeof(config.commitGraph.apiUrl));

  // Load Display config
  config.display.darkMode = prefs.getBool("dark_mode", false);

  // Load Energy config
  config.energy.syncInterval = prefs.getInt("sync_interval", 4);  // Default 4 hours

  prefs.end();

  Serial.println("Configuration loaded from preferences");
  return config;
}

void UserConfig::save() {
  Preferences prefs;

  if (!prefs.begin(PREFERENCES_NAMESPACE, false)) {  // false = read-write
    Serial.println("Failed to open preferences for writing");
    return;
  }

  // Save WiFi config
  prefs.putString("wifi_ssid", wifi.ssid);
  prefs.putString("wifi_pass", wifi.password);

  // Save Commit Graph config
  prefs.putString("commit_user", commitGraph.username);
  prefs.putString("commit_url", commitGraph.apiUrl);

  // Save Display config
  prefs.putBool("dark_mode", display.darkMode);

  // Save Energy config
  prefs.putInt("sync_interval", energy.syncInterval);

  prefs.end();

  Serial.println("Configuration saved to preferences");
}
