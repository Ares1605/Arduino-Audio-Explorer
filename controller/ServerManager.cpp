#include <WiFiS3.h>
#include "ServerManager.h"
#include "OLEDLogger.h"

ServerManager::ServerManager(char* ssid_, char* password_, Router* router_) {
    ssid = ssid_;
    password = password_;
    router = router_;
    logger = new OLEDLogger("S");
    ServerManager::boot();
}
void ServerManager::listen() {
  WiFiClient client = server->available();
  
  if (client) {
    logger->info("New Client");
    
    // Read any available data immediately
    if (client.available()) {
      String message = client.readStringUntil('\n');
      Serial.println("Received: " + message);
      
      // Echo back to client
      client.println("Server received: " + message);
    }
    
    // If client is done sending data, close connection
    if (!client.connected()) {
      client.stop();
      logger->info("DC Client");
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
