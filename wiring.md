# Wiring

```
PIEZO BUS  (diode-OR — any number of discs, one ADC pin)

  piezo1 red ──►|──┐
  piezo2 red ──►|──┤      1N4148 per disc
  piezo3 red ──►|──┤      (stripe toward bus)
    ...           ─┤
  piezoN red ──►|──┴──┬───────┬──────────► ESP32 GPIO 32
                      │       │
                    [1MΩ]  3.3V zener
                      │   (stripe ▲)
  all piezo blacks ───┴───────┴──────────► ESP32 GND

  Only TWO wires run from vest to box: bus + GND.
  Diodes live at each disc; resistor + zener live in the box.


PRESSURE  (FSR or Velostat — set SENSOR_MODE = MODE_PRESSURE)

   3V3 ───[ FSR / Velostat pad ]───┬──────► ESP32 GPIO 32
                                   │
                                 [10kΩ]
                                   │
                                  GND

  Multiple pads: wire them in parallel across the same 3V3/GPIO32 nodes.
  Any pad pressed drops the combined resistance → divider voltage rises.

  Velostat pad stack (outside → body):

    EVA foam  (outer, optional cosmetic)
    ── thin plastic plate A ── copper tape on INNER face ── lead 1
       Velostat sheet  (cut ~5mm larger than copper on all edges)
    ── thin plastic plate B ── copper tape on INNER face ── lead 2
    EVA foam  (body padding)

  Electrodes can be copper tape OR conductive fabric. Fabric is one
  continuous conductor (no bus-strip needed) and tolerates flex better.

  Lead attachment: solder a wire to a corner before assembly. If the
  fabric won't take solder, use a metal snap fastener through it (snap
  conducts; crimp the wire to the snap stud) or sew with conductive
  thread to a wire-tail.

  Tape the perimeter to hold the sandwich; do NOT laminate tightly
  (pre-compression kills dynamic range).

  PLATE STOCK
    Strike side: ~1 mm PETG or polypropylene sheet — stiff enough to
    spread a point hit across the Velostat, flexible enough not to crack.
    Body side:   0.5 mm PET (report cover) is fine; just holds fabric flat.

  ROBUST LEAD ATTACHMENT (per electrode)
    1. Punch a metal sew-on snap through the fabric corner + plate.
    2. Crimp a ring terminal onto the stranded wire; bolt/rivet it to
       the snap stud (or solder + hot-glue).
    3. Hot-glue the first ~20 mm of wire flat to the plate — flex must
       never reach the joint.
    4. Exit the pad through a small edge notch in the plate.

  PLUG-IN PAD (field-swappable)
    Mount a GX12 2-pin panel connector on the strike-plate edge: drill
    12 mm, connector screws in with its nut. Solder cups are INSIDE the
    metal shell — electrode-A → pin 1, electrode-B → pin 2, then close
    the shell. Nothing exposed. Hot-glue the shell to the plate. Pad is
    now a sealed module with one screw-locking socket.

    Harness is the mating GX12 cable plug on stranded wire. Screw collar
    locks it on — cannot shake loose mid-fight. Broken cable = unscrew,
    swap cable, pad untouched.

    Cheaper alt: 5.5×2.1 mm barrel jack (recessed terminals on most), or
    3.5 mm audio jack with the rear lugs potted in hot glue.
    Zero-depth alt: expose the two electrode snaps on the pad edge and
    crimp mating snap sockets onto the harness wires.

  WIRE ROUTING
    Cut a shallow groove in the inner EVA foam with a hobby knife,
    press the harness cable into it, tack with hot-glue dots every
    ~50 mm. Leave a service loop at each end. Terminate at the hip box
    in a 2-pin JST (or a second 3.5 mm jack) so both ends unplug.


CONTACT  (frequency-tagged weapon vs conductive shirt — Variant E)

  WEAPON TRANSMITTER  (in the hilt; one per weapon)

    CR2032 (+) ──┬────────────── VCC (LMC555 pin 8, pin 4)
                 │
               [10k]
                 │
    ┌──── pin 7 ─┤
    │            │
    │          [15k]                f ≈ 1.44 / ((R1+2·R2)·C)
    │            │                  10k + 2·15k, 10nF → ~3.6 kHz
    ├──── pin 6 ─┼─── pin 2         (tune R2 for your target freq)
    │            │
    │          [10nF]
    │            │
    │           GND ─────────────── CR2032 (–), pin 1
    │
    └──── pin 3 ──[100nF]──[1k]──► conductive blade wrap

    Slide switch in the (+) line. Conductive wrap = copper tape or
    fabric along the foam blade's striking edge, wired to pin 3 via
    the coupling cap. ~3 V square wave, microamp-class — touch-lamp
    safety territory.

  SHIRT RECEIVER  (hip box; set SENSOR_MODE = MODE_BUTTON)

    copper shirt ──[100nF]──┬──► LM567 pin 3 (input)
                            │
                         [100k] to GND   (bias)

    LM567 timing: pin 5 → 10k → pin 6 → C to GND. Choose C so
    1/(1.1·R·C) matches the weapon frequency.
    Pin 8 (output, open-collector) ──► ESP32 GPIO 32 (INPUT_PULLUP).

    LM567 pulls pin 8 LOW when it locks onto the weapon's tone, so
    the existing MODE_BUTTON firmware path works unchanged.

    If signal is too weak to lock, add a one-transistor preamp
    (2N3904 common-emitter, ~40× gain) between the 100nF and LM567.

  NOTES
    - Return path is capacitive (attacker body ↔ defender body ↔ env);
      no wire between players. Signal level depends on contact area —
      bench-test and tune the LM567 bandwidth (pin 2 cap) before
      committing.
    - Per-weapon ID: give each weapon a distinct frequency, add one
      LM567 per frequency (or run Goertzel in firmware instead).
    - Bare hands carry no tone → won't score. Gloves with their own
      TX would fix that.


NFC  (RC522 reader, passive tag in weapon — set SENSOR_MODE = MODE_NFC)

  RC522 module          ESP32
  ────────────          ─────
    SDA / SS   ───────► GPIO 4
    SCK        ───────► GPIO 14
    MOSI       ───────► GPIO 13
    MISO       ───────► GPIO 27
    RST        ───────► GPIO 22
    IRQ        ───────► GPIO 21   (interrupt — cuts dwell from ~40ms to ~15-20ms)
    3.3V       ───────► 3V3
    GND        ───────► GND

  Weapon: glue a 25 mm NTAG213 sticker (or card-size tag) inside the
  foam tip. No battery, no wiring — completely passive.

  Range ~2–5 cm; firm contact registers, glancing tip-graze may not.
  On hit the tag UID prints to Serial — that is your attacker ID.

  Multi-zone: add more RC522s sharing SCK/MISO/MOSI; give each its own
  SS pin and poll round-robin. NOT compatible behind copper fabric (the
  shirt blocks 13.56 MHz) — mount reader in front of any conductive layer.


BUTTON  (arcade microswitch — set SENSOR_MODE = MODE_BUTTON)

  All NO terminals ──────────────────────► ESP32 GPIO 32  (INPUT_PULLUP)
  All COM terminals ─────────────────────► ESP32 GND

  No passives. Any button pressed pulls the line LOW.


PERIPHERALS
                         ESP32 DevKit
                       ┌──────────────┐
   WS2812B  DIN  ◄─────┤ GPIO 5       │
            5V   ◄─────┤ VIN / 5V     │
            GND  ◄─────┤ GND          │
                       │              │
   Buzzer   (+)  ◄─────┤ GPIO 18      │
            (–)  ◄─────┤ GND          │
                       │              │
   Button  leg1  ◄─────┤ GPIO 19      │  (INPUT_PULLUP, press = LOW)
           leg2  ◄─────┤ GND          │
                       │              │
   USB power bank ─────┤ micro-USB    │
                       └──────────────┘
```

## Notes
- Solder each 1N4148 directly to its piezo's red lead and heatshrink it — the diode rides on the disc, so only the shared bus wire runs back to the box.
- If the LED strip flickers, add a ~470Ω resistor in series on DIN and a 100–470µF cap across the strip's 5V/GND.

## Prototype vs final
- **Breadboard**: yes for bench bring-up and calibration. Half-size board + a jumper kit is plenty.
- **In the vest**: breadboard contacts will shake loose on the first hit. Move to one of:
  - solder onto **perfboard / stripboard**, or
  - an **ESP32 screw-terminal breakout** (no soldering, vibration-proof), or
  - solder wires directly to the DevKit pins and hot-glue for strain relief.
