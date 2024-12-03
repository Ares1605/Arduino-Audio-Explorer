#include "ServerManager.h"
#include "OLEDLogger.h"

ServerManager* server;

void setup() {
  OLEDLogger::initialize();

  Serial.begin(9600);
  delay(3000);

  server = new ServerManager("test", "password");
}
void loop() {
  server->listen();
  delay(300);
}
