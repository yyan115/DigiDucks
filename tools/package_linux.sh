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
install -Dm644 "$repository_directory/packaging/linux/edu.digipen.quackkitchen.desktop" \
  "$app_directory/edu.digipen.quackkitchen.desktop"
install -Dm644 "$repository_directory/packaging/linux/edu.digipen.quackkitchen.metainfo.xml" \
  "$app_directory/usr/share/metainfo/edu.digipen.quackkitchen.appdata.xml"
convert "$repository_directory/Project/DuckEngine/Resources/GameIcon.ico[8]" \
  "$app_directory/edu.digipen.quackkitchen.png"

# The game is stripped by cmake --install above. linuxdeploy's bundled strip
# is too old for SHT_RELR sections used by current distributions, while the
# system libraries it copies are already stripped by their distributors.
NO_STRIP=1 LDAI_OUTPUT="$output_file" "$linuxdeploy_command" \
  --appdir "$app_directory" \
  --desktop-file "$app_directory/edu.digipen.quackkitchen.desktop" \
  --icon-file "$app_directory/edu.digipen.quackkitchen.png" \
  --executable "$app_directory/usr/bin/Quack Kitchen" \
  --output appimage

if [[ ! -s "$output_file" ]]; then
  echo "linuxdeploy did not create the expected AppImage: $output_file" >&2
  exit 1
fi

echo "Created $output_file"
