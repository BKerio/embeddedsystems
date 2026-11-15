/*
 * keypad_4x3.cpp
 * ------------------------------------------------------------
 * Reads a 4x3 matrix keypad and prints each key press to the
 * Serial Monitor.
 *
 * Board:     ESP32 Dev Module
 * Library:   Keypad (Mark Stanley, Alexander Brevig)
 * Baud rate: 115200
 *
 * Wiring
 * ------------------------------------------------------------
 * | Keypad Pin | ESP32 GPIO |
 * |------------|------------|
 * | R1         | 14         |
 * | R2         | 27         |
 * | R3         | 26         |
 * | R4         | 25         |
 * | C1         | 33         |
 * | C2         | 32         |
 * | C3         | 13         |
 */

#include <Keypad.h>

// ---------- Keypad configuration ----------
const byte ROWS = 4;
const byte COLS = 3;

const char KEYS[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {14, 27, 26, 25};
byte colPins[COLS] = {33, 32, 13};

Keypad keypad(makeKeymap(KEYS), rowPins, colPins, ROWS, COLS);

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  Serial.println("Keypad ready. Press a key...");
}

// ---------- Main loop ----------
void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Pressed: ");
    Serial.println(key);
  }
}
