#!/usr/bin/env bash
set -euo pipefail

repository_directory=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
build_directory=${1:-"$repository_directory/Build/linux-release"}
package_directory=${2:-"$repository_directory/Build/linux-package"}
linuxdeploy_command=${LINUXDEPLOY:-linuxdeploy}

app_directory="$package_directory/AppDir"
output_file="$package_directory/Quack_Kitchen-1.0.0-x86_64.AppImage"

cmake -E remove_directory "$app_directory"
cmake -E make_directory "$app_directory/usr/bin"
cmake --install "$build_directory" --prefix "$app_directory/usr/bin" --strip

install -Dm755 "$repository_directory/packaging/linux/AppRun" "$app_directory/AppRun"
install -Dm755 "$repository_directory/packaging/linux/quack-kitchen" "$app_directory/usr/bin/quack-kitchen"
install -Dm644 "$repository_directory/packaging/linux/edu.digipen.QuackKitchen.desktop" \
  "$app_directory/edu.digipen.QuackKitchen.desktop"
convert "$repository_directory/Project/DuckEngine/Resources/GameIcon.ico[8]" \
  "$app_directory/edu.digipen.QuackKitchen.png"

OUTPUT="$output_file" "$linuxdeploy_command" \
  --appdir "$app_directory" \
  --desktop-file "$app_directory/edu.digipen.QuackKitchen.desktop" \
  --icon-file "$app_directory/edu.digipen.QuackKitchen.png" \
  --executable "$app_directory/usr/bin/Quack Kitchen" \
  --output appimage

if [[ ! -s "$output_file" ]]; then
  echo "linuxdeploy did not create the expected AppImage: $output_file" >&2
  exit 1
fi

echo "Created $output_file"
