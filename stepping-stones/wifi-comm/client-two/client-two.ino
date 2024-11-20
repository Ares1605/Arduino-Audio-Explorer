#include <WiFiS3.h>

const char* ssid = "ArduinoR4AP";        // Must match AP's SSID
const char* password = "password123";     // Must match AP's password

WiFiClient client;
IPAddress server_ip(192,168,4,1);  // Default AP IP address
int counter = 0;

void setup() {
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

void loop() {
  // Try to connect to the server
  if (client.connect(server_ip, 80)) {
    Serial.println("Connected to server");
    
    // Send a message with an incrementing counter
    String message = "Hello from client: " + String(counter++);
    client.println(message);
    Serial.println("Sent: " + message);
    
    // Wait for response
    unsigned long timeout = millis();
    while (client.connected() && !client.available()) {
      if (millis() - timeout > 5000) {
        Serial.println("Response timeout!");
        break;
      }
    }
    
    // Read response
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println("Server response: " + response);
    }
    
    client.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Couldn't connect to server");
  }
  
  delay(5000);  // Wait 5 seconds before next message
}
