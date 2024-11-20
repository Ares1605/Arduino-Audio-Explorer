#include <SoftwareSerial.h>

#define SPEAKER_PIN 9
#define TF_CARD_PIN 10

SoftwareSerial bt(10, 11); // RX, TX for Bluetooth

int currentVolume = 128; // Default volume (50%)
bool isPlaying = false;

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  bt.begin(9600); // Bluetooth communication
  Serial.begin(9600); // Debugging
}

void loop() {
  if (bt.available()) {
    String command = bt.readStringUntil('\n'); // Read command
    handleCommand(command);
  }

  if (isPlaying) {
    playAudio(); // Simulated audio playback
  }
}

void handleCommand(String command) {
  if (command == "PLAY") {
    isPlaying = true;
    Serial.println("PLAY");
  } else if (command == "PAUSE") {
    isPlaying = false;
    Serial.println("PAUSE");
  } else if (command == "NEXT") {
    Serial.println("NEXT SONG");
    bt.println("SONG:Next Song"); // Notify LCD Arduino
  } else if (command == "PREV") {
    Serial.println("PREVIOUS SONG");
    bt.println("SONG:Previous Song"); // Notify LCD Arduino
  } else if (command.startsWith("VOL:")) {
    currentVolume = command.substring(4).toInt();
    Serial.print("VOLUME SET TO: ");
    Serial.println(currentVolume);
  }
}

void playAudio() {
  // Placeholder function to simulate audio playback
  analogWrite(SPEAKER_PIN, currentVolume);
}

