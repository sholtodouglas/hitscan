# Flashing from any computer

One-time setup on the flashing machine (Mac / Linux / Windows):

```bash
pip3 install platformio        # or: brew install platformio
```

Then from this directory:

```bash
./flash.sh blink    # smoke test — onboard LED blinks, serial prints "tick N"
./flash.sh vest     # hit-detection firmware (default)
./flash.sh weapon   # 3.6kHz blade transmitter for Variant E bench
```

Windows (no bash): `pio run -e blink -t upload && pio device monitor`

## First run: prove the board works

1. Plug ESP32 in via USB. `./flash.sh blink`
2. First build downloads the toolchain (~2 min, one-time).
3. Success = onboard blue LED blinking 1 Hz + `tick 0, tick 1, ...` in terminal.

**No serial port found?** Board likely has a CH340 USB chip:
- Mac: `brew install --cask wch-ch34x-usb-serial-driver`, replug
- Windows: install "CH340 driver" from wch.cn
- Linux: should just work; if not, add yourself to `dialout` group

## Changing sensor mode / calibration

Edit `firmware/vest/main.cpp`:
- `#define SENSOR_MODE` — `MODE_PIEZO` / `MODE_PRESSURE` / `MODE_BUTTON` / `MODE_NFC`
- `#define CALIBRATION 1` — prints raw sensor values instead of running game logic
- `HIT_THRESHOLD` — set from your calibration peaks

Then `./flash.sh vest` again.
