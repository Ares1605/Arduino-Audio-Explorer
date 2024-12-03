#include "ServerManager.h"
#include "Router.h"
#include "OLEDLogger.h"

ServerManager* server;
Router* router;


void setup() {
  OLEDLogger::initialize();

  Serial.begin(9600);
  delay(3000);
  router = new Router();

  server = new ServerManager("test", "password", router);
}
void loop() {
  server->listen();
  delay(300);
}
