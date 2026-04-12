#include <Arduino.h>
/*
 * Hunger Games Vest — standalone hit-detection firmware
 * Board: ESP32 DevKit (arduino-esp32 core 3.x for tone())
 * Libs:  FastLED
 *
 * SENSOR WIRING (pick one, set SENSOR_MODE below)
 *   PIEZO    — diode-OR bus: each piezo red -> 1N4148 -> bus -> GPIO 32.
 *              1MΩ + 3.3V zener from bus to GND. Piezo blacks to GND.
 *   PRESSURE — FSR or Velostat pad as top half of a divider:
 *              3V3 -> pad -> GPIO 32 -> 10kΩ -> GND.
 *   BUTTON   — all microswitch NO -> GPIO 32, all COM -> GND. No passives.
 *
 *   WS2812B stick -> GPIO 5   (DIN), 5V, GND
 *   Passive buzzer-> GPIO 18, GND
 *   Reset button  -> GPIO 19 to GND (uses internal pullup)
 *   Power: USB bank or 1S LiPo + TP4056 into 5V/VIN.
 */

#include <FastLED.h>

// ---------- sensor selection ----------
#define MODE_PIEZO     0   // impact spike (analog)
#define MODE_PRESSURE  1   // FSR or Velostat divider (analog)
#define MODE_BUTTON    2   // arcade microswitch OR LM567 tone-decoder output (digital, active-low)
#define MODE_NFC       3   // RC522 reader(s), passive tag in weapon tip (SPI)
#define MODE_NFC_PN532 4   // PN532 reader(s) w/ external flex antennas (SPI)

#ifndef SENSOR_MODE
#define SENSOR_MODE  MODE_PIEZO
#endif

// Set to 1, flash, open Serial Monitor @115200, hit the pad to see raw
// readings. Use this to pick HIT_THRESHOLD for analog modes.
#ifndef CALIBRATION
#define CALIBRATION 0
#endif

// ---------- tuning ----------
constexpr int   MAX_HP        = 5;
#ifndef HIT_THRESHOLD
#define HIT_THRESHOLD 800
#endif
#ifndef DEBOUNCE_MS
#define DEBOUNCE_MS 300
#endif
constexpr int   hitThreshold  = HIT_THRESHOLD;   // 0..4095
constexpr int   debounceMs    = DEBOUNCE_MS;
#ifndef NUM_LEDS
#define NUM_LEDS 8
#endif
constexpr int   numLeds       = NUM_LEDS;

// ---------- pins ----------
constexpr int   SENSOR_PIN    = 32;
constexpr int   LED_PIN       = 5;
constexpr int   BUZZER_PIN    = 18;
constexpr int   RESET_PIN     = 19;
constexpr int   ONBOARD_LED   = 2;

#if SENSOR_MODE == MODE_NFC || SENSOR_MODE == MODE_NFC_PN532
#include <SPI.h>
constexpr int NFC_SCK = 14, NFC_MISO = 27, NFC_MOSI = 13;
// One chip-select pin per reader. Tile more by adding pins — all readers
// share SCK/MISO/MOSI.
constexpr int NFC_SS[]    = { 4, 16, 17, 25, 26 };
constexpr int NUM_READERS = sizeof(NFC_SS) / sizeof(NFC_SS[0]);
int lastReader = -1;
#endif

#if SENSOR_MODE == MODE_NFC
#include <MFRC522.h>
constexpr int NFC_RST = 22;
MFRC522 nfc[NUM_READERS];
#elif SENSOR_MODE == MODE_NFC_PN532
#include <Adafruit_PN532.h>
Adafruit_PN532* nfc[NUM_READERS];
#endif

// ---------- state ----------
CRGB     leds[numLeds];
int      hp;
uint32_t lastHitMs;
bool     armed;
int      hitPeak;

