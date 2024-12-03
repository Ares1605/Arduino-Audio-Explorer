#include <SPI.h>
#include <SD.h>

// SD card chip speaker pin
const int chipSelect = 10;
const int buzzer = 4;

// Current song choose an index from 1 - numberSongs
int currentSong = -999999;

const int numberSongs = 9;
const int lineSize = 3;

// Variables to manage melody playback state
int currentNote = 0;          // Current note being played
int notes = 0;                // Total number of notes
int noteDuration = 0;         // Duration of the current note
unsigned long noteStartTime;  // When the current note started
bool isPlayingNote = true;    // Whether we're currently playing a note
static bool initialized = false;



// Stores all of the melodies after eading in from file
String melodies[numberSongs][lineSize];
int count = 0;
void setup() {
  Serial.begin(9600);

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("** SD Card initialization failed! **");
    return;
  }
  Serial.println("** SD Card initialized successfully. **");

  readSongs("songs.txt");
  //  readFile("song1.txt");
  // readFile("ReadMe.txt");
  printMelodies();
}

bool pause = false;

void loop() {

  if (Serial.available() > 0) {

    int incomingByte = Serial.read();

    if (incomingByte != '\n' && incomingByte != '\r') {
      int numericValue = incomingByte - '0';
      if (numericValue > 0 && numericValue <= numberSongs) {  // For selecting a song
        currentSong = numericValue;
        playMelody();                          // Start playing the melody
      } else if ((char)incomingByte == 'x') {  // Pause
        pause = true;
        Serial.println();
        Serial.println("Paused");
      } else if ((char)incomingByte == 'c') {  // Play
        pause = false;
        Serial.println();
        Serial.println("Play");
      } else if ((char)incomingByte == 'o') {  // Next
        Serial.println("Play Next");

        if (currentSong + 1 == numberSongs) {
          currentSong = 1;
        } else {
          currentSong++;
        }

        initialized = false;  // Reset the playback state
        currentNote = 0;
        notes = 0;

        playMelody();
      } else if ((char)incomingByte == 'i') {  // Previous
        Serial.println("Prev");

        if (currentSong - 1 == 0) {
          currentSong = numberSongs - 1;
        } else {
          currentSong--;
        }

        initialized = false;
        currentNote = 0;
        notes = 0;  // Reset the playback state
        playMelody();
      } else {  // Not a valid input
        Serial.println("Not a valid input");
      }
    }
  }

  // Continuously call playMelody to manage non-blocking playback
  if (initialized && !pause) {
    playMelody();
  }
}

void readSongs(const char* filename) {
  // Open the file on the SD card
  File file = SD.open(filename);

  // Check if the file opened successfully
  if (file) {
    Serial.print("** Reading file: ");
    Serial.println(filename);

    int outterArrayIndex = 0;

    // Read file line by line
    while (file.available()) {
      String line = file.readStringUntil('\n');

      int innerArrayIndex = 0;
      while (line.length() > 0) {  // Reads inside of each line for every comma value
        int commaIndex = line.indexOf(',');
        if (commaIndex == -1) {
          // No more commas, print the remaining part
          //  Serial.println("Token: " + line);
          melodies[outterArrayIndex][innerArrayIndex++] = line;
          break;
        }
        // Extract the token and print it
        String token = line.substring(0, commaIndex);
        // Serial.println("Token: " + token);
        melodies[outterArrayIndex][innerArrayIndex] = token;

        innerArrayIndex++;  // increment inner array

        // Remove the token from the line
        line = line.substring(commaIndex + 1);
      }
      outterArrayIndex++;  // increment outter array
      // Serial.println(line);  // Print the line to the Serial Monitor
    }

    // Close the file when done
    file.close();
    Serial.println("** File closed. **");
  } else {
    Serial.print("** Error: Could not open file **");
    Serial.println(filename);
  }
  Serial.println();
}

