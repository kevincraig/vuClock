#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <Arduino.h>
#include <iostream>

class Dashboard
{
public:
    String ntpServer;
    String timeZone;
    String ssid;
    String ipAddress;
};

#endif