#include "ClientManager.h"

ClientManager* client;
int counter = 0;

void handleResponse(String message) {
  Serial.println("callback triggered");
  String response = String(counter++);
  client->send(response, handleResponse);
}
void setup() {
  Serial.begin(9600);
  delay(3000);
  IPAddress serverIP(192,168,4,1);
  client = new ClientManager("test", "password", serverIP);

  String message = "Hello" + String(counter);
  client->send(message, handleResponse);
}
void loop() {
  delay(500);
  client->listen();
}
