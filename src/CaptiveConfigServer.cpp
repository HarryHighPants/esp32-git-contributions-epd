#include "CaptiveConfigServer.h"

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

#include "captivePortalHtml/ConfigHtml.generated.h"
#include "captivePortalHtml/ConfigSavedHtml.generated.h"

const auto configSSID = "Contributions Screen";
constexpr unsigned long configModeTimeout = 20 * 60 * 1000;  // 20 minutes
DNSServer dnsServer;
WebServer server(80);

String buildConfigHtml(const UserConfig* config) {
  auto html = String(ConfigHtml);
  html.replace("{{wifi-ssid}}", config->wifi.ssid);
  html.replace("{{wifi-password}}", config->wifi.password);
  html.replace("{{username}}", config->commitGraph.username);
  html.replace("{{url}}", config->commitGraph.apiUrl);
  html.replace("{{interval}}", String(config->energy.syncInterval));
  html.replace("{{dark-mode}}", config->display.darkMode ? "checked" : "");
  return html;
}

CaptiveConfigServer::CaptiveConfigServer(UserConfig* config) {
  this->config = config;
  this->loaded = false;
}

void CaptiveConfigServer::begin() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(configSSID);

  this->configPageHtml = buildConfigHtml(this->config);

  server.on("/portal", [this] { server.send(200, "text/html", this->configPageHtml); });

  server.on("/submit", [this] {
    // Read the form values from the param and store them in RTC memory
    strcpy(this->config->wifi.ssid, server.arg("wifi-ssid").c_str());
    strcpy(this->config->wifi.password, server.arg("wifi-password").c_str());
    strcpy(this->config->commitGraph.username, server.arg("username").c_str());
    strcpy(this->config->commitGraph.apiUrl, server.arg("url").c_str());
    this->config->energy.syncInterval = server.arg("interval").toInt();
    this->config->display.darkMode = server.hasArg("dark-mode");
    this->loaded = true;

    Serial.println("Recieved updated config");

    server.send(200, "text/html", ConfigSavedHtml);
    delay(100);  // Wait for the response to be sent
  });

  server.onNotFound([] {
    server.sendHeader("Location", "/portal");
    server.send(302, "text/plain", "Redirecting to portal");
  });

  this->loaded = false;
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.begin();
}

bool CaptiveConfigServer::getConfig() const {
  const unsigned long configStartMillis = millis();

  while (true) {
    if (loaded) {
      return true;
    }

    if (millis() - configStartMillis > configModeTimeout) {
      Serial.println("Config mode timeout");
      return false;
    }

    dnsServer.processNextRequest();
    server.handleClient();
  }
}

void CaptiveConfigServer::end() {
  server.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
}
