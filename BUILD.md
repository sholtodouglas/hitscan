# Vest #1 — step-by-step build

Every step where something seals or holds load is tagged **[SEAL]** or
**[MECH]** with what to check.

---

## A. Pad ×2 (chest, back)

### A1 — Cut

| Piece | Size | Qty/pad |
|---|---|---|
| PETG strike plate | 25×30 cm, + a 25×30 **mm** tab on one short edge | 1 |
| Copper fabric A | 23×28 cm | 1 |
| Copper fabric B | 23×28 cm | 1 |
| Velostat | 24×29 cm (overlaps copper ≥5 mm every side) | 1 |
| 2 mm foam | 25×30 cm | 1 |
| PUL | 30×35 cm | 2 |

Round every corner with scissors. Drill an **8 mm hole** centred in the
PETG tab.

### A2 — Electrode leads

Per copper sheet:
1. Push a metal **snap stud** through one corner from the front; fold
   the prongs flat on the back. **[MECH]** — tug the snap; it shouldn't
   spin or pull out.
2. Crimp a **ring terminal** onto ~12 cm of stranded wire. Fit the ring
   over the snap post; peen the post over (or add a tiny nut).
   **[MECH]** — tug the wire; ring shouldn't slip off the post.
3. Hot-glue the first 20 mm of wire flat to the fabric so flex never
   reaches the joint.

Do copper A's snap in the **top-left** corner, copper B's in the
**bottom-right** — offset so neither snap sits over the other.

### A3 — Stack and tack

1. Lay PETG plate, tab pointing toward where the cable will exit.
2. Two small dots of E6000 on the PETG → press copper A on (snap up).
3. Two dots on copper A's far corners → press Velostat on.
4. Two dots on Velostat's far corners → press copper B on (snap up).
5. Route both snap-lead wires toward the PETG tab.

**[CHECK]** — DMM Ω across the two wires, stack on the table untouched:
should read **>10 kΩ**. Press the centre hard: should drop to **<100 Ω**.
If it reads <1 kΩ at rest → copper is touching past the Velostat
somewhere; re-stack.

### A4 — M8 socket in the tab

0. **E6000 the outer PUL sheet to the PETG tab** across the whole tab
   area; clamp flat, cure overnight. PUL + tab become one piece — PUL
   can't slip out from under the O-ring later. **[MECH]**
1. Punch the bonded PUL to 8 mm right over the tab hole (so the barrel
   passes *through* the PUL, not just under the flange). From the
   **outside**, push the M8 socket's threaded barrel through PUL + tab. The flange + its
   **O-ring** sit flat on the outside face. **[SEAL]** — the O-ring is
   the primary seal; make sure it's seated in its groove, not pinched.
2. From the inside, spin the **locknut** onto the threads. Tighten firm
   (finger-tight + ¼ turn with pliers). **[MECH]** — socket shouldn't
   rotate in the hole.
3. Run a thin **RTV bead** around the flange edge on the outside.
   **[SEAL]** — secondary; let it skin over before handling (~15 min).
4. Inside: socket has 3 pre-wired leads (brown/blue/black). Splice
   snap-lead-A → **brown**, snap-lead-B → **blue** (solder + heat-
   shrink). Snip black off. **[MECH]** — tug each splice.

### A5 — PUL pocket

1. Lay one PUL sheet **shiny (PU) side up**. Place the stack on it,
   PETG-side down, foam on top. The M8 socket's flange faces down
   through a hole you punch in this PUL sheet (8 mm, aligned with the
   tab hole) — the socket barrel passes through PUL + tab; flange and
   O-ring end up against the **outer PUL face**.

   *Re-do A4 if you mounted the socket before the PUL — easier to do
   PUL → tab → socket in one go: punch PUL, lay PUL over tab, push
   socket through both, nut inside.*

2. Lay the second PUL sheet on top, **shiny side down** (PU faces meet).
3. **Heat-weld** all four edges, ~20 mm in from the PUL edge, leaving a
   solid fused margin. Iron on **low** through baking parchment, slow
   pass, ~5 s/inch; or impulse sealer. **[SEAL]** — the weld is PU
   fused to PU; peel-test a scrap first (should tear the PUL before
   the weld lets go). Weld goes **around** the tab/socket area — the
   socket's O-ring + RTV is the seal there, not the weld.
4. **[CHECK]** — pocket should be slightly loose around the stack; if
   it's drum-tight you've pre-compressed the Velostat.

### A6 — Grommets

1. Punch a 3/8" grommet through each corner of the **welded margin**
   (the solid fused border, ~10 mm from the edge — NOT over the
   sensor stack). **[MECH]** — set the grommet with the tool until the
   flange is flat; tug it, shouldn't spin.
2. **[SEAL]** — none needed; the margin is solid (no cavity), so the
   hole has no path into the pocket.

### A7 — Pad tests

- **Continuity:** DMM Ω across the M8 socket's pins. Rest >10 kΩ, hard
  press <100 Ω. Same as A3 but through the finished connector.
- **Pull:** hang ~5 kg (gallon of water) from each grommet for 1 min.
  **[MECH]** — no tearing, grommet doesn't pull through.
- **Dunk:** RTV cured 24 h. Submerge the whole pad in a sink for 5 min,
  weight it down. Dry the outside, then DMM Ω across the pins again —
  if it reads <1 kΩ at rest, water got in. **[SEAL]** — investigate
  the socket area or weld; re-RTV or re-weld.

