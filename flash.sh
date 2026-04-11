#!/usr/bin/env bash
# Usage: ./flash.sh [blink|vest|weapon]   (default: vest)
# Flashes the chosen firmware then opens the serial monitor (Ctrl-C to exit).
set -euo pipefail
ENV="${1:-vest}"
pio run -e "$ENV" -t upload
exec pio device monitor -e "$ENV"
