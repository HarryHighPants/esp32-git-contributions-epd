#pragma once
#include <GxEPD2_4G_4G.h>

#define WEEKS 17
#define BUTTON_PIN 39
#define BAT_TEST_PIN 35
// 4 color greyscale
#define GxEPD2_DISPLAY_CLASS GxEPD2_4G_4G
// GDEY0213B74 122x250, SSD1680, (FPC-A002 20.04.08)
#define GxEPD2_DRIVER_CLASS GxEPD2_213_GDEY0213B74
#define EPD_PIN_CS 5
#define EPD_PIN_DC 17
#define EPD_PIN_RST 16
#define EPD_PIN_BUSY 4

// TODO: Add additional device types
// // 4 color greyscale
// #define GxEPD2_DISPLAY_CLASS GxEPD2_4G_4G
// // DEPG0266BN 152x296, SSD1680, (FPC-A002 20.04.08)
// #define GxEPD2_DRIVER_CLASS GxEPD2_266_DEPG0266BN
