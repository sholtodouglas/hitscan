# Vest #1 — consolidated order

Everything to build one complete production Velostat vest (2 pads + box +
harness). **(have)** = already in your inventory from the bench order.

## Pads (×2: chest + back)

| Qty | Part | Link | Cost | |
|---|---|---|---|---|
| 1 | Velostat 11×11" sheet | [adafruit 1361](https://www.adafruit.com/product/1361) | $5 | |
| 1 yd | PUL fabric | [amazon](https://www.amazon.com/s?k=PUL+fabric+waterproof+yard) | $10 | |
| — | copper fabric | — | — | (have) |
| — | 0.5 mm PETG / thin plastic sheet | — | — | (have) |
| — | 2 mm foam | — | — | (have) |
| — | metal snap studs | — | — | (have) |
| — | ring terminals + stranded wire | — | — | (have) |
| 2 | PG7 cable gland, nylon | [amazon](https://www.amazon.com/s?k=PG7+cable+gland+nylon+5+pack) | $6 | |
| 2 | M8 2-pin pre-molded cable, 0.5 m PUR | [amazon](https://www.amazon.com/s?k=M8+2+pin+male+cable+PUR) | $14 | |
| 1 | neutral-cure RTV silicone | [amazon](https://www.amazon.com/s?k=neutral+cure+RTV+silicone+electronics) | $7 | |
| 1 | E6000 adhesive | [amazon](https://www.amazon.com/s?k=E6000+adhesive) | $6 | |
| 1 | 3/8" grommet kit + setter | [amazon](https://www.amazon.com/s?k=grommet+kit+3%2F8+inch) | $10 | |

**Pads subtotal ≈ $58**

## Hip box

| Qty | Part | Link | Cost | |
|---|---|---|---|---|
| 1 | **Pelican 1010 Micro** (clear, or 1020 for room) | [amazon](https://www.amazon.com/dp/B000VZORPI) | $15 | |
| 2 | M8 2-pin panel socket, IP67 | [amazon](https://www.amazon.com/s?k=M8+2+pin+female+panel+mount+IP67) | $8 | |
| 1 | WS2812B 16-LED ring, pre-wired | [amazon](https://www.amazon.com/s?k=WS2812B+16+LED+ring+neopixel) | $8 | |
| 1 | perfboard variety pack | [amazon](https://www.amazon.com/s?k=perfboard+prototype+board+pack) | $8 | |
| 1 | 2.54 mm female header strip (cut to 2×15) | [amazon](https://www.amazon.com/s?k=2.54mm+female+header+strip+40+pin) | $6 | |
| 1 | 2-pin screw terminal, 5 mm pitch ×4 | [amazon](https://www.amazon.com/s?k=2+pin+screw+terminal+5mm+PCB) | $5 | |
| 1 | adhesive PCB standoffs, M3, nylon | [amazon](https://www.amazon.com/s?k=self+adhesive+PCB+standoff+nylon+M3) | $8 | |
| 1 | adhesive velcro strips | [amazon](https://www.amazon.com/s?k=adhesive+velcro+strips) | $5 | |
| 1 | short USB-A → micro/C cable | [amazon](https://www.amazon.com/s?k=short+USB+cable+6+inch) | $5 | |
| — | 22Ω ½W + 1Ω resistors | — | — | (kit) |
| — | active buzzer | — | — | (have) |
| — | tactile button | — | — | (have) |
| — | ESP32 DevKit | — | — | (have) |
| — | 5000 mAh power bank | — | — | (have) |

**Box subtotal ≈ $68**

## Harness

| Qty | Part | Link | Cost |
|---|---|---|---|
| 8 | 1" side-release buckle | [amazon](https://www.amazon.com/s?k=1+inch+side+release+buckle+10+pack) | $10 |
| 4 | 1" tri-glide | [amazon](https://www.amazon.com/s?k=1+inch+triglide+slider) | $4 |
| 10 yd | 1" nylon webbing | [amazon](https://www.amazon.com/s?k=1+inch+nylon+webbing+10+yard) | $8 |

**Harness subtotal ≈ $22**

## Tools (one-time)

| Part | Link | Cost | |
|---|---|---|---|
| impulse heat sealer 12" (or use clothes iron + parchment) | [amazon](https://www.amazon.com/s?k=impulse+heat+sealer+12+inch) | $25 | |
| step drill bit set (3–12 mm) | [amazon](https://www.amazon.com/s?k=step+drill+bit+set) | $12 | |
| cordless drill | — | — | borrow/own |
| soldering iron, hot-glue gun, multimeter | — | — | (have) |

**Tools ≈ $37 one-time**

---

## Total: **≈ $148/vest** + $37 tools

(Drops to ~$110/vest at scale once tools amortize and you bulk-buy
M8/buckles/webbing. PCB run replaces perfboard at ~$1/board for 30.)

---

## Design decisions — the why-trail

| Choice | Over | Because |
|---|---|---|
| **Velostat** primary sensor | piezo, FSR | piezo saturated from jogging; FSR works but 18mm dot. Velostat = FSR-but-any-size, same jog-proof gate at low divider |
| **1Ω divider + 22Ω limiter** | 10kΩ stock | bench-tuned to hard-hit-only; 22Ω caps short-current (~140 mA vs ~3 A) |
| **PUL heat-weld pocket** | sewn, laminator, food-bag | fused seam = no needle holes, soft, immersion-grade. Laminator risks warping Velostat; food-bag is the fast prototype |
| **PETG strike plate** | bare stack | keeps face flat when pad curves to torso; spreads point hits |
| **Snap-fastener electrode leads** | solder-to-fabric | mechanical clamp on the weave — solder joints to copper fabric crack |
| **Gland through PETG tab** | M8-in-pad, RTV blob | gland clamps rigid (the tab); M8 lives downstream so the connector never touches the seal. RTV-blob is the fine-for-rain shortcut |
| **Grommets in welded margin** | loops-in-weld, separate bag | margin is solid fused PUL → hole has no leak path. Mechanically certain regardless of weld skill |
| **Pelican 1010 clear lid** | junction box + window | latch = reset/charge access without holes; clear lid = ring visible. **2 holes total** (M8×2) |
| **Ring dark-until-hit** | always-on HP bar | ~45 h battery vs ~18 h; bonus mechanic — opponents only see your HP when they land one |
| **Side-release buckle harness** | snap-hooks, JPC carrier, plate bags | no sewing, click-swap pads, lightest. JPC is the off-the-shelf fallback |
| **Adhesive standoffs** | drilled floor | Pelican floor stays unpunctured |
| **Perfboard #1 → PCB fleet** | all-perfboard | hand-wiring 30 = 25h + 30 bug chances; PCB = design once, $15/30 boards |

## Still open

- ring-arc `renderHp()` (shrinking circle + death spin) — firmware TODO
- pull-test pad #1 grommets (5 kg/corner) before making 30
- dunk-test pad #1 gland exit before making 30
- PCB layout (after vest #1 proves out)
- NFC bullseye as add-on (PN5321 spec in `NFC_PLATE.md`)
