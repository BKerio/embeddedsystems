/*
 * ultrasonic_distance.cpp
 * ------------------------------------------------------------
 * Measures distance with an HC-SR04 ultrasonic sensor and
 * prints it in centimetres to the Serial Monitor every 500 ms.
 *
 * Board:     ESP32 Dev Module
 * Libraries: None
 * Baud rate: 115200
 *
 * Wiring
 * ------------------------------------------------------------
 * | HC-SR04 Pin | ESP32 Pin |
 * |-------------|-----------|
 * | VCC         | 5V (VIN)  |
 * | GND         | GND       |
 * | TRIG        | GPIO 5    |
 * | ECHO        | GPIO 18   |
 *
 * WARNING: The HC-SR04 ECHO pin outputs 5 V, but ESP32 GPIOs
 * accept 3.3 V only. Use a voltage divider (1k + 2k ohm) on
 * ECHO, or use a 3.3 V sensor such as the HC-SR04P.
 */

// ---------- Pin configuration ----------
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

// ---------- Measurement settings ----------
const unsigned long ECHO_TIMEOUT_US = 30000;   // ~5 m max range
const float SOUND_SPEED_CM_PER_US   = 0.0343;  // At ~20 C
const unsigned long READ_INTERVAL_MS = 500;

// Sends a trigger pulse and returns the distance in cm.
// Returns 0 if no echo was received before the timeout.
float readDistanceCm() {
  // Make sure the trigger starts low
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // A 10 us high pulse starts a measurement
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo pulse length is the round-trip time of the sound
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // Divide by 2 because the sound travels there and back
  return (durationUs * SOUND_SPEED_CM_PER_US) / 2.0;
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// ---------- Main loop ----------
void loop() {
  float distanceCm = readDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  delay(READ_INTERVAL_MS);
}
