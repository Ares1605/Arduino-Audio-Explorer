#ifndef SERVER_H
#define SERVER_H

#include <WiFiS3.h>
#include "OLEDLogger.h"
#include "Router.h"

class ServerManager {
public: 
    ServerManager(char* ssid_, char* password_, Router* router_);
    void listen();

private:
    char* ssid;
    char* password;

    Router* router;
    OLEDLogger* logger;
    WiFiServer* server;
    void boot();
};

#endif

