#include "ClientManager.h"

ClientManager* client;
int counter = 0;

void setup() {
  Serial.begin(9600);
  delay(3000);
  IPAddress serverIP(192,168,4,1);
  client = new ClientManager("test", "password", serverIP);
}
void loop() {
  String message = "Hello from client: " + String(counter++);
  client->send(message);
  delay(1000);
}
