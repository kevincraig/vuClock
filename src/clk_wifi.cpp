#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "clk_wifi.h"
#include "wifi_config.h"
#include "WiFiManager.h"

WiFiManager wifiManager;

void CLKWifi::Start()
{

    wifiManager.autoConnect("vuClock", "vuClock");

    Serial.println("Connected to: " + String(WiFi.SSID()));
    Serial.println("WiFi Connected");
    Serial.println("IP Address (DHCP): " + WiFi.localIP().toString());
}

String CLKWifi::getSSID()
{
    return WiFi.SSID();
}

void CLKWifi::scanNetworks()
{
}
