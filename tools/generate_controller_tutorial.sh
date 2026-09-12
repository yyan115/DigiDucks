#!/usr/bin/env sh
set -eu

repository_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
keyboard_art="$repository_root/Project/DuckEngine/Resources/Sprites/Tutorial/controls.png"
controller_glyphs="$repository_root/artwork/tutorial-controls-controller.svg"
output_art="$repository_root/Project/DuckEngine/Resources/Sprites/Tutorial/controls_controller.png"

magick "$keyboard_art" \
  -alpha on -channel A -fill black -stroke none \
  -draw 'rectangle 365,285 605,460 rectangle 425,525 550,640 rectangle 425,735 550,850 rectangle 425,935 550,1050 rectangle 425,1175 550,1290' \
  +channel \
  \( -background none "$controller_glyphs" \) \
  -compose over -composite \
  "$output_art"
