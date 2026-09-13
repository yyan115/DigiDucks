# Quack Kitchen licenses

This directory is installed beside the game executable and records the licenses
and provenance for software, fonts, and externally sourced assets shipped with
Quack Kitchen.

## Software

- `FMOD.txt` — FMOD Engine 2.03.09 notice and governing terms.
- `Dear-ImGui.txt` — Dear ImGui 1.91.1 (MIT).
- `Nlohmann-JSON.txt` — JSON for Modern C++ 3.11.3 (MIT).
- `FreeType.txt` and `FreeType-FTL.txt` — FreeType.
- `GLEW.txt` — OpenGL Extension Wrangler Library.
- `GLFW.txt` — GLFW.
- `GLM.txt` — OpenGL Mathematics.
- `stb_image.txt` — stb_image.
- `Brotli.txt`, `bzip2.txt`, `libpng.txt`, and `zlib.txt` — transitive
  FreeType dependencies used by release builds.
- `libXau.txt`, `libXdmcp.txt`, `libbsd.txt`, and `libmd.txt` — libraries the
  Linux packages carry inside them. The AppImage bundles the shared libraries
  the game links against, and the Flatpak is built from the same directory, so
  both redistribute these four and both reproduce their notices here.

Which libraries end up bundled depends on the machine the Linux packages are
built on, so this list describes the released packages rather than every
possible build. The Windows installer bundles none of them. Anything else the
operating system or the Flatpak runtime supplies is governed by the licenses
that come with it.

## Fonts and assets

- `Fonts.txt` — Kenney fonts (CC0) and the two Mikado faces, whose licensing
  is unestablished. No Inter font is shipped.
- `Sound-Ideas.txt` — course-library provenance and usage notice.
- `ASSET_PROVENANCE.md` — audit record for the shipped media.

