#include "Config.h"
#include <Arduino.h>
#include <TM1637Display.h>
#include <WiFi.h>
#include <time.h>

// 4-Digit Display
#define CLK 1
#define DIO 0
TM1637Display display(CLK, DIO);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  display.showNumberDecEx(timeinfo.tm_hour * 100 + timeinfo.tm_min,
                          (0x80 | (timeinfo.tm_sec % 2) * 0x40), true);
}

void setup()

{

  Serial.begin(115200);

  Serial.println("Hello from setup!");

  display.setBrightness(0x0f);

  // connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  // disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

unsigned long lastSyncTime = 0;
const unsigned long syncInterval = 3600000; // 1 hour in milliseconds

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastSyncTime > syncInterval) {
    lastSyncTime = currentTime;

    // Reconnect to WiFi
    Serial.printf("Reconnecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");

    // Re-sync the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();

    // Disconnect WiFi
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  delay(1000);
  printLocalTime();
}
