#include "OLEDLogger.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

OLEDLogger::OLEDLogger(String header_) : header(header_) {
    header = header_;
}

bool OLEDLogger::initialize() {
    if (!display) {
        display = new Adafruit_SSD1306(128, 32, &Wire, -1); 
    }
    
    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        return false;
    }

    return true;
}

void OLEDLogger::displayBuffer() {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  Serial.println("PRINTING");
  for (int i = 0; i < OLEDLogger::MAX_LINE; i++) {
    display->setCursor(0, i*8);
    display->print(OLEDLogger::buffer[i]);
    Serial.println(OLEDLogger::buffer[i]);
  }
  Serial.println("OVER");
  display->display();
}
void OLEDLogger::shiftBuffer() {
    for (int i = OLEDLogger::MAX_LINE - 1; i > 0; i--) {
        OLEDLogger::buffer[i] = OLEDLogger::buffer[i - 1];
    }
    OLEDLogger::buffer[0] = "";
}

void OLEDLogger::infoImpl(const String& message) {;
    char line[128];  // Make sure this is large enough for your needs
    // [I][header] [message]]
    snprintf(line, sizeof(line), "I|%s|%s", header.c_str(), message.c_str());

    OLEDLogger::shiftBuffer();
    OLEDLogger::buffer[0] = strdup(line); // use strdup to copy the data. line variable is pass by reference
    OLEDLogger::displayBuffer();
}

void OLEDLogger::errorImpl(const String& message) {
    // // Implement your OLED display logic here
    // Serial.print("[E][");
    // Serial.print(header);
    // Serial.print("] ");
    // Serial.println(message);
}

Adafruit_SSD1306* OLEDLogger::display = nullptr;
char* OLEDLogger::buffer[MAX_LINE] = {""}; // initialize the buffer array