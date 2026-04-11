# NFC chest/back plate — spec

## The constraint

Largest off-the-shelf flex NFC coil ≈ **50×35mm** (Taoglas FXR.01.A,
Abracon ANFCA-5035). Reader↔antenna are a **tuned pair** — swapping in a
non-stock coil can cut range to 1/5 without re-matching caps. So: tile
small matched units, don't chase one big coil.

## Recommended build: tiled RC522 Mini

The RC522 Mini board **is** ~38×25×5mm — same footprint as the flex
antennas, with the reader chip already on it. Putting the whole module on
the chest (potted) is simpler and ~4× cheaper than remote-antenna PN532
for the same coverage.

| Qty | Part | Link | ~Cost |
|---|---|---|---|
| 6 | **RC522 Mini** RFID module | [amazon](https://www.amazon.com/s?k=RC522+mini+RFID+module+small) | $4 ea |
| 1 | 8-conductor ribbon cable, 26AWG | [amazon](https://www.amazon.com/s?k=8+conductor+ribbon+cable+26+awg) | $8 |
| 6 | JST-XH 8-pin pairs (module ↔ ribbon) | (in your JST kit) | — |
| 1 | Conformal coating spray (waterproof) | [amazon](https://www.amazon.com/s?k=MG+Chemicals+422C+conformal+coating) | $15 |
| 1 | 6mil poly bags / heat-shrink wrap | [amazon](https://www.amazon.com/s?k=3x4+6mil+poly+bag) | $8 |
| — | thin ABS or PETG strike plate (you have) | — | — |

**≈ $55 covers 5-zone plates for one vest + a spare.**

## Wiring

All readers share **3.3V / GND / SCK(14) / MISO(27) / MOSI(13) / RST(22)**
on a 6-wire bus (the ribbon). Each reader's **SDA** gets its own ESP32 pin:

```c
constexpr int NFC_SS[] = { 4, 16, 17, 25, 26 };   // chest L/C/R, back U/L
```

Edit the array in `firmware/vest/main.cpp`, `./flash.sh vest-nfc`. Hit
prints `reader N tag UID: …` — zone + attacker for free.

## Waterproofing

1. Conformal-coat each RC522 Mini (spray both sides, dry 24h)
2. Slip into a thin poly bag, heat-seal or tape shut
3. Mount behind 1mm PETG strike plate (NFC reads through ~5mm plastic fine)
4. Ribbon exits via a glued grommet; JST at the hip-box end

## Layout (top-down, 5 zones)

```
   chest                    back
┌─────────────┐        ┌─────────────┐
│ [0] [1] [2] │        │     [3]     │
│  L   C   R  │        │   upper     │
│             │        │     [4]     │
└─────────────┘        │   lower     │
                       └─────────────┘
```

Each zone ≈ 4×2.5cm read area. Gaps between zones are intentional — a
flush, deliberate hit lands; a glancing swipe misses. That's the design.

## Alt: PN532 + flex coil (if you want board-in-box)

Only if RC522-on-chest proves too fragile. Reader sits in hip box; only a
paper-thin coil + coax goes to the vest.

| Part | Link | Note |
|---|---|---|
| Elechouse **PN5321 MINI** w/ ext antenna | [elechouse](https://www.elechouse.com/) | bundled matched coil; ~$18 ea |
| Taoglas FXR.01.A (53×37mm, u.FL) | [digikey](https://www.digikey.com/en/products/result?keywords=FXR.01.A) | 50Ω-matched, *should* pair |

Needs `Adafruit_PN532` lib — separate firmware mode, not yet written.

## Game hooks (zone-aware)

- **Back-stab** (readers 3/4): instant kill or 2× damage
- **Heart** (reader 1): instant kill
- **Weapon-specific**: tag UID → lookup table → dagger only scores on back,
  arrow only on chest, etc.
- Zone lights up on the LED bar in a different color per location
