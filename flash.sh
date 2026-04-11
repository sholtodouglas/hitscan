#!/usr/bin/env bash
# Usage: ./flash.sh [blink|vest|weapon]   (default: vest)
set -euo pipefail
ENV="${1:-vest}"

if command -v pio >/dev/null 2>&1; then
  PIO=(pio)
else
  PIO=(python3 -m platformio)
fi

"${PIO[@]}" run -e "$ENV" -t upload
exec "${PIO[@]}" device monitor -e "$ENV"
