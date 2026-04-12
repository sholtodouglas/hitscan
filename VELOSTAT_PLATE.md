# Velostat hit plate — production spec

**Design intent:** large-area force-gated hit pad, jog-proof, rain-proof.
Two regions (chest + back), wired to one ESP32 in a sealed hip box with
a ring-LED health display.

## Pad stack — laminated, no foam

```
  ┌──────────────────────────────────────┐
  │  0.5 mm TPU / vinyl outer skin       │  ← strike face
  │  copper fabric electrode A           │
  │  Velostat (overlaps copper +5mm)     │
  │  copper fabric electrode B           │
  │  0.5 mm TPU / vinyl inner skin       │  ← body side
  └────────────┬─────────────────────────┘
               │ M8 2-pin IP67 panel jack, potted in
```

**Size:** as big as the vest panel — ~25×30 cm chest, ~25×30 cm back.

**Lamination:** stack the three conductive layers, slide into a TPU/vinyl
pouch (or two sheets), **impulse-seal three edges**, fit the M8 jack
through the fourth edge in a punched hole, bed it in RTV silicone, then
seal the fourth edge around it. Don't vacuum it tight — a little air gap
preserves dynamic range.

Round all copper-fabric corners. Velostat must overlap copper ≥5 mm on
every side (prevents the edge-short you hit on the bench).

**Electrode leads:** snap-fastener through each copper layer (you have
these), ring-terminal on a short wire to the M8 jack's solder cups. Hot-
glue the first 20 mm of wire flat so flex never reaches the joint.

## Connector — M8 2-pin IP67

One per pad. Panel jack potted into the pad edge; pre-molded M8 cable on
the harness side. Same family as the NFC spec — common spares.

## Hip box

IP65 junction box ~130×80×50 with cable glands. Holds:
- ESP32
- power bank (or 18650 + holder)
- 22Ω + 1Ω divider network (see below)
- buzzer
- **WS2812 ring LED** (12 or 16 px) behind a clear acrylic window in the
  lid — visible health from any angle

Two M8 cables (chest, back) + USB charge port enter via glands.

## Two-region wiring

Simplest (hit/no-hit, no zone): **pads in parallel** on one divider.

```
 3V3 ──[22Ω ½W]──┬──[ chest pad ]──┐
                 └──[ back pad  ]──┤
                                   ├── P32 (ADC)
                                 [1Ω]
                                   │
                                  GND
```

The 22Ω is a **current limiter**: at 1Ω divider, a pad short would
otherwise pull ~3A through the ESP32's 3V3 regulator and cook the
resistor. With 22Ω in series, worst-case is ~140 mA / 0.5 W — safe.
Barely affects readings (22Ω ≪ idle-Velostat kΩ).

Zone-aware variant: each pad on its own divider → P32 and P33. Firmware
change is two `analogRead`s; do it later if you want chest-vs-back rules.

## Ring LED

WS2812B 16-LED ring, ~44 mm OD. Pre-wired ones exist. `NUM_LEDS = 16` in
firmware. Health renders as a shrinking arc; death = full red spin.

## Shopping list (per vest, 2 pads + box)

| Qty | Part | Link | Cost |
|---|---|---|---|
| 1 | Velostat 11×11" sheet (cuts both pads) | [adafruit 1361](https://www.adafruit.com/product/1361) | $5 |
| 0.5 m | Copper fabric (you have) | — | — |
| 2 | TPU/vinyl sheet 0.5 mm, 30×35 cm ×2 | [amazon](https://www.amazon.com/s?k=clear+vinyl+sheet+20+gauge) | $10 |
| 2 | M8 **2-pin** IP67 panel jack | [amazon](https://www.amazon.com/s?k=M8+2+pin+panel+mount+connector+IP67) | $8 |
| 2 | M8 2-pin pre-molded cable, 0.5 m | [amazon](https://www.amazon.com/s?k=M8+2+pin+male+cable+IP67) | $10 |
| 1 | RTV silicone sealant | [amazon](https://www.amazon.com/s?k=RTV+silicone+sealant+clear) | $7 |
| 1 | Impulse heat sealer 12" | [amazon](https://www.amazon.com/s?k=impulse+heat+sealer+12+inch) | $25 |
| 1 | IP65 junction box ~130×80×50 + glands | [amazon](https://www.amazon.com/s?k=IP65+junction+box+130x80+cable+gland) | $10 |
| 1 | **WS2812B 16-LED ring**, pre-wired | [amazon](https://www.amazon.com/s?k=WS2812B+16+LED+ring+neopixel) | $8 |
| 1 | 40 mm clear acrylic disc (lid window) | [amazon](https://www.amazon.com/s?k=40mm+clear+acrylic+disc) | $5 |
| — | 22Ω ½W + 1Ω ¼W resistors | (in your kit / [amazon](https://www.amazon.com/s?k=22+ohm+half+watt+resistor)) | $5 |
| — | snap fasteners, ring terminals, hot glue | (you have) | — |

**≈ $93/vest** (impulse sealer is one-time, amortizes). About a third the
cost of the NFC plate.

## Firmware

`./flash.sh vest-game` already covers it. To do before production flash:
- `NUM_LEDS = 16` (ring)
- optional: per-zone ADC (P32 + P33) if you want chest/back distinction
- the ring-arc `renderHp()` — nicer than the linear bar
