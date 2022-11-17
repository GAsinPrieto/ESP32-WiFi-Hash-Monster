#include <M5StickC.h>
#include <WiFi.h>
#include "time.h"

struct tm timeInfo;

//WIFI Credentials
const char* ssid     = "SSID";
const char* password = "PASSWORD";

const char* ntp_server = "ntp.i2t.ehu.es";
const long gmt_offset     = 3600; //in seconds
const int daylightsaving = 3600; //offset in seconds

void checkServer() {
  if (!getLocalTime(&timeInfo)) {
    M5.Lcd.println("Server unreachable");
    while(1);
  }
}


void printTime() {
  if (!getLocalTime(&timeInfo)) {
    M5.Lcd.println("Server unreachable");
    return;
  }
  M5.Lcd.println(&timeInfo, "%A, %B %d \n%Y %H:%M:%S");
}

void setTime(struct tm timeInfo){
  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = timeInfo.tm_hour;
  TimeStruct.Minutes = timeInfo.tm_min;
  TimeStruct.Seconds = timeInfo.tm_sec;
  M5.Rtc.SetTime(&TimeStruct);

  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = timeInfo.tm_wday;
  DateStruct.Month = timeInfo.tm_mon + 1;
  DateStruct.Date = timeInfo.tm_mday;
  DateStruct.Year = timeInfo.tm_year + 1900;
  M5.Rtc.SetData(&DateStruct);
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.printf("\nConnecting to %s \n", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
        delay(1000);
        M5.Lcd.print("-");
    }
    M5.Lcd.println("Successful connection");
    configTime(gmt_offset, daylightsaving,ntp_server);
    checkServer();
    setTime(timeInfo);
    WiFi.disconnect(true);
    delay(20);
}

void loop() {
    M5.Lcd.setCursor(0, 50);
    printTime();
    delay(1000);
}
