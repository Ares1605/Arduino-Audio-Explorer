#include <SPI.h>
#include <SD.h>

// SD card chip select pin
const int chipSelect = 10;
// const int speakerPin = 4; // Pin for audio output (PWM)
const int buzzer = 4;





int currentSong = 1;

const int numberSongs = 5;
const int lineSize = 3;
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

  playMelody();
}

void loop() {
  // Nothing to do in the loop
  // Serial.println("Entering SD Card Test");
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
  int tempo = melodies[currentSong][1].toInt();
  String title = melodies[currentSong][0];
  Serial.print("Title: ");
  Serial.println(title);
  Serial.print("Tempo: ");
  Serial.println(tempo);

  int* melody;
  int melodySize = 50;
  int valuesAdded = 0;
  readMelody(melody, melodySize, valuesAdded);

  // Serial.println("** Melody **");
  // for (int i = 0; i < valuesAdded; i++) {
  //   Serial.println(melody[i]);
  // }

  Serial.print("Values Added: ");
  Serial.println(valuesAdded);

  Serial.print("Array Size: ");
  Serial.println(melodySize);

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = valuesAdded / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;  // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
