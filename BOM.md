# Parts — per vest

| Qty | Part                          | Notes                              | ~Cost |
|-----|-------------------------------|------------------------------------|-------|
| 1   | ESP32 DevKit v1               | 30-pin, USB-C preferred            | $6    |
| N   | Piezo disc 35mm               | bare disc, with leads (4–8 typical)| $0.50 ea |
| N   | 1N4148 signal diode           | one per disc, OR onto shared bus   | $0.05 ea |
| 1   | 1MΩ resistor                  | bus bleed                          | $0.02 |
| 1   | 3.3V zener (1N4728A)          | bus spike clamp                    | $0.10 |
| 1   | WS2812B strip, 10 LEDs        | cut from a 60/m roll               | $2    |
| 1   | Passive piezo buzzer          | PWM-driven                         | $0.50 |
| 1   | Tactile pushbutton            | reset / respawn                    | $0.10 |
| 1   | USB power bank (small)        | or 1S LiPo + TP4056 + boost        | $8    |
| —   | EVA foam pads + fabric vest   | piezos epoxied to thin rigid plate under foam | $10   |
| —   | Hookup wire, hot glue, JST    |                                    | $3    |

**≈ $32/player.**

## Power
**Easiest:** slim USB power bank (5000 mAh lipstick-style) into the DevKit's USB
port. ESP32 + 10 LEDs averages ~150 mA, so 5000 mAh runs a vest all day. No
charge circuit, no LiPo handling. One caveat: some banks auto-sleep at low
draw — pick one marketed as "always-on" / "low-current mode" / "IoT", or test
yours (this rig usually draws enough to stay awake).

**Hot-swappable alternative:** 18650 cell + holder + MT3608 boost-to-5V board
(~$5 total). Players swap a fresh cell between rounds.

## Prototype order (1 vest, Amazon search terms)
| Search term                                   | Qty    | ~Cost |
|-----------------------------------------------|--------|-------|
| `ESP32 DevKit V1 USB-C`                       | 1      | $8    |
| `35mm piezo disc transducer with leads`       | 10-pk  | $7    |
| `1M ohm resistor 1/4W`                        | pack   | $5    |
| `1N4728A 3.3V zener diode`                    | pack   | $5    |
| `1N4148 signal diode`                         | 100-pk | $5    |
| `WS2812B LED strip 60/m 1m`                   | 1      | $8    |
| `passive piezo buzzer 3-5V`                   | pack   | $5    |
| `tactile push button 6mm`                     | pack   | $5    |
| `half size breadboard jumper wire kit`        | 1      | $10   |
| `5000mAh power bank slim always-on`           | 1      | $12   |
| `22 AWG solid core hookup wire kit`           | 1      | $8    |
| `JST-XH 2.54mm connector kit` (for perfboard later) | 1 | $10   |
| `EVA foam sheet 6mm`                          | 1      | $10   |

Hot-glue gun + old credit cards (piezo backing plates) assumed on hand.
**≈ $93** gets you everything for vest #1 with leftovers for several more.

## Mounting tip
Glue each piezo to a thin plastic plate (credit-card stiffness), then sandwich:
`outer foam → piezo+plate → inner foam → body`. The plate spreads impact so a
hit anywhere on the pad rings the disc; foam-only mounting deadens the signal.

## Calibration
Flash, open serial monitor, whack the pad at "valid hit" force, note peak ADC
values. Set `HIT_THRESHOLD` ~70% of the weakest valid hit. Jog in place to
confirm no false positives.
