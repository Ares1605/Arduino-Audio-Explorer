#include <WiFiS3.h>
#include "ClientManager.h"
#include <vector>

struct ClientManager::Connection {
   void (*callback)(String message);
   WiFiClient* client;
};

ClientManager::ClientManager(char* ssid_, char* password_, IPAddress ip_) {
    ssid = ssid_;
    ip = ip_;
    password = password_;
    connections = std::vector<Connection*>();
    ClientManager::boot();
}

void ClientManager::send(String message, void (*callback_)(String message)) {
 // Try to connect to the server
 WiFiClient client;
 if (client.connect(ip, 80)) {
   Serial.println("Connected to server");
   
   // Send a message with an incrementing counter
   client.println(message);
   Serial.println("Sent: " + message);
   
   Connection* conn = new Connection;  // Just allocate the struct
   conn->callback = callback_;        // Set fields directly
   conn->client = &client;
   connections.push_back(conn);
 } else {
   Serial.println("Couldn't connect to server");
 }
}
void ClientManager::boot() {
  Serial.begin(9600);
  while (!Serial && millis() < 5000);
  
  // Check for WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // Attempt to connect to WiFi network
  int status = WiFi.begin(ssid, password);
  
  // Wait for connection
  while (status != WL_CONNECTED) {
    Serial.print(".");
    // Try to connect again
    status = WiFi.begin(ssid, password);
    delay(2000);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void ClientManager::listen() {
  for (auto it = connections.begin(); it != connections.end();) {
    Connection* connection = *it;
    if (!connection) {
        it = connections.erase(it);
        continue;
    }

    WiFiClient* client = connection->client;
    if (!client) {
        delete connection;
        it = connections.erase(it);
        continue;
    }

    // Check if client is actually connected first
    if (!client->connected()) {
        Serial.println("Client disconnected");
        client->stop();
        delete connection;
        it = connections.erase(it);
        continue;
    }

    // Only check for data if we're still connected
    if (client->available()) {
        String response = client->readStringUntil('\n');
        Serial.println("Server response: " + response);
        
        if (connection->callback) {
            // Add debug print before callback
            Serial.println("Executing callback");
            connection->callback(response);
            Serial.println("Callback completed");
        }
        
        // Don't immediately disconnect after getting data
        ++it;
    } else {
        // No data available, keep connection alive
        ++it;
    }
  }
}
