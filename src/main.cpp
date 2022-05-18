#include <Arduino.h>
/**
 * @file player-url-i2s.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-player/player-url-i2s/README.md
 */
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioConfigLocal.h"

#include <Ticker.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "esp_adc_cal.h"
#include <FS.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <HTTPClient.h>
#include "esp_wifi.h"
#include "WiFiManager.h" //https://github.com/tzapu/WiFiManager

#include "global.h"
#include "ota.h"
#include "lcd.h"

const char *urls[] = {
    "http://media3.mcr.iol.pt/livefm/comercial.mp3/icecast.audio",
    "http://stream.live.vc.bbcmedia.co.uk/bbc_radio_fourfm",
    "http://stream.live.vc.bbcmedia.co.uk/bbc_radio_one",
};
//  const char *wifi = "shed";
//  const char *password = "elephantseat";

// ICYStream urlStream(wifi, password);
ICYStream urlStream;
// URLStream urlStream;
AudioSourceURL source(urlStream, urls, "audio/mp3");
// AudioSourceIcyUrl source(urlStream, urls, "audio/mp3");

I2SStream i2s;
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
char string[16];
TelnetSpy debug;

#define BUTTON_1 35
#define BUTTON_2 0
#define UPDATE_TASK_INTERVAL 1000

// Print Audio Metadata
void printMetaData(MetaDataType type, const char *str, int len)
{

  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);

  // TFT_printLine(toStr(type));

  switch (type)
  {
  case MetaDataType::Name:
    TFT_printLineAt(str, 1);
    break;
  case MetaDataType::Description:
    TFT_printLineAt(str, 2);
    break;
  case MetaDataType::Genre:
    TFT_printLineAt(str, 3);
    break;
  case MetaDataType::Title:
    TFT_printLineAt(str, 4);
    break;
  default:
    break;
  }
}

// Moves to the next url when we touch the pin
void updatePosition()
{
  if (!digitalRead(BUTTON_1))
  {
    Serial.println("Moving to next url");
    player.next();
  }
}

void setupWifi()
{

  byte mac[6];
  WiFi.macAddress(mac);
  char buf[30];
  sprintf(buf, "%s_%02X%02X", HOSTNAME, mac[4], mac[5]);
  Serial.println(buf);
  TFT_printLine(buf);

  WiFi.hostname(buf);
  WiFi.begin();
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect(HOSTNAME);

  if (WiFi.status() != WL_CONNECTED)
  {
    ESP.restart();
  }

  esp_wifi_set_ps(WIFI_PS_NONE);
}

// Arduino setup
void setup()
{
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);
  printf("Audio Buffer:%d", URL_STREAM_BUFFER_COUNT);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // setup telnet debug
  debug.setStoreOffline(true);
  debug.begin(115200);
  debug.println();

  tft.init();
  tft.setRotation(1);

  if (ENABLE_WIFI)
  {
    // wifi
    setupWifi();
    TFT_printLine("Wifi OK");
    TFT_printLine(WiFi.localIP().toString());

    // over the air updates
    setupOTA();
    TFT_printLine("OTA OK");
  }

  // display CPU speed
  uint32_t f = getCpuFrequencyMhz();
  sprintf(string, "CPU Freq: %i", f);
  TFT_printLine(string);
  // setup output
  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.pin_bck = 13;
  i2s.begin(cfg);

  // setup player
  player.setMetadataCallback(printMetaData);

  delay(2000);
  TFT_clear();

  player.begin();
}

// Arduino loop
void loop()
{
  static long nextCheck;
  static int loopCount;
  player.copy();

  if (WiFi.isConnected())
  {
    debug.handle();
    ArduinoOTA.handle();
  }

  if (millis() > nextCheck)
  {
    loopCount++;
    updatePosition();
    //Serial.println(ESP.getMaxAllocHeap() / 1024);
    player.setVolume(0.5);
    nextCheck = millis() + UPDATE_TASK_INTERVAL;
    //sprintf(string, "C %d", loopCount);
    //TFT_printLineAt(string, 0);
  }
}