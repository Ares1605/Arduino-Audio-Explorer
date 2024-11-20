// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// //
// // Pin definitions for buttons
// const int buttonPin1 = 4;  // Button 1
// const int buttonPin2 = 8;  // Button 2
// const int buttonPin3 = 13; // Button 3

// // Variables to store button states
// int buttonState1 = 0;
// int buttonState2 = 0;
// int buttonState3 = 0;

// // Initialize the LCD (I2C address may vary; commonly 0x3F or 0x27)
// LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if necessary

// void setup() {
//   // Start the serial monitor
//   Serial.begin(9600);

//   // Set button pins as input
//   pinMode(buttonPin1, INPUT);
//   pinMode(buttonPin2, INPUT);
//   pinMode(buttonPin3, INPUT);

//   // Initialize the LCD
//   lcd.begin(16, 2);
//   lcd.backlight();  // Turn on the backlight
//   lcd.setCursor(0, 0);
//   lcd.print("Testing Buttons");
//   delay(2000);  // Show message for 2 seconds
// }

// void loop() {
//   // Read the state of each button
//   buttonState1 = digitalRead(buttonPin1);
//   buttonState2 = digitalRead(buttonPin2);
//   buttonState3 = digitalRead(buttonPin3);

//   // Clear the LCD to update
//   lcd.clear();

//   // Check the state of Button 1
//   if (buttonState1 == HIGH) {
//     lcd.setCursor(0, 0);
//     lcd.print("Button 1: Pressed");
//   } else {
//     lcd.setCursor(0, 0);
//     lcd.print("Button 1: Not Pressed");
//   }

//   // Check the state of Button 2
//   if (buttonState2 == HIGH) {
//     lcd.setCursor(0, 1);
//     lcd.print("Button 2: Pressed");
//   } else {
//     lcd.setCursor(0, 1);
//     lcd.print("Button 2: Not Pressed");
//   }

//   // Check the state of Button 3
//   if (buttonState3 == HIGH) {
//     lcd.setCursor(0, 1);
//     lcd.print("Button 3: Pressed");
//   } else {
//     lcd.setCursor(0, 1);
//     lcd.print("Button 3: Not Pressed");
//   }

//   delay(200);  // Small delay to avoid screen flicker
// }

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

// OLED display setup (using SSD1306)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// LCD display setup (using LiquidCrystal_I2C)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 is the default I2C address for many LCDs

// LED pin
const int ledPin = 11;  // LED connected to pin 11

void setup() {
  // Start the serial monitor for debugging
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C is the I2C address for the OLED
    Serial.println(F("OLED initialization failed!"));
    for (;;);  // Halt if OLED initialization fails
  }
  display.clearDisplay();         // Clear OLED buffer
  display.setTextSize(1);         // Set text size for OLED
  display.setTextColor(WHITE);    // Set text color for OLED
  display.setCursor(0, 0);        // Set text position on OLED
  display.println(F("Hello, World!"));
  display.display();              // Display the text on OLED

  // Initialize LCD display
  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on the backlight for the LCD
  lcd.setCursor(0, 0);            // Set text position on LCD
  lcd.print("Hello, World!");     // Display the text on LCD

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);        // Set pin 11 as output
  digitalWrite(ledPin, LOW);      // Ensure the LED is initially off
}

void loop() {
  // Check if data is available in the serial monitor
  if (Serial.available() > 0) {
    String input = Serial.readString();  // Read the incoming serial data
    input.trim();  // Remove any leading or trailing spaces or newline characters

    // Check if the input is "pause" or "unpause"
    if (input.equalsIgnoreCase("pause")) {
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      Serial.println("LED is ON (Pause)");
    } 
    else if (input.equalsIgnoreCase("unpause")) {
      digitalWrite(ledPin, LOW);   // Turn off the LED
      Serial.println("LED is OFF (Unpause)");
    } 
    else {
      Serial.println("Invalid command. Please type 'pause' or 'unpause'.");
    }
  }
}













