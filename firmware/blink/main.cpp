#include <Arduino.h>

// Smoke test: proves toolchain + board + USB + serial all work.
// Onboard LED (GPIO 2 on most DevKit V1 clones) blinks; serial prints a counter.

constexpr int LED = 2;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(300);
  Serial.println("\n[blink] esp32 alive");
}

void loop() {
  static uint32_t n = 0;
  digitalWrite(LED, n & 1);
  Serial.printf("tick %lu\n", n++);
  delay(500);
}
