#ifndef ClientManager_h
#define ClientManager_h

#include <WiFiS3.h>
#include <vector>

class ClientManager {
private:
   struct Connection;  // Forward declaration of Connection struct

   char* ssid;
   char* password;
   IPAddress ip;
   std::vector<Connection*> connections;
   void boot();

public:
   ClientManager(char* ssid_, char* password_, IPAddress ip_);
   void send(String message, void (*callback_)(String message));
   void listen();
};

#endif