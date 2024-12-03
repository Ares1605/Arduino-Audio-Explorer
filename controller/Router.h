#ifndef ROUTER_H
#define ROUTER_H

#include <Arduino.h>

class Router {
public:
    void bind(String endpoint_, void (*callback_)(String));
    void recieve();
    
private:
    String endpoint;
    void (*callback)(String);
};

#endif
