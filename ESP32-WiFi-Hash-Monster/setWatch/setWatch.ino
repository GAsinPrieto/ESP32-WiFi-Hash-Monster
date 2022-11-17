#include <M5StickC.h>
#include <WiFi.h>

#include "time.h"

// Set the name and password of the wifi to be connected.
const char* ssid     = "maiher";
const char* password = "MH-08076298";

const char* ntpServer =
    "time1.aliyun.com";  // Set the connect NTP server.
const long gmtOffset_sec     = 3600;
const int daylightOffset_sec = 3600;

struct tm timeinfo;

void printLocalTime() {  // Output current time.
    if (!getLocalTime(&timeinfo)) {  // Return 1 when the time is successfully obtained.
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    M5.Lcd.println(&timeinfo, "%A, %B %d \n%Y %H:%M:%S");  // Screen prints date and time.   
}

void setTime(struct tm timeinfo){
  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = timeinfo.tm_hour;
  TimeStruct.Minutes = timeinfo.tm_min;
  TimeStruct.Seconds = timeinfo.tm_sec;
  M5.Rtc.SetTime(&TimeStruct);

  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = timeinfo.tm_wday;
  DateStruct.Month = timeinfo.tm_mon + 1;
  DateStruct.Date = timeinfo.tm_mday;
  DateStruct.Year = timeinfo.tm_year + 1900;
  M5.Rtc.SetData(&DateStruct);
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.printf("\nConnecting to %s", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.println("\nCONNECTED!");
    configTime(gmtOffset_sec, daylightOffset_sec,ntpServer);  // init and get the time.
    printLocalTime();
    setTime(timeinfo);
    WiFi.disconnect(true);  // Disconnect wifi.
    WiFi.mode(WIFI_OFF);  // Set the wifi mode to off
    delay(20);
}

void loop() {
    delay(1000);
    M5.Lcd.setCursor(0, 25);
    printLocalTime();
}
