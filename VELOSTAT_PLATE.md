# Velostat hit plate — production spec

Two laminated pads (chest, back) → IP65 hip box (ESP32, ring LED, buzzer,
divider). Force-gated, jog-proof, rain-proof. ~$95/vest.

## Pad construction

### Stack

```
  PUL outer skin  ──────────────────────────────  strike face
    copper fabric A   (snap → lead 1, LEFT corner)
    Velostat          (overlaps both coppers ≥5mm every side)
    copper fabric B   (snap → lead 2, RIGHT corner)
  PUL inner skin  ──────────────────────────────  body side
```

Copper A and B are **offset** so each snap corner sits over Velostat
only, never over the other copper — no short path through a snap.

### Electrode leads (do this BEFORE stacking)

Per copper sheet:
1. Punch a metal snap **stud** through one corner of the fabric. Prongs
   fold over on the back.
2. Crimp a ring terminal onto ~12 cm of stranded wire; fit it over the
   snap post; peen the post or add a small nut.
3. Hot-glue the first 20 mm of wire flat to the fabric — flex must never
   reach the joint.

Snap = mechanical clamp on the fabric weave. No solder-to-fabric (which
cracks). Snaps stay **inside** the pocket; nothing pierces the PUL.

### Assembly

1. Stack copper A / Velostat / copper B. Two **tiny** dots of fabric
   glue at the non-snap corners, copper→Velostat, to stop sliding.
   Don't coat the face — Velostat needs direct contact to work.
2. Round every copper corner with scissors (sharp corners punch through).
3. Route both lead wires to the same edge.
4. Heat-weld a **gland plate** (25×25 mm PETG, 7 mm hole) to the inside
   of one PUL sheet at that edge — iron on low through parchment, or
   E6000 glue. This gives the gland a rigid surface to clamp.
5. Fit a **PG7 cable gland** through the plate + PUL. Feed both lead
   wires + a 20 cm 2-core pigtail through; solder leads to pigtail
   inside the pocket; tighten the gland's compression nut on the
   pigtail. RTV bead around the plate-to-PUL edge.
6. Lay the second PUL sheet over, **heat-weld all four edges** (impulse
   sealer, or iron on low through parchment, PU sides together). Leave
   the pocket *slightly* loose — pre-compression kills dynamic range.
7. Crimp/solder an **M8 2-pin plug** onto the pigtail end.

### Why gland-plus-pigtail, not M8-in-pad

A panel-mount M8 through flexible PUL relies on an RTV bead that flexes
every hit — it'll crack eventually. The gland is purpose-built: rubber
compression seal grips the round cable, threaded body clamps a rigid
plate. The M8 connector lives 20 cm downstream where it can't affect the
pad seal. Dead pad = unscrew M8, swap pad, harness untouched.

### Dunk test

Before trusting it: submerge the gland corner in a bowl for 5 min,
weight it down. Pull out, dry the outside, open a weld corner 1 cm and
check inside for damp. Re-weld. Do this once per design rev, not per pad.

## Hip box

IP65 junction box ~130×80×50 mm with cable glands. Contains ESP32, power
bank, buzzer, divider network. **WS2812B 16-LED ring** behind a 40 mm
clear acrylic window in the lid.

### Divider — with current limiter

```
 3V3 ──[22Ω ½W]──┬──[ chest pad ]──┐
                 └──[ back pad  ]──┤
                                   ├── P32 (ADC)
                                 [1Ω]
                                   │
                                  GND
```

The 22Ω caps short-circuit current at ~140 mA (vs ~3 A without it —
which cooks the regulator). Negligible effect on readings since
22Ω ≪ idle-Velostat kΩ. **Don't skip it.**

Zone-aware variant: separate divider per pad → P32 / P33.

## Shopping list (per vest, 2 pads + box)

| Qty | Part | Link | Cost |
|---|---|---|---|
| 1 | Velostat 11×11" | [adafruit 1361](https://www.adafruit.com/product/1361) | $5 |
| 1 yd | PUL fabric | [amazon](https://www.amazon.com/s?k=PUL+fabric+waterproof+yard) | $10 |
| 0.5 m | copper fabric | (have) | — |
| — | metal snap studs, ring terminals, stranded wire | (have) | — |
| 2 | PG7 cable gland | [amazon](https://www.amazon.com/s?k=PG7+cable+gland+nylon) | $4 |
| 1 | PETG sheet scrap (gland plates) | (have) | — |
| 2 | M8 2-pin plug + socket pair, IP67 | [amazon](https://www.amazon.com/s?k=M8+2+pin+connector+IP67) | $10 |
| 1 m | 2-core cable 24AWG | [amazon](https://www.amazon.com/s?k=2+core+cable+24awg) | $5 |
| 1 | neutral-cure RTV silicone | [amazon](https://www.amazon.com/s?k=neutral+cure+RTV+silicone+electronics) | $7 |
| 1 | E6000 / fabric glue | [amazon](https://www.amazon.com/s?k=E6000+adhesive) | $6 |
| 1 | IP65 junction box ~130×80×50 + glands | [amazon](https://www.amazon.com/s?k=IP65+junction+box+130x80+cable+gland) | $10 |
| 1 | WS2812B 16-LED ring, pre-wired | [amazon](https://www.amazon.com/s?k=WS2812B+16+LED+ring+neopixel) | $8 |
| 1 | 40 mm clear acrylic disc (lid window) | [amazon](https://www.amazon.com/s?k=40mm+clear+acrylic+disc) | $5 |
| — | 22Ω ½W + 1Ω ¼W resistors | (kit) | — |
| *(tool)* | impulse sealer 12" — or use a clothes iron | [amazon](https://www.amazon.com/s?k=impulse+heat+sealer+12+inch) | $25 |

**≈ $95/vest** (sealer is one-time).

## Firmware

`./flash.sh vest-game`. Before production flash: `-DNUM_LEDS=16` for the
ring. Ring-arc `renderHp()` (shrinking circle, red death-spin) — TODO.