Repeat A1–A7 for the second pad.

---

## B. Perfboard

### B1 — Layout (50×70 mm board)

```
  ┌──────────────────────────────────────┐
  │  [hdr ×15]              [22Ω]─┬─[1Ω] │
  │                               │      │
  │     ESP32 socket         P32──┘      │
  │                                      │
  │  [hdr ×15]    [buzz]  [btn]  [T][T]  │
  │                              ↑   ↑   │
  │  ○                  screw terminals ○│ ← M3 holes
  └──────────────────────────────────────┘
```

### B2 — Solder

1. Two **15-pin female header** rows, spaced for the ESP32 (measure your
   board — usually 25.4 mm / 1" between rows). **[MECH]** — seat ESP32,
   check it's snug.
2. **22Ω ½W** and **1Ω** end-to-end; their junction wires to the P32
   header pin. 22Ω free end → 3V3 header pin. 1Ω free end → GND.
3. **Buzzer**: + → P18, − → GND.
4. **Tactile button**: one leg → P19, other → GND.
5. Two **2-pin screw terminals**. Both terminal-pin-1 → the 22Ω/pad
   node (3V3 side of the divider); both terminal-pin-2 → the P32 node.
   (Pads in parallel.)
6. **3-pin header** for the ring LED: VIN, GND, P5.
7. Drill 3 mm holes in two opposite corners for standoffs.

### B3 — Bench test

Seat the ESP32. USB in. `./flash.sh vest-game`. Short the screw-terminal
pins with a resistor (~100 Ω) to simulate a hit → beep + onboard blue
LED. **[CHECK]** — proves the perfboard wiring before it goes in the box.

---

## C. Pelican box

### C1 — Drill

Two **8 mm holes** in one short side, ~20 mm apart, centred vertically.
Step-drill bit, slow, let the bit cut. Deburr the inside edge.

### C2 — Mount M8 sockets

Per hole: socket barrel through from **outside**, O-ring against the
outer wall, locknut inside, tighten. **[SEAL]** — O-ring is the seal;
RTV bead around the flange optional. **[MECH]** — socket shouldn't spin.

### C3 — Mount perfboard

1. Clean the Pelican floor with isopropyl.
2. Peel-and-stick **two adhesive standoffs** where the board's corner
   holes will land. Press firm, wait 10 min. **[MECH]** — try to twist
   one off; it shouldn't budge.
3. Screw the perfboard onto the standoffs (M3 × 6 mm).

### C4 — Ring LED in the lid

1. Hot-glue the ring to the **inside** of the clear lid, LEDs facing
   out. **[MECH]** — let cool, tug.
2. Three wires from the ring → the 3-pin header on the perfboard.
   Leave enough slack to open the lid fully.
3. **[SEAL]** — none; nothing penetrates the lid.

### C5 — Wire sockets → perfboard

Short wire pair from each M8 socket's solder cups → the screw terminals.
**[MECH]** — tug each; tighten terminal screws.

### C6 — Power

Velcro strip on floor + on power bank. Short USB cable bank → ESP32.
**[MECH]** — bank shouldn't slide when you shake the box.

### C7 — Box test

Lid latched. Submerge the M8-socket end (with dust caps on, or cables
mated) for 5 min. **[SEAL]** — open, check dry inside.

---

## D. Harness

### D1 — Grommet → buckle (×8, one per pad corner)

1. Thread a **20 mm D-ring** through the grommet.
2. 10 cm of webbing: fold in half through the D-ring, both tails into
   the **female** half of a side-release buckle. Melt cut ends.
**[MECH]** — yank the buckle; webbing shouldn't slip through buckle or
D-ring.

### D2 — Straps (×4: 2 shoulder, 2 side)

~50 cm webbing each. **Male** buckle on each end (thread through the
buckle's bar, fold back through the tri-glide). Tri-glide mid-strap
for length. Melt all cut ends.

### D3 — Assemble

Clip shoulder straps front-top ↔ back-top. Clip side straps
front-bottom ↔ back-bottom. Pelican carabiner onto one side strap.

---

## E. System bring-up

1. M8 cables: pad socket ↔ Pelican socket, both pads. Screw collars
   tight. **[SEAL]** — IP67 when fully mated.
2. `./flash.sh vest-game` (with `-DNUM_LEDS=16` for the ring).
3. Put it on. Jog 30 s — should see **zero** hits. Slap each pad — beep
   + ring shows HP for 1 s. Five hits → death strobe → auto-respawn.
4. Adjust `HIT_THRESHOLD` in `platformio.ini` if jog triggers or slaps
   don't.

---

## Seal & mech audit

| Penetration | Sealed by | Check |
|---|---|---|
| Pad: M8 socket | O-ring + RTV | A7 dunk |
| Pad: PUL edges | heat-weld | A7 dunk |
| Pad: grommets | (solid margin, no cavity) | n/a |
| Box: 2× M8 socket | O-ring (+ RTV) | C7 dunk |
| Box: lid | factory gasket | C7 dunk |
| Cable: M8 mated | screw-collar O-ring | — |

| Load joint | Held by | Check |
|---|---|---|
| Snap → copper | folded prongs | A2 tug |
| Ring terminal → snap | peened post | A2 tug |
| M8 socket → tab | locknut | A4 spin |
| Grommet → PUL | crimped flange | A7 5 kg pull |
| Buckle → webbing | friction fold + melted end | D1 yank |
| Perfboard → box | adhesive standoff | C3 twist |
