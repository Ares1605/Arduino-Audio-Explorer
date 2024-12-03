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
  // Create WiFiClient on heap
  WiFiClient* client = new WiFiClient();
  Serial.println("TRIGGERRR");
  
  if (client->connect(ip, 80)) {
    while(client->available()) {
        client->read();  // Clear any existing data
    }
    Serial.println("Connected to server");
    
    client->println(message);
    delay(50);
    Serial.println("Sent: " + message);
    
    Connection* conn = new Connection;
    conn->callback = callback_;
    conn->client = client;  // Store the heap-allocated client
    connections.push_back(conn);
  } else {
    Serial.println("Couldn't connect to server");
    delete client;  // Clean up if connection failed
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
  Serial.print("conn size:");
  Serial.println(connections.size());
  for (int i = 0; i < connections.size(); i++) {
    Connection* connection = connections[i];
    if (!connection) {
        connections.erase(connections.begin() + i);
        i--;  // Adjust index after removal
        continue;
    }


    WiFiClient* client = connection->client;
    if (!client) {
        delete connection;
        connections.erase(connections.begin() + i);
        i--;  // Adjust index after removal
        continue;
    }

    // Check if client is actually connected first
    if (!client->connected()) {
        Serial.println("Client disconnected");
        client->stop();
        delete client;          // Delete the WiFiClient first
        delete connection;      // Then delete the Connection
        connections.erase(connections.begin() + i);
        i--;
        continue;
    }

    // Only check for data if we're still connected
    if (client->available()) {
        String response = client->readStringUntil('\n');
        Serial.println("Server response: " + response);
        
        if (connection->callback) {
            Serial.println("Executing callback");
            connection->callback(response);
            Serial.println("Callback completed");
        }

        client->stop();
        delete client;          // Delete the WiFiClient first
        delete connection;      // Then delete the Connection
        connections.erase(connections.begin() + i);
        i--;
        continue;
    }
  }
}