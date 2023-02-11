#ifndef CLKWIFI_H
#define CLKWIFI_H

class CLKWifi
{
public:
    static void Start();
    static void scanNetworks();
    static String getSSID();
};

#endif