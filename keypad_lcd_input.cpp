/*
 * keypad_lcd_input.cpp
 * ------------------------------------------------------------
 * Two-step input form using a 4x4 keypad and a 16x2 I2C LCD.
 * The user enters a phone number, then an amount, and confirms.
 *
 * Board:     ESP32 Dev Module
 * Libraries: Keypad (Mark Stanley, Alexander Brevig)
 *            LiquidCrystal I2C (Frank de Brabander)
 * Baud rate: 115200
 *
 * Controls
 * ------------------------------------------------------------
 *   0-9, A-D  Add a character to the current field
 *   #         Confirm phone number, then confirm amount
 *   *         Clear everything and start over
 *
 * Wiring: LCD (I2C backpack)
 * ------------------------------------------------------------
 * | LCD Pin | ESP32 Pin                |
 * |---------|--------------------------|
 * | VCC     | 5V (or 3.3V if required) |
 * | GND     | GND                      |
 * | SDA     | GPIO 21                  |
 * | SCL     | GPIO 22                  |
 *
 * Wiring: 4x4 Keypad
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
 * | C4         | 12         |
 *
 * Note: GPIO 12 is a boot strapping pin. If uploads fail,
 * disconnect C4 while uploading.
 */

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- LCD configuration ----------
const uint8_t LCD_ADDRESS = 0x27;  // Try 0x3F if the screen shows no text
const uint8_t LCD_COLUMNS = 16;
const uint8_t LCD_ROWS    = 2;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// ---------- Keypad configuration ----------
const byte ROWS = 4;
const byte COLS = 4;

const char KEYS[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {14, 27, 26, 25};
byte colPins[COLS] = {33, 32, 13, 12};

Keypad keypad(makeKeymap(KEYS), rowPins, colPins, ROWS, COLS);

// ---------- Control keys ----------
const char KEY_RESET   = '*';
const char KEY_CONFIRM = '#';

const unsigned long CONFIRM_DISPLAY_MS = 2000;

// ---------- Input state ----------
String phone  = "";
String amount = "";
bool enteringPhone = true;

// ---------- Display helpers ----------

// Clears the LCD and prints a label on line 1 and a value on line 2.
void showScreen(const char* label, const String& value) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label);
  lcd.setCursor(0, 1);
  lcd.print(value);
}

void showPhone()  { showScreen("Phone:", phone); }
void showAmount() { showScreen("Amount:", amount); }

// Shows the confirmed phone number briefly, then the amount.
void showConfirmation() {
  showScreen("CONFIRMED", phone);
  delay(CONFIRM_DISPLAY_MS);
  showAmount();
}

// Clears all input and returns to the phone number step.
void resetInput() {
  phone = "";
  amount = "";
  enteringPhone = true;
  showScreen("Enter Phone:", "");
}

// ---------- Key handling ----------
void handleKey(char key) {
  if (key == KEY_RESET) {
    resetInput();
    return;
  }

  if (key == KEY_CONFIRM) {
    if (enteringPhone) {
      enteringPhone = false;
      showAmount();
    } else {
      showConfirmation();
    }
    return;
  }

  // Any other key is input for the current field
  if (enteringPhone) {
    phone += key;
    showPhone();
  } else {
    amount += key;
    showAmount();
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  resetInput();
}

// ---------- Main loop ----------
void loop() {
  char key = keypad.getKey();

  if (key) {
    handleKey(key);
  }
}