void printMelodies() {
  Serial.println("Print Melodies Called");
  for (int i = 0; i < numberSongs; i++) {
    Serial.print("Line ");
    Serial.print(i);
    Serial.print(":");
    Serial.print("\t");
    for (int j = 0; j < lineSize; j++) {
      Serial.print(melodies[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
  Serial.println();
}

void readMelody(int*& melody, int& musicSize, int& valuesAdded) {
  String musicFile = melodies[currentSong][2];
  File file = SD.open(musicFile);

  melody = new int[musicSize];


  if (file) {
    Serial.print("** Reading File: ");
    Serial.print(musicFile);
    Serial.println(" **");

    while (file.available()) {
      String line = file.readStringUntil('\n');

      while (line.length() > 0) {  // Reads inside of each line for every comma value
        int commaIndex = line.indexOf(',');
        if (commaIndex == -1) {
          // No more commas, print the remaining part
          // Serial.println("Token: " + line);
          valuesAdded++;
          break;
        }

        if (valuesAdded == musicSize) {
          musicSize = musicSize * 2;
          int* tempArray = new int[musicSize];
          for (int i = 0; i < musicSize / 2; i++) {
            tempArray[i] = melody[i];
          }

          delete[] melody;

          melody = tempArray;
        }


        // Extract the token and print it
        String token = line.substring(0, commaIndex);
        // Serial.println("Token: " + token);

        melody[valuesAdded] = token.toInt();

        // Remove the token from the line
        line = line.substring(commaIndex + 1);

        valuesAdded++;
      }
    }

    //    Serial.print("Values Added in Function: ");
    //    Serial.println(valuesAdded);
    //    Serial.print("Array Size in Function: ");
    //    Serial.println(musicSize);

    //    Serial.println("** Melody in function **");
    //    for (int i = 0; i < valuesAdded; i++) {
    //      Serial.println(melody[i]);
    //    }
  } else {
    Serial.print("** Error: Could not open file ");
    Serial.println(musicFile);
    Serial.println("**");
  }
}

void playMelody() {
  static int* melody = nullptr;
  static int melodySize = 50;
  static int valuesAdded = 0;

  if (!initialized) {
    int tempo = melodies[currentSong][1].toInt();
    String title = melodies[currentSong][0];
    Serial.print("Title: ");
    Serial.println(title);
    Serial.print("Tempo: ");
    Serial.println(tempo);

    currentNote = 0;
    notes = 0;

    if (melody != nullptr) {
      delete[] melody;  // Free previous memory before allocating new
    }
    melodySize = 50;
    valuesAdded = 0;

    melody = new int[melodySize];
    readMelody(melody, melodySize, valuesAdded);

    notes = valuesAdded / 2;  // Number of notes is half of the valuesAdded
    noteStartTime = millis();
    initialized = true;
    return;
  }

  if (currentNote < notes) {
    unsigned long currentTime = millis();
    int tempo = melodies[currentSong][1].toInt();
    int wholenote = (60000 * 4) / tempo;

    if (isPlayingNote) {
      // Play the current note
      if (currentTime - noteStartTime >= noteDuration * 0.9) {  // Play 90% of the note
        noTone(buzzer);                                         // Stop the tone
        isPlayingNote = false;                                  // Switch to pause phase
        noteStartTime = currentTime;                            // Reset the start time for the pause
      }
    } else {
      // Pause phase
      if (currentTime - noteStartTime >= noteDuration * 0.1) {  // Pause for 10%
        currentNote++;                                          // Move to the next note
        if (currentNote < notes) {
          int divider = melody[currentNote * 2 + 1];
          if (divider > 0) {
            noteDuration = wholenote / divider;
          } else if (divider < 0) {
            noteDuration = wholenote / abs(divider) * 1.5;  // Handle dotted notes
          }
          tone(buzzer, melody[currentNote * 2], noteDuration * 0.9);
          noteStartTime = currentTime;  // Reset the start time for the next note
          isPlayingNote = true;         // Switch back to playing phase
        }
      }
    }
  } else {
    // Finished playing the melody
    delete[] melody;
    initialized = false;  // Reset for the next call
    currentNote = 0;
    notes = 0;
    valuesAdded = 0;
    melodySize = 50;
    // Serial.println("Finished playing the melody.");
    printMelodies();
  }
}
