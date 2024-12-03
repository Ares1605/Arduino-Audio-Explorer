// Button pins
const int prevButtonPin = 4;     // Button for previous song
const int nextButtonPin = 8;     // Button for next song
const int pauseButtonPin = 13;   // Button for play/pause

// Potentiometer pin
const int potPin = A3;           // Potentiometer connected to analog pin A3

// Variables to store button states
int prevButtonState = 0;
int nextButtonState = 0;
int pauseButtonState = 0;
int lastPrevButtonState = 0;
int lastNextButtonState = 0;
int lastPauseButtonState = 0;

// Variables for debouncing using millis
unsigned long lastDebounceTime = 0;  // the last time the button state was changed
unsigned long debounceDelay = 50;     // the debounce time; increase if the button bounces

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set button pins as input
  pinMode(prevButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  pinMode(pauseButtonPin, INPUT);
  
  // Initialize potentiometer pin (as INPUT by default)
  pinMode(potPin, INPUT);
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(potPin);  // Read the potentiometer value (0-1023)
  
  // Read the button states
  int currentPrevButtonState = digitalRead(prevButtonPin);
  int currentNextButtonState = digitalRead(nextButtonPin);
  int currentPauseButtonState = digitalRead(pauseButtonPin);
  
  // Print the potentiometer value
  Serial.print("Potentiometer value: ");
  Serial.println(potValue);

  // Check for previous song button press with debounce
  if (currentPrevButtonState != lastPrevButtonState) {
    lastDebounceTime = millis();  // reset debounce timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentPrevButtonState == HIGH) {
      Serial.println("Previous song button pressed.");
    }
  }

  // Check for next song button press with debounce
  if (currentNextButtonState != lastNextButtonState) {
    lastDebounceTime = millis();  // reset debounce timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentNextButtonState == HIGH) {
      Serial.println("Next song button pressed.");
    }
  }

  // Check for play/pause button press with debounce
  if (currentPauseButtonState != lastPauseButtonState) {
    lastDebounceTime = millis();  // reset debounce timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentPauseButtonState == HIGH) {
      Serial.println("Play/Pause button pressed.");
    }
  }

  // Save the current button states for the next loop
  lastPrevButtonState = currentPrevButtonState;
  lastNextButtonState = currentNextButtonState;
  lastPauseButtonState = currentPauseButtonState;

  // Add a short delay to prevent serial monitor overload
  delay(50);
}
