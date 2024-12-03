#include <WiFiS3.h>
#include "ServerManager.h"
#include "OLEDLogger.h"

ServerManager::ServerManager(char* ssid_, char* password_) {
    ssid = ssid_;
    password = password_;
    logger = new OLEDLogger("S");
    ServerManager::boot();
}
void ServerManager::listen() {
  WiFiClient client;
  
  while (client = server->available()) {
    Serial.println("\n\nserver available!");
    
    // Read any available data immediately
    if (client.available()) {
      Serial.println("client available");
      String message = client.readStringUntil('\n');
      logger->info(message);
      
      client.println(message);
      client.stop();
    } else {
      Serial.println("client unavailable");
    }
  }
}
void ServerManager::boot() {
    server = new WiFiServer(80);
    if (WiFi.status() == WL_NO_MODULE) {
        logger->error("No WiFi module found");
    }
    int status = WiFi.beginAP(ssid, password);

    if (status != WL_AP_LISTENING) {
        logger->error("Creating access point failed");
        while (true);
    }
    
    server->begin();
    
    IPAddress ip = WiFi.localIP();
    logger->info("IP: " + ip.toString());
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "SSID: %s", ssid);
    logger->info(buffer);
}
