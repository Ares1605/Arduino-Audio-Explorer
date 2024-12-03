#include <WiFiS3.h>
#include "ServerManager.h"
#include "OLEDLogger.h"

ServerManager::ServerManager(char* ssid_, char* password_) {
    ssid = ssid_;
    password = password_;
    logger = new OLEDLogger("Server");
    wifiServer = new WiFiServer(80);  // Create WiFiServer with different name
}

void ServerManager::run(Router* router) {
    if (WiFi.status() == WL_NO_MODULE) {
        logger->error("No WiFi module found");
    }
    int status = WiFi.beginAP(ssid, password);
}
