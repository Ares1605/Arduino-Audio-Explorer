#ifndef OLED_LOGGER_H
#define OLED_LOGGER_H
#include <Arduino.h>
#include <IPAddress.h>
#include <Adafruit_SSD1306.h>

class OLEDLogger {
public:
    OLEDLogger(String header_);
    static Adafruit_SSD1306* display;
    static bool initialize();
    static const int MAX_LINE = 5;
    static char* buffer[MAX_LINE];

    // Variadic template methods for info and error messages
    template<typename... Args>
    void info(const Args&... args) {
        infoImpl(concatenateArgs(args...));
    }
    template<typename... Args>
    void error(const Args&... args) {
        errorImpl(concatenateArgs(args...));
    }
private:
    String header;
    
    void displayBuffer();
    void shiftBuffer();
    void infoImpl(const String& message);
    void errorImpl(const String& message);
    // Base case for template recursion
    String concatenateArgs() {
        return "";
    }
    // Single argument specialization
    template<typename T>
    String concatenateArgs(const T& value) {
        return convertToString(value);
    }
    // Recursive variadic template for multiple arguments
    template<typename T, typename... Args>
    String concatenateArgs(const T& first, const Args&... rest) {
        return convertToString(first) + concatenateArgs(rest...);
    }
    // Type conversion specializations
    template<typename T>
    String convertToString(const T& value) {
        return String(value);
    }
    String convertToString(const char* value) {
        return String(value);
    }
    String convertToString(const String& value) {
        return value;
    }
    String convertToString(const IPAddress& ip) {
        return ip.toString();
    }
    String convertToString(const bool& value) {
        return value ? "true" : "false";
    }
    String convertToString(const float& value) {
        return String(value, 2);
    }
    String convertToString(const double& value) {
        return String(value, 2);
    }
};

#endif
