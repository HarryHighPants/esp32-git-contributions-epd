#pragma once
#include <GxEPD2_4G_4G.h>

#ifdef LILYGO213
#define WEEKS 17
#define BUTTON_PIN 39
#define BAT_TEST_PIN 35
#define GxEPD2_DISPLAY_CLASS GxEPD2_4G_4G
#define GxEPD2_DRIVER_CLASS GxEPD2_213_GDEY0213B74
#define EPD_PIN_CS 5
#define EPD_PIN_DC 17
#define EPD_PIN_RST 16
#define EPD_PIN_BUSY 4
#endif

#ifdef LILYGO266
#include "GxEPD2_266_DEPG0266BN.h"
#define WEEKS 19
#define BUTTON_PIN 39
#define BAT_TEST_PIN 35
#define GxEPD2_DISPLAY_CLASS GxEPD2_4G_4G
#define GxEPD2_DRIVER_CLASS GxEPD2_266_DEPG0266BN
#define EPD_PIN_CS 5
#define EPD_PIN_DC 19
#define EPD_PIN_RST 4
#define EPD_PIN_BUSY 34
#endif

// Throw if weeks not defined (no device set)
#ifndef WEEKS
#error "WEEKS not defined, please set a device type"
#endif
