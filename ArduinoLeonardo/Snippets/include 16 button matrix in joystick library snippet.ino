#include <Joystick.h>

// Define the number of rows and columns in the button matrix
const int ROWS = 4;
const int COLS = 4;

// Define the pins connected to the rows and columns of the button matrix
int rowPins[ROWS] = {2, 3, 4, 5};
int colPins[COLS] = {6, 7, 8, 9};

// Define the button matrix array
int buttonMatrix[ROWS][COLS] = {
  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {8, 9, 10, 11},
  {12, 13, 14, 15}
};

// Initialize the Joystick library
Joystick_ joystick;

void setup() {
  // Configure the row pins as outputs
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
  }

  // Configure the column pins as inputs with pull-up resistors
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  // Initialize the Joystick library
  joystick.begin();
}

void loop() {
  // Read the state of each button in the matrix
  for (int row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < COLS; col++) {
      int buttonState = digitalRead(colPins[col]);
      joystick.setButton(buttonMatrix[row][col], buttonState);
    }
    digitalWrite(rowPins[row], HIGH);
  }
}
