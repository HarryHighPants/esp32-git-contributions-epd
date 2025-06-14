#pragma once

class UserConfig {
 public:
  struct Wifi {
    char ssid[50] = "";
    char password[50] = "";
    bool isSet() const;
  };

  struct CommitGraph {
    char username[50] = "";
    char apiUrl[100] = "https://contributions-api.harryab.com/";
    bool isSet() const;
  };

  struct Display {
    bool darkMode = false;
  };

  struct Energy {
    int syncInterval = 4;  // Hours
  };

  bool isSet() const;
  bool load();
  bool save();

  Wifi wifi;
  CommitGraph commitGraph;
  Display display;
  Energy energy;

 private:
  static const char *PREFERENCES_NAMESPACE;
  static const int VERSION;
};
