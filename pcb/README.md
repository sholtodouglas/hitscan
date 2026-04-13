# Hitscan vest controller PCB

Replaces the perfboard. Fits Pelican 1010 floor (board 50×70 mm).
ESP32 DevKit socketed; everything else through-hole for hand assembly.

## Schematic

```
                ESP32 DevKit V1 (30-pin, socketed in 2×15 female header)
   left hdr (J4)                                       right hdr (J5)
   ┌──────────┐                                        ┌──────────┐
 1 │ EN       │                                        │ GPIO23   │ 1
 2 │ VP       │                                        │ GPIO22   │ 2
 3 │ VN       │                                        │ TX0      │ 3
 4 │ GPIO34   │                                        │ RX0      │ 4
 5 │ GPIO35   │                                        │ GPIO21   │ 5
 6 │ GPIO32 ──┼── P32 net                    P19 net ──┼─ GPIO19  │ 6
 7 │ GPIO33   │                              P18 net ──┼─ GPIO18  │ 7
 8 │ GPIO25   │                              P5  net ──┼─ GPIO5   │ 8
 9 │ GPIO26   │                                        │ GPIO17   │ 9
10 │ GPIO27   │                                        │ GPIO16   │10
11 │ GPIO14   │                                        │ GPIO4    │11
12 │ GPIO12   │                                        │ GPIO2    │12
13 │ GPIO13   │                                        │ GPIO15   │13
14 │ GND ─────┼── GND net                    GND net ──┼─ GND     │14
15 │ VIN ─────┼── VIN net                    3V3 net ──┼─ 3V3     │15
   └──────────┘                                        └──────────┘

  Velostat divider (both pads in parallel):

    3V3 ──[R1 22Ω ½W]── PAD_HI ──┬── J1.1 (chest pad)
                                 └── J2.1 (back pad)
                       ┌── J1.2
              P32 ─────┼── J2.2
                       └──[R2 1Ω]── GND

  Ring LED header J3:   1=VIN  2=GND  3=P5
  Buzzer BZ1:           + → P18,  − → GND
  Reset button SW1:     a → P19,  b → GND
```

## Netlist

| Net | Nodes |
|---|---|
| 3V3 | J5.15, R1.1 |
| PAD_HI | R1.2, J1.1, J2.1 |
| P32 | J4.6, R2.1, J1.2, J2.2 |
| GND | J4.14, J5.14, R2.2, BZ1.2, SW1.2, J3.2 |
| VIN | J4.15, J3.1 |
| P5 | J5.8, J3.3 |
| P18 | J5.7, BZ1.1 |
| P19 | J5.6, SW1.1 |

## BOM / footprints

| Ref | Part | Footprint (KiCad lib) |
|---|---|---|
| J4, J5 | 1×15 female header 2.54mm | `PinSocket_1x15_P2.54mm_Vertical` |
| R1 | 22Ω **½W** axial | `R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal` |
| R2 | 1Ω ¼W axial | `R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal` |
| J1, J2 | 2-pin screw terminal 5.08mm | `TerminalBlock_bornier-2_P5.08mm` |
| J3 | 1×3 male header 2.54mm | `PinHeader_1x03_P2.54mm_Vertical` |
| BZ1 | active buzzer 12mm | `Buzzer_12x9.5RM7.6` |
| SW1 | 6mm tactile | `SW_PUSH_6mm` |
| H1–H4 | M3 mounting hole | `MountingHole_3.2mm_M3` |

## Board outline & placement (50×70 mm)

```
  (0,0) ┌──────────────────────────────────────────────┐ (70,0)
        │ H1                                        H2 │
        │   ┌─J4────────────────────────────────┐      │
        │   │         (ESP32 sits here)         │      │
        │   └─J5────────────────────────────────┘      │
        │                                              │
        │   R1  R2      BZ1     SW1     J3             │
        │                                              │
        │   [J1]  [J2]      ← screw terms, board edge  │
        │ H3                                        H4 │
 (0,50) └──────────────────────────────────────────────┘ (70,50)
```

- J4/J5 rows 25.4 mm apart (1"), centred, USB end toward J1/J2 edge
- J1/J2 at the board edge nearest the Pelican's M8-socket side
- H1–H4 at 3.5 mm inset from each corner
- Single-sided routing is feasible (all through-hole, low net count)

## Order path

**EasyEDA** (recommended — zero install, JLCPCB one-click):
1. easyeda.com → new project → place parts per BOM → wire per netlist
2. Convert to PCB → drag to placement above → autoroute → DRC
3. Fabrication Output → "Order at JLCPCB" → 5 boards ≈ $4, 30 ≈ $12

**KiCad** (if you'd rather have files): `hitscan.kicad_sch` /
`.kicad_pcb` in this dir — open, run DRC, plot Gerbers, upload to JLCPCB.

## Future revs

- NFC: add 5× 1×6 headers for PN532 SPI (SCK/MISO/MOSI shared, SS per
  reader on GPIO 4/16/17/25/26)
- swap R1/R2 to 1206 SMD if you want JLCPCB assembly
