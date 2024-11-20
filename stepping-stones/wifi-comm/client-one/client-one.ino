#include <WiFiS3.h>

const char* ap_ssid = "ArduinoR4AP";      
const char* ap_password = "password123";   

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 5000);
  
  Serial.println("Configuring access point...");
  
  // Check for WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  
  // Create Access Point
  int status = WiFi.beginAP(ap_ssid, ap_password);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }
  
  // Wait 10 seconds for connection:
  delay(10000);
  
  server.begin();
  
  IPAddress ip = WiFi.localIP();
  Serial.print("AP IP Address: ");
  Serial.println(ip);
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
}

void loop() {
  // Check if number of clients changed
  if (WiFi.status() != WL_AP_LISTENING) {
    // Print connection status
    Serial.print("Status: ");
    Serial.println(WiFi.status());
  }
  
  // Handle any incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String message = client.readStringUntil('\n');
        Serial.println("Received: " + message);
        
        // Echo back to client
        client.println("Server received: " + message);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  
  delay(1000);
}
