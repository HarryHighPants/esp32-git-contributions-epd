#include "UserConfig.h"

#include <Arduino.h>
#include <Preferences.h>

Preferences prefs;

const char* UserConfig::PREFERENCES_NAMESPACE = "user_config";
const int UserConfig::VERSION = 1;

bool UserConfig::Wifi::isSet() const { return strlen(ssid) > 0; }

bool UserConfig::CommitGraph::isSet() const { return strlen(username) > 0 && strlen(apiUrl) > 0; }

bool UserConfig::isSet() const { return wifi.isSet() && commitGraph.isSet(); }

bool UserConfig::load() {
  if (!prefs.begin(PREFERENCES_NAMESPACE, true)) {  // true = read-only
    Serial.println("Failed to open preferences for reading");
    return false;  // Return default config
  }

  const int storedVersion = prefs.getInt("version", -1);

  if (storedVersion == -1) {
    Serial.println("Configuration uninitialized, using defaults");
    prefs.end();
    return false;
  }

  if (storedVersion != VERSION) {
    Serial.println("Configuration changed, clearing old preferences");
    prefs.end();

    if (prefs.begin(PREFERENCES_NAMESPACE, false)) {  // false = read-write
      prefs.clear();
      prefs.end();
    } else {
      Serial.println("Failed to clear preferences");
    }

    return false;
  }

  // Load WiFi config
  prefs.getString("wifi_ssid", wifi.ssid, sizeof(wifi.ssid));
  prefs.getString("wifi_pass", wifi.password, sizeof(wifi.password));

  // Load Commit Graph config
  prefs.getString("commit_user", commitGraph.username, sizeof(commitGraph.username));
  prefs.getString("commit_url", commitGraph.apiUrl, sizeof(commitGraph.apiUrl));

  // Load Display config
  display.darkMode = prefs.getBool("dark_mode", false);

  // Load Energy config
  energy.syncInterval = prefs.getInt("sync_interval", 4);  // Default 4 hours

  prefs.end();

  Serial.println("Configuration loaded from preferences");
  return true;
}

bool UserConfig::save() {
  if (!prefs.begin(PREFERENCES_NAMESPACE, false)) {  // false = read-write
    Serial.println("Failed to open preferences for writing");
    return false;
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

  // Save version
  prefs.putInt("version", VERSION);

  prefs.end();

  Serial.println("Configuration saved to preferences");
  return true;
}
