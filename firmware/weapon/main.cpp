#include <Arduino.h>
/*
 * Weapon transmitter — alternative to the 555 circuit in wiring.md.
 * Flash this onto a spare ESP32 to bench-test the LM567 receiver
 * before the LMC555 arrives (or instead of the NE555+2×CR2032 stack).
 *
 * Wiring: TX_PIN -> 100nF -> 1k -> blade wrap. Board GND to battery (–).
 */

constexpr int  TX_PIN  = 25;     // GPIO 25 on ESP32; change for other boards
constexpr long TX_FREQ = 3600;   // Hz; match LM567 on the vest

void setup() {
  pinMode(TX_PIN, OUTPUT);
  tone(TX_PIN, TX_FREQ);         // runs forever
}

void loop() {}
