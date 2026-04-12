# Velostat hit plate — production spec

Two laminated pads (chest, back) → IP65 hip box (ESP32, ring LED, buzzer,
divider). Force-gated, jog-proof, rain-proof. ~$95/vest.

## Pad construction

### Stack (outside → body)

```
  PUL outer skin  ──────────────────────────────  strike face
    0.5 mm PETG sheet   ← strike plate: keeps form, spreads point hits
    copper fabric A     (snap → lead 1, LEFT corner)
    Velostat            (overlaps both coppers ≥5mm every side)
    copper fabric B     (snap → lead 2, RIGHT corner)
    2 mm foam           ← optional, body comfort only
  PUL inner skin  ──────────────────────────────  body side
```

The PETG keeps the face flat (no wrinkles when the pad curves to a torso)
and spreads an off-center hit across more Velostat. Cut it same size as
copper A, round corners, two glue dots to copper A. Foam is comfort, not
function — never put foam *between* copper and Velostat.

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
5. Fit an **M8 2-pin panel socket** through the plate + PUL: 8 mm hole,
   socket's threaded barrel through, locknut on the inside clamps the
   tab. Socket's O-ring seals against the outside face; RTV bead for
   paranoia. Solder snap-lead-A → pin 1, snap-lead-B → pin 2 inside. Feed both lead
   wires + a 20 cm 2-core pigtail through; solder leads to pigtail
   inside the pocket; tighten the gland's compression nut on the
   pigtail. RTV bead around the plate-to-PUL edge.
6. Lay the second PUL sheet over. **Heat-weld all four edges** (impulse
   sealer, or iron on low through parchment, PU sides together), leaving
   a **~20 mm fused margin** past the sensor stack on every side. Keep
   the pocket *slightly* loose — pre-compression kills dynamic range.
7. **Punch a grommet** through each corner of the welded margin (3/8"
   eyelet). The margin is solid fused PUL — no cavity — so the hole has
   no leak path into the pocket. Straps clip here.
8. Pad now has a flush M8 socket. A **male-to-male M8 cable** (plug
   both ends, 1 m) connects pad ↔ Pelican. Cable is the wear part —
   damaged cable = unscrew both ends, swap, done.

### Why M8 socket through the rigid tab

The PETG tab gives the socket a rigid wall to clamp (flange + nut),
same as the Pelican side. The earlier worry about M8-through-PUL was
the *flexible* PUL — the tab fixes that. Cable is now fully swappable
at both ends: damaged cable, dead pad, or dead box are all independent
unscrew-and-replace.

### Dunk test

Before trusting it: submerge the gland corner in a bowl for 5 min,
weight it down. Pull out, dry the outside, open a weld corner 1 cm and
check inside for damp. Re-weld. Do this once per design rev, not per pad.

## Hip box

