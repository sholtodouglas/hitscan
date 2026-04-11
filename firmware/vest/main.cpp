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
#define MODE_NFC       3   // RC522 reader, passive tag in weapon tip (SPI)

#define SENSOR_MODE  MODE_PIEZO

// Set to 1, flash, open Serial Monitor @115200, hit the pad to see raw
// readings. Use this to pick HIT_THRESHOLD for analog modes.
#define CALIBRATION 0

// ---------- tuning ----------
constexpr int   MAX_HP        = 5;
constexpr int   HIT_THRESHOLD = 800;   // 0..4095; raise if false-positives from running/jostle
constexpr int   DEBOUNCE_MS   = 300;   // min gap between registered hits
constexpr int   NUM_LEDS      = 8;

// ---------- pins ----------
constexpr int   SENSOR_PIN    = 32;
constexpr int   LED_PIN       = 5;
constexpr int   BUZZER_PIN    = 18;
constexpr int   RESET_PIN     = 19;

#if SENSOR_MODE == MODE_NFC
#include <SPI.h>
#include <MFRC522.h>
constexpr int NFC_SCK = 14, NFC_MISO = 27, NFC_MOSI = 13, NFC_SS = 4, NFC_RST = 22, NFC_IRQ = 21;
MFRC522 nfc(NFC_SS, NFC_RST);
volatile bool nfcIrq = false;
void IRAM_ATTR nfcISR() { nfcIrq = true; }

void nfcActivateRx() {
  nfc.PCD_WriteRegister(MFRC522::ComIrqReg, 0x7F);                 // clear IRQ bits
  nfc.PCD_WriteRegister(MFRC522::FIFOLevelReg, 0x80);              // flush FIFO
  nfc.PCD_WriteRegister(MFRC522::FIFODataReg, MFRC522::PICC_CMD_REQA);
  nfc.PCD_WriteRegister(MFRC522::CommandReg, MFRC522::PCD_Transceive);
  nfc.PCD_WriteRegister(MFRC522::BitFramingReg, 0x87);             // StartSend, 7-bit
}
#endif

// ---------- state ----------
CRGB     leds[NUM_LEDS];
int      hp;
uint32_t lastHitMs;
bool     armed;

void renderHp() {
  int lit = map(hp, 0, MAX_HP, 0, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < lit) {
      uint8_t hue = map(i, 0, NUM_LEDS - 1, 0, 96);   // red -> green
      leds[i] = CHSV(hue, 255, 255);
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void flash(CRGB c, int ms) {
  fill_solid(leds, NUM_LEDS, c);
  FastLED.show();
  delay(ms);
}

void resetGame() {
  hp = MAX_HP;
  lastHitMs = 0;
  armed = true;
  tone(BUZZER_PIN, 880, 120);
  flash(CRGB::Blue, 200);
  renderHp();
}

void die() {
  tone(BUZZER_PIN, 200, 1500);
  for (int i = 0; i < 6; i++) {
    flash(CRGB::Red, 150);
    flash(CRGB::Black, 150);
  }
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
}

void registerHit() {
  hp--;
  lastHitMs = millis();
  tone(BUZZER_PIN, 440, 150);
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
  if (!nfcIrq) return 0;
  nfcIrq = false;
  bool ok = nfc.PICC_ReadCardSerial();
  if (ok) {
    Serial.print("tag UID:");
    for (byte i = 0; i < nfc.uid.size; i++) Serial.printf(" %02X", nfc.uid.uidByte[i]);
    Serial.println();
    nfc.PICC_HaltA();
  }
  nfcActivateRx();
  return ok ? 4095 : 0;
#else
  return analogRead(SENSOR_PIN);
#endif
}

bool triggered() {
  return rawSensor() > HIT_THRESHOLD;
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
#if SENSOR_MODE == MODE_BUTTON
  pinMode(SENSOR_PIN, INPUT_PULLUP);
#elif SENSOR_MODE == MODE_NFC
  SPI.begin(NFC_SCK, NFC_MISO, NFC_MOSI, NFC_SS);
  nfc.PCD_Init();
  pinMode(NFC_IRQ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(NFC_IRQ), nfcISR, FALLING);
  nfc.PCD_WriteRegister(MFRC522::ComIEnReg, 0xA0);   // RxIRq enable, IRQ pin push-pull active-low
  nfcActivateRx();
#endif
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
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

  if (hp <= 0) {
    // dead: hold red, wait for reset
    return;
  }

  bool t = triggered();
  if (!t) armed = true;
  if (armed && t && millis() - lastHitMs > DEBOUNCE_MS) {
    armed = false;
    registerHit();
  }
}
