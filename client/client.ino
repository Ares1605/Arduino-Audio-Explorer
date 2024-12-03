#include "ClientManager.h"

ClientManager* client;
int counter = 0;

void handleResponse(String message) {
  String response = message + "R" + String(counter++);
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
  delay(1000);
  Serial.println("pre-crash");
  delay(500);
  client->listen();
}
