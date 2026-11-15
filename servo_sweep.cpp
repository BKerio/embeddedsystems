/*
 * servo_sweep.cpp
 * ------------------------------------------------------------
 * Sweeps a hobby servo (SG90, MG996R, etc.) from 0 to 180
 * degrees and back, continuously.
 *
 * Board:     ESP32 Dev Module
 * Library:   ESP32Servo (Kevin Harrington)
 * Baud rate: 115200
 *
 * Wiring
 * ------------------------------------------------------------
 * | Servo Wire             | Connect To                        |
 * |------------------------|-----------------------------------|
 * | Signal (orange/yellow) | ESP32 GPIO 13                     |
 * | VCC (red)              | External 5-6 V supply (+)         |
 * | GND (brown/black)      | External supply (-) AND ESP32 GND |
 *
 * IMPORTANT: Do not power the servo from the ESP32 3.3V or 5V
 * pins. The external supply ground must be connected to ESP32
 * GND, or the servo will not respond reliably.
 *
 * Safe servo pins: 13, 14, 16-19, 21-23, 25-27, 32, 33
 * Avoid: 0-12 and 15 (boot, flash and UART pins)
 */

#include <ESP32Servo.h>

// ---------- Servo configuration ----------
const int SERVO_PIN       = 13;
const int SERVO_FREQ_HZ   = 50;    // Standard servo frequency
const int PULSE_MIN_US    = 500;   // Pulse width at 0 degrees
const int PULSE_MAX_US    = 2400;  // Pulse width at 180 degrees

const int ANGLE_MIN       = 0;
const int ANGLE_MAX       = 180;
const int STEP_DELAY_MS   = 15;    // Lower = faster sweep

Servo servo;

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);

  // Allocate all hardware timers for PWM
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo.setPeriodHertz(SERVO_FREQ_HZ);
  servo.attach(SERVO_PIN, PULSE_MIN_US, PULSE_MAX_US);

  Serial.print("Servo ready on GPIO ");
  Serial.println(SERVO_PIN);
}

// ---------- Main loop ----------
void loop() {
  // Sweep forward
  for (int angle = ANGLE_MIN; angle <= ANGLE_MAX; angle++) {
    servo.write(angle);
    delay(STEP_DELAY_MS);
  }

  // Sweep back
  for (int angle = ANGLE_MAX; angle >= ANGLE_MIN; angle--) {
    servo.write(angle);
    delay(STEP_DELAY_MS);
  }
}
