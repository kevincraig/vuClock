#include <Arduino.h>
#include <iostream>
#include <string.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>

#include "dashboard_model.h"
#include "api_server.h"
#include "vuclock.h"
#include "LittleFS.h"
#include "led_controller.h"
#include "clk_wifi.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

ApiServer apiServer;

void ApiServer::StartServer()
{
  Serial.println("Starting WebServer on " + WiFi.localIP().toString());

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.on("/api/ledcolor", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              String r;
              String g;
              String b;
              
              int params = request->params();

              for(int i=0; i<params; i++){
                AsyncWebParameter* p = request->getParam(i);
                Serial.printf("PARAM [%s]: %s\n", p->name().c_str(), p->value().c_str());
              }

              // Get params from request
              r = request->getParam(0)->value();
              g = request->getParam(1)->value();
              b = request->getParam(2)->value();
              
              // convert to uint8_t
              uint8_t rInt = atoi(r.c_str());
              uint8_t gInt = atoi(g.c_str());
              uint8_t bInt = atoi(b.c_str());
              
              LEDController::setLEDColor(rInt, gInt, bInt);
              
              request->send(200, "text/plain", "SUCCESS"); });

  server.on("/api/brightness", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              String level = request->getParam(0)->value();
              uint8_t levelInt = atoi(level.c_str());

              LEDController::setBrightness(levelInt);
              
              request->send(200, "text/plain", "SUCCESS"); });

  server.on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String ssid = CLKWifi::getSSID();
    String ntp = "pool.ntp.org";

    String json = "{\"config\":" "{\"ssid\":\"" + ssid + "\", \"ntp_server\":\"" + ntp + "\" }}";
                 request->send(200, "text/json", json); });

  server.on("/api/time", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
                String time;
                time = VUClock::GetTime();
                Serial.println("getTime");
                Serial.println("Current API Time: " + time);
                request->send(200, "text/json", R"({"time": {"current_time": ")" + time + "\" }}"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404);
  } });

  server.begin();
}

// {
//   "time": {
//     "current_time": "23:23"
//   }
// }