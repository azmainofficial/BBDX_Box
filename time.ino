#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // IF YOUR ADDRESS IS DIFFERENT, PUT THAT IN PLACE OF '0x3F'
const char *ssid = "Proton";         // PUT YOUR WIFI NAME HERE
const char *password = "Tasko009";   // PUT YOUR WIFI PASSWORD HERE
const int buzzer = D5; // GPIO2
int t = 0, flag = 0;
WiFiUDP ntpUDP;
#define offset 21600  //UTC -1 = -3600,UTC +1 = 3600, UTC +2 = 7200 (FOR EVERY +1 HOUR ADD 3600,SUBTRACT 3600 FOR -1 HOUR)
NTPClient timeClient(ntpUDP, "pool.ntp.org");
void setup() {
    pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  WiFi.begin(ssid, password);
  timeClient.begin();
  timeClient.setTimeOffset(offset);
  lcd.init();
  lcd.backlight();
  lcd.blink_on();
  lcd.clear();
  Serial.begin(9600);
}
void loop() {
  int hour, minute, second;
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DATE:" + currentDate);
  lcd.setCursor(0, 1);
  lcd.print("TIME:" + formattedTime);
  lcd.setCursor(15, 1);
  sscanf(formattedTime.c_str(), "%d:%d:%d", &hour, &minute, &second);
  if (hour == 4 && minute == 8 ) {
    Serial.println("Time matches 03:30!");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    flag = 1;
  }
  if (flag == 1){
    flag = 0;
  }
  digitalWrite(buzzer, LOW);
  delay(500);
}
