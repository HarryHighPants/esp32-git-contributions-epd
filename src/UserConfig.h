#pragma once
struct UserConfig {
  char username[50] = "";
  char wifiSSID[50] = "";
  char wifiPassword[50] = "";
  int syncInterval = 4;  // Hours
  bool darkMode = false;
  char apiUrl[100] = "https://contributions-api.harryab.com/";

  bool isSet() const;
};
