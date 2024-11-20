#include <SoftwareSerial.h>

#define PLAY_PAUSE_BTN 2
#define NEXT_BTN 3
#define PREV_BTN 4
#define POT_PIN A0

SoftwareSerial bt(10, 11); // RX, TX for Bluetooth

void setup() {
  pinMode(PLAY_PAUSE_BTN, INPUT_PULLUP);
  pinMode(NEXT_BTN, INPUT_PULLUP);
  pinMode(PREV_BTN, INPUT_PULLUP);
  bt.begin(9600); // Start Bluetooth communication

  Serial.begin(9600); // Debugging
}

void loop() {
  static bool playPauseState = false;
  static int lastVolume = 0;

  // Handle Play/Pause Button
  if (digitalRead(PLAY_PAUSE_BTN) == LOW) {
    delay(200); // Debounce
    playPauseState = !playPauseState;
    bt.println(playPauseState ? "PLAY" : "PAUSE");
    Serial.println(playPauseState ? "PLAY" : "PAUSE");
  }

  // Handle Next Button
  if (digitalRead(NEXT_BTN) == LOW) {
    delay(200); // Debounce
    bt.println("NEXT");
    Serial.println("NEXT");
  }

  // Handle Previous Button
  if (digitalRead(PREV_BTN) == LOW) {
    delay(200); // Debounce
    bt.println("PREV");
    Serial.println("PREV");
  }

  // Handle Volume
  int volume = analogRead(POT_PIN) / 4; // Map to 0-255 range
  if (volume != lastVolume) {
    lastVolume = volume;
    bt.print("VOL:");
    bt.println(volume);
    Serial.print("VOL:");
    Serial.println(volume);
  }

  delay(50); // Reduce communication frequency
}

