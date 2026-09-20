# Quack Kitchen

![Quack Kitchen](Project/DuckEngine/Resources/Sprites/MainMenu/LOGO_1.png)

Quack Kitchen is a single-player 2D cooking game built by the DigiDucks team
at DigiPen Institute of Technology Singapore. Take orders, prepare each dish,
and keep a growing line of customers happy before the day ends.

The game and its custom C++ engine use OpenGL, GLFW, GLEW, FreeType, and FMOD.
Windows and Linux are the supported platforms.

**[Play it](https://yyan115.github.io/DigiDucks/)** — the site has the
downloads for both platforms.

## Play

- Move with `WASD`, the left analog stick, or the directional pad.
- Pick up or place an item with `J` / gamepad `X`.
- Use a station with `K` / gamepad `A`.
- Pause with `Esc` / gamepad `Start`.
- `Esc` also closes whatever is open on top: the journal, the options panel,
  or a confirmation, one layer at a time. Gamepad `B` does the same.
- Toggle fullscreen with `Alt+Enter`.

The How to Play pages show the keyboard controls. Both inputs work everywhere,
including the menus, and a gamepad can be connected while the game is running.

## Download

The release workflow produces three installable packages, each named after the
release version:

- `Quack_Kitchen_<version>_Setup.exe` — Windows 10/11 installer.
- `Quack_Kitchen-<version>-x86_64.AppImage` — portable Linux application.
- `Quack_Kitchen-<version>-x86_64.flatpak` — Linux Flatpak bundle.

Every published build is on the
[releases page](https://github.com/yyan115/DigiDucks/releases), and the
[website](https://yyan115.github.io/DigiDucks/) links the latest one.

## Build from source

Clone the repository normally. CMake 3.21 or newer and a C++17 compiler are
required.

### Linux

On Ubuntu, install the native build dependencies:

```sh
sudo apt-get install cmake g++ libfreetype-dev libglew-dev libglfw3-dev ninja-build
```

Then configure, build, and test the Release preset:

```sh
cmake --preset linux-release
cmake --build --preset linux-release --parallel
ctest --test-dir Build/linux-release --output-on-failure
```

Run the staged build directly:

```sh
"Build/linux-release/Project/DuckEngine/Quack Kitchen"
```

#### Fedora KDE and VS Code

Install the native build and debugging dependencies:

```sh
sudo dnf install cmake ninja-build gcc-c++ gdb freetype-devel glew-devel glfw-devel libglvnd-devel
```

Open the repository root in VS Code and install the recommended Microsoft
**CMake Tools** and **C/C++** extensions. Press `F5` to configure, build, and
debug the game, or press `Ctrl+F5` to run it without stopping in the debugger.
The default `Ctrl+Shift+B` build task uses the same `linux-debug` CMake preset.

The VS Code setup is only an interface over the Linux CMake build. It does not
modify or replace the existing Visual Studio solution or project files.

To create an AppImage, install ImageMagick and provide a `linuxdeploy`
executable:

```sh
LINUXDEPLOY=/path/to/linuxdeploy-x86_64.AppImage tools/package_linux.sh
```

### Windows

Install Visual Studio 2022 with the Desktop development with C++ workload,
CMake, Ninja, and vcpkg. Bootstrap vcpkg and expose its directory as
`VCPKG_ROOT`, then run these commands from a Developer PowerShell:

```powershell
cmake --preset windows-release
cmake --build --preset windows-release --parallel
ctest --test-dir Build/windows-release --output-on-failure
```

The executable is generated at
`Build\windows-release\Project\DuckEngine\Quack Kitchen.exe`.

The original Visual Studio solution remains at
`Project/DuckEngine/DuckEngine.sln` for legacy development. CMake is the
supported path for reproducible release builds.

## Packaging and verification

- `tools/validate_assets.py` checks serialized and source-code asset paths,
  including case-sensitive Linux paths.
- `GAME_GALLERY_READINESS.md` says which DigiPen Game Gallery requirements
  the repository can settle and which are left to testing on an installed
  Windows build. `tools/validate_gallery.py` is a checklist kept from an
  earlier approach to that work: several of its entries look for an
  implementation this game solved differently, so it is not run by CI and its
  output is not a regression report.
- `tools/verify_release.py` checks staged Windows/Linux runtime contents,
  licensing files, development-file exclusions, and the 500 MiB installed-size
  limit.
- `Installer/InstallScript.iss` defines the DigiPen-style Inno Setup package.
- `packaging/linux/` contains the AppImage launch files and Flatpak manifest.
- `.github/workflows/ci.yml` builds, tests, stages, verifies, and smoke-tests
  both supported operating systems.
- `.github/workflows/release.yml` builds all three release packages on every
  `main` push. A manual run can also publish them under a requested version
  tag after the package jobs pass.

## Versioning and releases

The release version is declared in exactly one place, the `project` command in
`Project/DuckEngine/CMakeLists.txt`. The Windows executable resource, the Inno
Setup installer, the AppImage filename, and the AppStream metadata are all
generated from it, so no second copy can drift out of date.
`tools/project_version.py` prints the declared version for scripts that need it.

To cut a release:

1. Change the version in `Project/DuckEngine/CMakeLists.txt` and commit it.
2. Tag the commit as `v<version>` and push the tag.

Pushing the tag runs the release workflow, which refuses to continue if the tag
and the declared version disagree, then builds, verifies, and publishes all
three packages. Pushes to `main` build and verify the same packages without
publishing them.

## Repository layout

```text
docs/            The game's website, served by GitHub Pages
Project/DuckEngine/
├── Engine/       Custom engine source and bundled SDK files
├── Game/         Quack Kitchen gameplay source
└── Resources/    Runtime scenes, sprites, audio, fonts, and shaders
Installer/        Windows installer definition
packaging/        Linux and Windows packaging metadata
tools/            Asset, release, and packaging checks
Licenses/         Third-party notices and shipped-asset provenance
```

The editor is intentionally outside the current release scope and is retained
as a possible future project.

## Licenses and attribution

Quack Kitchen is a DigiPen student project. Third-party software, font, and
asset notices are documented in [`Licenses/`](Licenses/README.md), with the
runtime media audit in
[`Licenses/ASSET_PROVENANCE.md`](Licenses/ASSET_PROVENANCE.md).
