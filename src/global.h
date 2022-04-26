#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <Arduino.h>
#include <TelnetSpy.h>
#include <TFT_eSPI.h>
#include <FS.h> // Font files are stored in SPIFFS, so load the library

// intellisense workaround
//  _VOID      _EXFUN(tzset,	(_VOID));
//  int	_EXFUN(setenv,(const char *__string, const char *__value, int __overwrite));

#define FASTLED_ESP8266_D1_PIN_ORDER
#define HOSTNAME "AUDIO01"

#define ENABLE_WIFI 1
#define ADC_EN 14 // ADC_EN is the ADC detection enable port
#define ADC_PIN 34
#define BUTTON_1 35
#define BUTTON_2 0
#define TFT_BACKLIGHT_ON 1
#define INTERRUPT_PIN 26

#define REPORT_PERIOD_SECONDS 60
#define BATTERY_CHECK_INTERVAL_MS 100

#define AA_FONT_SMALL "NotoSansBold15"
#define AA_FONT_LARGE "NotoSansBold36"


extern TelnetSpy debug;
extern TFT_eSPI tft;
extern char string[16];

#endif