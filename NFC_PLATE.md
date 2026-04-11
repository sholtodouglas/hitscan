# NFC chest/back plate — production spec

**Design intent:** flush, deliberate hits register; glancing swipes don't.
Weapon/glove/arrow carries a passive NFC tag; vest carries reader zones.
Zone + attacker UID reported on every hit.

## Reader: Elechouse PN5321 MINI

| Spec | Value |
|---|---|
| Board | 25 × 16 × 4.4 mm |
| Antenna | **40 × 50 mm**, ferrite-backed (interference-resistant — doesn't care what's behind it) |
| Antenna cable | 10 cm, IPEX4/MHF4 or MX1.25 |
| Interfaces | SPI / I2C / UART (solder-pad select — **set to SPI**) |
| Price | $34.50 ea, $29.90 @ 10+ |
| Link | [elechouse.com/product/pn532-mini](https://www.elechouse.com/product/pn532-mini/) |

The 10 cm coax + IPEX4 connector are the fragile parts → keep them
**inside** the potted puck; run robust ribbon for the long haul.

## Architecture

```
                  hip box (dry)
   ┌──────────────────────────────────────┐
   │  ESP32  ──SPI bus──┬──SS0──► puck 0  │
   │  power bank        ├──SS1──► puck 1  │  each puck = its own
   │  buzzer / LED      ├──SS2──► puck 2  │  6-core ribbon, star
   │                    ├──SS3──► puck 3  │  topology — no mid-run
   │                    └──SS4──► puck 4  │  connectors
   └────────┬─────────────────────────────┘
            │ 5× cable glands
            ▼
   5× potted pucks on vest panels
```

**Puck** = PN5321 board + 40×50 antenna + 10 cm coax, all cast in clear
epoxy as one ~45×55×12 mm tile. Ribbon enters the epoxy and cures in
place — no exposed RF connector, no exposed solder.

## Wiring — per puck (6 conductors)

| Ribbon core | PN5321 pin | ESP32 | shared? |
|---|---|---|---|
| 1 | VCC | 3V3 | bus |
| 2 | GND | GND | bus |
| 3 | SCK | P14 | bus |
| 4 | MISO | P27 | bus |
| 5 | MOSI | P13 | bus |
| 6 | **SS** | P4 / P16 / P17 / P25 / P26 | **per-puck** |

PN532 in SPI mode doesn't need RST or IRQ pins. Set the board's
interface-select solder pads to **SPI** before potting.

Firmware: `./flash.sh vest-nfc-pn532`. Edit `NFC_SS[]` in
`firmware/vest/main.cpp` to match the SS pins you wired.

## Waterproofing

- **Puck:** cast in 2-part clear epoxy in a ~50×60 mm silicone mold.
  Antenna face flush to the mold floor (thinnest epoxy over the coil =
  best range). Board + coax + first 20 mm of ribbon all submerged.
- **Ribbon run:** PVC jacket is already waterproof. Sleeve in braided
  loom for abrasion if desired.
- **Hip box:** IP65 junction box with cable glands (one per ribbon).
- **Field-swappable variant:** instead of potting the ribbon in, fit an
  **M8 6-pin IP67 circular connector** at the puck edge. ~$6/pair.

## Layout — 5 zones

```
   chest                    back
┌─────────────┐        ┌─────────────┐
│ [0] [1] [2] │        │     [3]     │
│  L  ♥   R  │        │  shoulder   │
│             │        │     [4]     │
└─────────────┘        │   kidney    │
                       └─────────────┘
```

Each zone ≈ 40×50 mm read area. Gaps between are intentional.

## Shopping list (per vest, 5 zones + 1 spare)

| Qty | Part | Link | Cost |
|---|---|---|---|
| 6 | PN5321 MINI w/ 40×50 antenna | [elechouse](https://www.elechouse.com/product/pn532-mini/) | $207 (or $179 @ 10+) |
| 3 m | 6-conductor ribbon, 26–28AWG | [amazon](https://www.amazon.com/s?k=6+conductor+flat+ribbon+cable+28awg) | $8 |
| 1 | 2-part clear potting epoxy, 200 ml | [amazon](https://www.amazon.com/s?k=clear+electronics+potting+epoxy) | $15 |
| 1 | rectangular silicone molds ~50×60 mm | [amazon](https://www.amazon.com/s?k=rectangular+silicone+mold+50mm) | $8 |
| 1 | IP65 junction box ~130×80×50, cable glands | [amazon](https://www.amazon.com/s?k=IP65+junction+box+130x80+cable+gland) | $10 |
| 1 | adhesive-lined 3:1 heat-shrink assortment | [amazon](https://www.amazon.com/s?k=adhesive+lined+heat+shrink+3%3A1) | $8 |
| 1 | braided cable sleeve 6 mm | [amazon](https://www.amazon.com/s?k=braided+cable+sleeve+6mm) | $7 |
| *(opt)* 5 | M8 6-pin IP67 connector pairs | [amazon](https://www.amazon.com/s?k=M8+6+pin+connector+IP67) | $30 |

**≈ $260/vest** non-swappable, **≈ $290** swappable. Tags (NTAG213
stickers) you already have — 50 covers all weapons + gloves + arrows.

## Game hooks

- **Back zones (3/4):** 2× damage or instant kill — rewards flanking
- **Heart (1):** instant kill on a dagger-tagged weapon only
- **Per-weapon rules:** UID → lookup table → arrows only score on chest,
  mace ignores zone, etc.
- **Tagged gloves:** punches score + ID the puncher
- Zone-colored hit flash on the LED bar
