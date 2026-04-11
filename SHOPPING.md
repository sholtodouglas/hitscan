# Prototype shopping list — all three sensor variants

Links are Amazon **search URLs** (stable, won't go stale) — click, pick the
top well-reviewed result. One order covers piezo + FSR + button prototypes.

## Shared core (needed for every variant)

| Part | Link | ~Cost |
|---|---|---|
| ESP32 DevKit V1, USB-C | [amazon](https://www.amazon.com/s?k=ESP32+DevKit+V1+USB-C) | $8 |
| WS2812B 8-LED stick (rigid PCB bar) | [amazon](https://www.amazon.com/s?k=WS2812B+8+LED+stick+neopixel) | $7 |
| Passive piezo buzzer 3–5V | [amazon](https://www.amazon.com/s?k=passive+piezo+buzzer+arduino) | $5 |
| Tactile pushbutton 6mm (reset) | [amazon](https://www.amazon.com/s?k=tactile+push+button+6mm+arduino) | $5 |
| Breadboard + jumper kit | [amazon](https://www.amazon.com/s?k=half+size+breadboard+jumper+wire+kit) | $10 |
| Alligator-clip-to-Dupont jumpers | [amazon](https://www.amazon.com/s?k=alligator+clip+to+dupont+jumper+wire) | $6 |
| 22AWG solid hookup wire kit | [amazon](https://www.amazon.com/s?k=22+awg+solid+core+hookup+wire+kit) | $8 |
| 22AWG **stranded** silicone wire | [amazon](https://www.amazon.com/s?k=22+awg+stranded+silicone+wire+kit) | $10 |
| Slim USB power bank, always-on | [amazon](https://www.amazon.com/s?k=5000mah+power+bank+always+on+low+current) | $12 |
| JST-XH 2.54mm connector kit | [amazon](https://www.amazon.com/s?k=JST+XH+2.54mm+connector+kit+crimper) | $12 |
| EVA foam sheet 6mm | [amazon](https://www.amazon.com/s?k=EVA+foam+sheet+6mm+cosplay) | $10 |
| Resistor assortment kit | [amazon](https://www.amazon.com/s?k=resistor+assortment+kit+1%2F4W) | $8 |
| Heat shrink tubing assortment | [amazon](https://www.amazon.com/s?k=heat+shrink+tubing+assortment) | $7 |
| Hot glue gun + sticks | [amazon](https://www.amazon.com/s?k=mini+hot+glue+gun+with+sticks) | $10 |
| Soldering iron kit (adjustable temp) | [amazon](https://www.amazon.com/s?k=soldering+iron+kit+adjustable+temperature) | $20 |
| 63/37 rosin-core solder, 0.8mm | [amazon](https://www.amazon.com/s?k=63%2F37+rosin+core+solder+0.8mm) | $8 |

**Core ≈ $113** (with leftovers for many vests).

## Variant A — Piezo (force-thresholded, current firmware)

| Part | Link | ~Cost |
|---|---|---|
| 35mm piezo discs, 10-pack | [amazon](https://www.amazon.com/s?k=35mm+piezo+disc+transducer+leads) | $7 |
| 1N4148 signal diodes, 100-pack | [amazon](https://www.amazon.com/s?k=1N4148+diode+100+pack) | $5 |
| 1N4728A 3.3V zener | [amazon](https://www.amazon.com/s?k=1N4728A+3.3V+zener+diode) | $5 |

(1MΩ is in the resistor kit.) **+$17**

## Variant B — Force-sensitive resistor (rugged, pressure-thresholded)

| Part | Link | ~Cost |
|---|---|---|
| Interlink FSR 406 (1.5" square), ×4–6 | [amazon](https://www.amazon.com/s?k=Interlink+FSR+406+force+sensitive+resistor) | $8 ea |
| Amphenol FCI Clincher connectors | [amazon](https://www.amazon.com/s?k=FSR+clincher+connector+2+pin) | $6 |

(10kΩ divider resistor is in the resistor kit.) **+$40–55**

## Variant C — Button (binary contact, simplest)

| Part | Link | ~Cost |
|---|---|---|
| EG STARTS 100mm LED dome arcade button, 5-pack | [amazon](https://www.amazon.com/s?k=EG+STARTS+100mm+dome+arcade+button+LED) | $18 |
| 3mm ABS/acrylic sheet (mounting panel) | [amazon](https://www.amazon.com/s?k=3mm+ABS+plastic+sheet+12x12) | $10 |

Wire all microswitches in parallel: every **COM → GND**, every **NO → GPIO 32**
(INPUT_PULLUP). Any press pulls the line LOW. **+$28**

**Gotchas:**
- Built-in LED is resistored for **12V**; at the ESP32's 5V it'll be dim or
  dark. Either ignore it (the WS2812B strip is your HP display) or swap the
  internal resistor for ~150Ω.
- These are ~65mm deep with the microswitch — they need a rigid panel with a
  ~98mm hole, not direct fabric mounting. Plan for plastic plates in
  front/back vest pockets.

## Variant D — Velostat (DIY pressure plate, any size)

| Part | Link | ~Cost |
|---|---|---|
| Velostat / Linqstat sheet, 11"×11" | [amazon](https://www.amazon.com/s?k=velostat+pressure+sensitive+conductive+sheet) · [adafruit](https://www.adafruit.com/product/1361) | $5 |
| Copper foil tape, **conductive adhesive** | [amazon](https://www.amazon.com/s?k=copper+foil+tape+conductive+adhesive) | $8 |
| 1mm PETG sheet 12"×12" (strike plate) | [amazon](https://www.amazon.com/s?k=PETG+sheet+1mm+12x12) | $10 |
| Metal sew-on snap fasteners | [amazon](https://www.amazon.com/s?k=metal+sew+on+snap+fasteners+heavy+duty) | $6 |
| Ring terminal crimp assortment | [amazon](https://www.amazon.com/s?k=ring+terminal+crimp+connector+assortment+22-16+awg) | $8 |
| GX12 2-pin aviation connector pairs (pad ↔ harness) | [amazon](https://www.amazon.com/s?k=GX12+2+pin+aviation+connector) | $10/5pr |
| *(alt)* Panel-mount 5.5×2.1mm barrel jack + plugs | [amazon](https://www.amazon.com/s?k=5.5x2.1mm+panel+mount+barrel+jack) | $8 |

(10kΩ divider resistor is in the resistor kit. Same readout circuit + firmware as FSR.) **+$13**

**Build a pad:** two copper-tape electrodes (comb pattern or solid patch) on
thin plastic, Velostat sheet between them, tape the sandwich shut. Solder a
lead to each copper layer. Squeeze → resistance drops → divider voltage rises.
Make pads as big as the vest panel.

## Variant E — Contact (frequency-tagged weapon → conductive shirt)

| Part | Link | ~Cost |
|---|---|---|
| LMC555 / TLC555 CMOS timer (weapon TX) | [amazon](https://www.amazon.com/s?k=LMC555+timer+IC) | $6/10 |
| LM567 tone decoder (shirt RX) | [amazon](https://www.amazon.com/s?k=LM567+tone+decoder+IC) | $7/10 |
| CR2032 + holder (weapon power) | [amazon](https://www.amazon.com/s?k=CR2032+battery+holder+with+switch) | $7 |
| Ceramic capacitor assortment kit | [amazon](https://www.amazon.com/s?k=ceramic+capacitor+assortment+kit) | $9 |
| Mini slide switch | [amazon](https://www.amazon.com/s?k=mini+SPDT+slide+switch) | $5 |
| 2N3904 transistors (optional preamp) | [amazon](https://www.amazon.com/s?k=2N3904+NPN+transistor+pack) | $5 |

(Conductive fabric/tape reused from Variant D for blade wrap + shirt.
Resistors from kit.) **+$39**

Firmware: LM567 output is active-low → use existing `MODE_BUTTON`. No
code change for single-frequency. Multi-weapon ID needs Goertzel
(separate sketch, not yet written).

## Variant F — NFC (passive tag in weapon, reader on vest)

| Part | Link | ~Cost |
|---|---|---|
| RC522 RFID reader module | [amazon](https://www.amazon.com/s?k=RC522+RFID+reader+module+arduino) | $6/2 |
| NTAG213 sticker tags 25mm, 20-pack | [amazon](https://www.amazon.com/s?k=NTAG213+NFC+sticker+25mm) | $8 |

Weapon = passive sticker, no battery. Range ~2–5 cm. UID printed on hit
= free attacker-ID. Needs `MFRC522` Arduino library. **+$14**

## Arrows — marker tips (off-pad evidence)

| Part | Link | ~Cost |
|---|---|---|
| Bingo daubers / dot markers, washable, 6-color | [amazon](https://www.amazon.com/s?k=bingo+daubers+washable+dot+markers) | $10 |
| LARP foam arrowheads (or pool-noodle stock) | [amazon](https://www.amazon.com/s?k=LARP+foam+arrow+head) | $15 |
| Hook velcro, adhesive-back (arrow tips) | [amazon](https://www.amazon.com/s?k=hook+velcro+adhesive+back+roll) | $8 |
| Loop velcro fabric / Veltex by the yard (vest face) | [amazon](https://www.amazon.com/s?k=loop+velcro+fabric+display+yard) | $12 |

**Velcro arrows + NFC:** hook velcro on the foam tip with the NTAG sticker
just beneath; loop fabric as the vest pad's outer face (over the RC522).
Arrow sticks → tag sits on the reader → UID read, no dwell race. Loop
fabric is non-conductive so it doesn't block 13.56 MHz. Also works
standalone: stuck arrow = visible hit, no electronics.

**Do NOT use a bare dauber as the arrowhead** — rigid plastic body, eye
hazard. Embed it: standard ≥2" foam mushroom head, small center hole,
dauber sponge tip protrudes ~5mm, dauber body buried in the foam/shaft.
Foam spreads the impact; sponge leaves the mark.

Complements the electronic vest: torso hits still register on the
piezo/Velostat pad; the paint dot is your evidence for limb shots and
disputes. Put a thin washable fabric layer over sensor pads so paint
doesn't gunk the copper. **+$25**

---

**Grand total for all five prototypes + arrows: ≈ $260–280.**