**Enclosure:** **Pelican 1010 Micro** (or 1020 for more room) — IP67,
clear lid, tool-free latch, carabiner loop.
[amazon](https://www.amazon.com/dp/B000VZORPI). Interior ~111×73×43 mm.

**Only two holes, both in one short side:**
- 2× **M8 2-pin panel sockets** (chest pad, back pad). Drill 8 mm,
  fitting's O-ring seals against the outside, nut on the inside.

**Everything else is inside — pop the latch to access:**
- 16-LED WS2812 ring hot-glued to the inside of the clear lid. Dark
  until a hit lands → shows HP arc for the cooldown → black. ≈45 h
  battery.
- Reset button on the perfboard (open lid, press).
- Charging: open lid, plug power bank in.
- Carabiner loop → belt clip. Done.

**Inside:**
```
┌──────────────────────────────────────┐
│ ┌──────────┐  ┌──────────────────┐  │
│ │ power    │  │ perfboard:       │  │
│ │ bank     │  │  ESP32 (socketed)│  │
│ │ 5000mAh  │  │  22Ω + 1Ω divider│  │
│ │          │  │  buzzer          │  │
│ └────┬─────┘  └────┬─────────────┘  │
│      └─USB────────►│                 │
│                                      │
│  M8-chest  M8-back  USB-C-charge    │ ← short side
└──────────────────────────────────────┘
        lid: ring LED + reset btn
```

ESP32 on female header (socketed, swappable). Perfboard screwed to two
standoffs. Power bank velcro'd to the floor. Short USB-A→C cable from
bank to ESP32.

**Mounting to body:** carabiner loop → belt or harness D-ring.

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
| 2 | M8 2-pin female panel socket (pad) | [amazon](https://www.amazon.com/s?k=M8+2+pin+female+panel+mount+IP67) | $8 |
| 1 | PETG sheet scrap (gland plates) | (have) | — |
| 2 | M8 2-pin male-to-male cable, 1 m | [amazon](https://www.amazon.com/s?k=M8+2+pin+male+to+male+cable+1m) | $16 |
| 1 | neutral-cure RTV silicone | [amazon](https://www.amazon.com/s?k=neutral+cure+RTV+silicone+electronics) | $7 |
| 1 | E6000 / fabric glue | [amazon](https://www.amazon.com/s?k=E6000+adhesive) | $6 |
| 1 | 3/8" grommet kit + setter | [amazon](https://www.amazon.com/s?k=grommet+kit+3%2F8+inch) | $10 |
| 1 | **Pelican 1010 Micro** (or 1020) | [amazon](https://www.amazon.com/dp/B000VZORPI) | $15 |
| 1 | WS2812B 16-LED ring, pre-wired | [amazon](https://www.amazon.com/s?k=WS2812B+16+LED+ring+neopixel) | $8 |
| 2 | M8 2-pin panel socket (box side) | [amazon](https://www.amazon.com/s?k=M8+2+pin+female+panel+mount+IP67) | $8 |
| 2 | M8 2-pin pre-molded sensor cable, 0.5 m PUR | [amazon](https://www.amazon.com/s?k=M8+2+pin+male+cable+PUR) | $14 |
| — | 22Ω ½W + 1Ω ¼W resistors | (kit) | — |
| *(tool)* | impulse sealer 12" — or use a clothes iron | [amazon](https://www.amazon.com/s?k=impulse+heat+sealer+12+inch) | $25 |

**≈ $95/vest** (sealer is one-time).

## Wear layer

**Straps clip directly to the pad** via side-release buckles. Each
grommet gets a short webbing loop (~8 cm folded) with a **female**
buckle half. Each strap has a **male** buckle on both ends + tri-glide
for length. Click in; squeeze to release.

- 2× shoulder straps: front-pad top → over shoulders → back-pad top
- 2× side straps: front-pad bottom → around ribs → back-pad bottom
- Don/doff: unclip the two side buckles, lift over head
- Pad swap: squeeze 4 buckles
- Pelican carabiners onto a side strap at the hip

| Qty | Part | Link | Cost |
|---|---|---|---|
| 8 | side-release buckle 1" | [amazon](https://www.amazon.com/s?k=1+inch+side+release+buckle+10+pack) | $10 |
| 4 | tri-glide 1" | [amazon](https://www.amazon.com/s?k=1+inch+triglide+slider) | $4 |
| 4 m | 25 mm nylon webbing (10 yd roll does ~3 vests) | [amazon](https://www.amazon.com/s?k=1+inch+nylon+webbing+10+yard) | $8 |

**~$22.** No sewing — webbing threads through buckles/tri-glides; melt
cut ends with a lighter. Lightest possible; pad swap = unclip 4 hooks.

*Off-the-shelf alt:* JPC-style carrier (Yakeda ~$30, IDOGEAR ~$55) —
pads slide into its plate bags. Heavier, more durable, zero assembly.

## Firmware

`./flash.sh vest-game`. Before production flash: `-DNUM_LEDS=16` for the
ring. Ring-arc `renderHp()` (shrinking circle, red death-spin) — TODO.
