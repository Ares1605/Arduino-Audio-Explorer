#ifndef OLED_LOGGER_H
#define OLED_LOGGER_H

#include <Arduino.h>

class OLEDLogger {
public:
    OLEDLogger(String header_);
    void info(String message);
    void error(String message);
    
private:
    String header;
};

#endif
