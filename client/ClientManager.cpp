#include <WiFiS3.h>
#include "ClientManager.h"

ClientManager::ClientManager(char* ssid_, char* password_, IPAddress ip_) {
    ssid = ssid_;
    ip = ip_;
    password = password_;
    ClientManager::boot();
}
void ClientManager::send(String message) {
  // Try to connect to the server
  WiFiClient client;
  if (client.connect(ip, 80)) {
    Serial.println("Connected to server");
    
    // Send a message with an incrementing counter
    client.println(message);
    Serial.println("Sent: " + message);
    
    // // Wait for response
    // unsigned long timeout = millis();
    // while (client.connected() && !client.available()) {
    //   if (millis() - timeout > 5000) {
    //     Serial.println("Response timeout!");
    //     break;
    //   }
    // }
    // 
    // // Read response
    // while (client.available()) {
    //   String response = client.readStringUntil('\n');
    //   Serial.println("Server response: " + response);
    // }
    
    client.stop();
    Serial.println("Disconnected from server");
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