void renderHp() {
  int lit = map(hp, 0, MAX_HP, 0, numLeds);
  for (int i = 0; i < numLeds; i++) {
    if (i < lit) {
      uint8_t hue = map(i, 0, numLeds - 1, 0, 96);   // red -> green
      leds[i] = CHSV(hue, 255, 255);
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void flash(CRGB c, int ms) {
  fill_solid(leds, numLeds, c);
  FastLED.show();
  delay(ms);
}

void beep(int ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void resetGame() {
  hp = MAX_HP;
  lastHitMs = millis();
  armed = false;
  beep(120);
  flash(CRGB::Blue, 200);
  FastLED.clear(true);
}

void die() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ONBOARD_LED, HIGH);
    beep(150);
    flash(CRGB::Red, 50);
    digitalWrite(ONBOARD_LED, LOW);
    flash(CRGB::Black, 150);
  }
  resetGame();
}

void registerHit(int val) {
  hp--;
  lastHitMs = millis();
  Serial.printf("HIT! val=%d hp=%d\n", val, hp);
  beep(150);
  flash(CRGB::White, 120);
  if (hp <= 0) {
    hp = 0;
    die();
  } else {
    renderHp();
  }
}

int rawSensor() {
#if SENSOR_MODE == MODE_BUTTON
  return digitalRead(SENSOR_PIN) == LOW ? 4095 : 0;
#elif SENSOR_MODE == MODE_NFC
  for (int r = 0; r < NUM_READERS; r++) {
    if (!nfc[r].PICC_IsNewCardPresent()) continue;
    if (!nfc[r].PICC_ReadCardSerial())   continue;
    lastReader = r;
    Serial.printf("reader %d  tag UID:", r);
    for (byte i = 0; i < nfc[r].uid.size; i++) Serial.printf(" %02X", nfc[r].uid.uidByte[i]);
    Serial.println();
    nfc[r].PICC_HaltA();
    return 4095;
  }
  return 0;
#elif SENSOR_MODE == MODE_NFC_PN532
  uint8_t uid[7], uidLen;
  for (int r = 0; r < NUM_READERS; r++) {
    if (!nfc[r]->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 30)) continue;
    lastReader = r;
    Serial.printf("reader %d  tag UID:", r);
    for (byte i = 0; i < uidLen; i++) Serial.printf(" %02X", uid[i]);
    Serial.println();
    return 4095;
  }
  return 0;
#else
  return analogRead(SENSOR_PIN);
#endif
}

#if CALIBRATION
void runCalibration() {
  static int peak = 0;
  static uint32_t lastPrint = 0;

  int v = rawSensor();
  if (v > peak) peak = v;
  if (millis() - lastPrint > 200) {
    Serial.printf("raw: %4d  peak: %4d\n", v, peak);
    peak = 0;
    lastPrint = millis();
  }
}
#endif

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
#if SENSOR_MODE == MODE_BUTTON
  pinMode(SENSOR_PIN, INPUT_PULLUP);
#elif SENSOR_MODE == MODE_NFC
  SPI.begin(NFC_SCK, NFC_MISO, NFC_MOSI);
  for (int r = 0; r < NUM_READERS; r++) {
    nfc[r].PCD_Init(NFC_SS[r], NFC_RST);
    delay(50);
    byte ver = nfc[r].PCD_ReadRegister(MFRC522::VersionReg);
    Serial.printf("reader %d (SS=%d): version 0x%02X  %s\n", r, NFC_SS[r], ver,
      (ver == 0x91 || ver == 0x92) ? "OK" : "FAIL/absent");
  }
#elif SENSOR_MODE == MODE_NFC_PN532
  SPI.begin(NFC_SCK, NFC_MISO, NFC_MOSI);
  for (int r = 0; r < NUM_READERS; r++) {
    nfc[r] = new Adafruit_PN532(NFC_SS[r], &SPI);
    nfc[r]->begin();
    uint32_t ver = nfc[r]->getFirmwareVersion();
    Serial.printf("reader %d (SS=%d): %s\n", r, NFC_SS[r],
      ver ? "PN532 OK" : "FAIL/absent");
    if (ver) nfc[r]->SAMConfig();
  }
#endif
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, numLeds);
  FastLED.setBrightness(180);
  resetGame();
}

void loop() {
#if CALIBRATION
  runCalibration();
  return;
#endif

  if (digitalRead(RESET_PIN) == LOW) {
    resetGame();
    delay(400);
    return;
  }

  int v = rawSensor();
  bool cooling = millis() - lastHitMs < (uint32_t)debounceMs;
  digitalWrite(ONBOARD_LED, cooling ? HIGH : LOW);

  if (cooling) {
    if (v > hitPeak) hitPeak = v;
  } else if (hitPeak > 0) {
    Serial.printf("  (peak %d)\n", hitPeak);
    hitPeak = 0;
    FastLED.clear(true);
  }

  if (v <= hitThreshold) armed = true;
  if (armed && v > hitThreshold && !cooling) {
    armed = false;
    hitPeak = v;
    registerHit(v);
  }
}
