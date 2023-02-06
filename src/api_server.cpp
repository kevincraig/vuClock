#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>

#include "dashboard_model.h"
#include "api_server.h"
#include "vuclock.h"
#include "LittleFS.h"
#include "main.h"

// String GetTime();

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

ApiServer apiServer;

void ApiServer::StartServer()
{
    Serial.println("Starting WebServer on " + WiFi.localIP().toString());

    // API ROUTES
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "index.html"); });

    server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "css/styles.css", "text/css"); });

    server.on("/img/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "img/favicon.ico", "image/x-icon"); });

    server.on("/api/ledcolor", HTTP_POST, [](AsyncWebServerRequest *request)
              { 
                String r;
                String g;
                String b;

                r = request->getParam("r")->value();
                g = request->getParam("g")->value();
                b = request->getParam("b")->value();
                Main::setLedColor(r,g,b); request->send(200); 
                request->send(200, "text/plain", "SUCCESS"); });

    server.on("/api/time", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                String time;
                time = VUClock::GetTime();
                Serial.println("getTime");
                Serial.println("Current API Time: " + time);
                request->send(200, "text/json", R"({"time": {"current_time": ")" + time + "\" }}"); });

    server.begin();
}