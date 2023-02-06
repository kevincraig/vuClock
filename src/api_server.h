#pragma once
#ifndef APISERVER_H
#define APISERVER_H

class ApiServer
{
public:
    void getTime();
    // void getPing();
    // void getDashboard();
    static void StartServer();
    // AsyncWebServer server;
};
#endif