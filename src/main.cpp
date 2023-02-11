#include <Arduino.h>
#include <iostream>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "clk_wifi.h"
#include "wifi_config.h"
#include "api_server.h"
#include "vuclock.h"
#include "LittleFS.h"
#include "led_controller.h"

ApiServer api;

#define PIN_METER_HOUR 5
#define PIN_METER_MINS 4

#define MINIMUM_PWM_VALUE_HOUR 18
#define MAXIMUM_PWM_VALUE_HOUR 240
#define MINIMUM_PWM_VALUE_MIN 1
#define MAXIMUM_PWM_VALUE_MIN 221
#define MINIMUM_PWM_VALUE_SEC 5
#define MAXIMUM_PWM_VALUE_SEC 235

int period = 60000;
unsigned long timeNow = 0;

void runClock()
{
  timeNow = millis();
  VUClock::UpdateTime();
  long hour = VUClock::GetTimeComponents().hour;
  long mins = VUClock::GetTimeComponents().mins;
  analogWrite(PIN_METER_HOUR, map(hour, 1, 12, MINIMUM_PWM_VALUE_HOUR, MAXIMUM_PWM_VALUE_HOUR));
  analogWrite(PIN_METER_MINS, map(mins, 0, 60, MINIMUM_PWM_VALUE_MIN, MAXIMUM_PWM_VALUE_MIN));
}

void initializeClock()
{
  LEDController::initLEDController();
}

void setup()
{

  Serial.begin(115200);
  MDNS.begin("vuclock-041977");

  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  // pinMode(2, OUTPUT);
  // digitalWrite(2, LOW);
  CLKWifi::Start();

  VUClock::SetUtcOffSet(-5);
  ApiServer::StartServer();
  MDNS.addService("http", "tcp", 80);

  initializeClock();

  timeNow = millis();
  runClock();
  CLKWifi::scanNetworks();
}

void loop()
{

  MDNS.update();

  // update clock every 60 sec
  if (millis() > timeNow + period)
  {
    runClock();
  }
}
