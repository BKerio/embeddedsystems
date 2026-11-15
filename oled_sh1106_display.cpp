/*
 * oled_sh1106_display.cpp
 * ------------------------------------------------------------
 * Test sketch for a 1.3" 128x64 OLED (e.g. JMD 1.3A) with the
 * SH1106 controller. Shows a message and a frame to confirm
 * the display is wired and working.
 *
 * Board:     ESP32 Dev Module
 * Libraries: Adafruit GFX Library (Adafruit)
 *            Adafruit SH110X (Adafruit)
 * Baud rate: 115200
 *
 * Wiring (I2C)
 * ------------------------------------------------------------
 * | OLED Pin | ESP32 Pin |
 * |----------|-----------|
 * | VCC      | 3.3V      |
 * | GND      | GND       |
 * | SDA      | GPIO 21   |
 * | SCL      | GPIO 22   |
 *
 * Notes:
 * - Most modules use I2C address 0x3C. Some use 0x3D.
 * - This display needs the SH1106 driver, not SSD1306.
 * - Keep wires short for a stable picture.
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ---------- Display configuration ----------
const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
const uint8_t OLED_ADDRESS = 0x3C;  // Try 0x3D if the screen stays blank

const int I2C_SDA_PIN = 21;
const int I2C_SCL_PIN = 22;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!display.begin(OLED_ADDRESS, true)) {
    Serial.println("OLED not found. Check wiring and I2C address.");
    while (true) {
      delay(1000);  // Stop here
    }
  }

  display.clearDisplay();

  // Text
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("OLED WORKING!");
  display.println("ESP32 OK");

  // Frame across the bottom half of the screen
  display.drawRect(0, 30, SCREEN_WIDTH, 34, SH110X_WHITE);

  // Nothing appears until display() is called
  display.display();

  Serial.println("OLED ready.");
}

// ---------- Main loop ----------
void loop() {
  // Nothing to do: the screen is drawn once in setup()
}
