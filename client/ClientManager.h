#ifndef SERVER_H
#define SERVER_H

#include <WiFiS3.h>

class ClientManager {
public: 
    ClientManager(char* ssid_, char* password_, IPAddress ip_);
    void send(String message);

private:
    char* ssid;
    char* password;
    IPAddress ip;

    void boot();
};

#endif


