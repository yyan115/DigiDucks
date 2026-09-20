# Quack Kitchen

![Quack Kitchen](Project/DuckEngine/Resources/Sprites/MainMenu/LOGO_1.png)

A single-player 2D cooking game by the DigiDucks team at Singapore Institute of
Technology and DigiPen Institute of Technology, on our own C++ engine.

Mum and dad are on holiday, so the restaurant is yours for five days. Take the
orders, chop, cook, plate up and serve before anyone runs out of patience.

## Download

Windows and Linux builds are on the
[releases page](https://github.com/yyan115/DigiDucks/releases):

- `Quack_Kitchen_<version>_Setup.exe` for Windows 10 and 11
- `Quack_Kitchen-<version>-x86_64.AppImage` for Linux
- `Quack_Kitchen-<version>-x86_64.flatpak` for Linux

## Build from source

CMake 3.21 or newer and a C++17 compiler.

### Linux

```sh
sudo apt-get install cmake g++ libfreetype-dev libglew-dev libglfw3-dev ninja-build
cmake --preset linux-release
cmake --build --preset linux-release --parallel
"Build/linux-release/Project/DuckEngine/Quack Kitchen"
```

On Fedora the dependencies are
`cmake ninja-build gcc-c++ freetype-devel glew-devel glfw-devel libglvnd-devel`.

### Windows

Visual Studio 2022 with the Desktop development with C++ workload, plus vcpkg
with `VCPKG_ROOT` set. From a Developer PowerShell:

```powershell
cmake --preset windows-release
cmake --build --preset windows-release --parallel
```

The game is built to `Build\windows-release\Project\DuckEngine\Quack Kitchen.exe`.

## Layout

```text
Project/DuckEngine/Engine/     Engine source
Project/DuckEngine/Game/       Gameplay source
Project/DuckEngine/Resources/  Scenes, sprites, audio, fonts, shaders
docs/                          The website
Installer/ packaging/          Windows and Linux packaging
tools/                         Build and release checks
```

## License

A DigiPen student project. All content &copy; 2025 DigiPen Institute of
Technology Singapore. Third-party notices are in [`Licenses/`](Licenses/README.md).
Made using FMOD Studio by Firelight Technologies Pty Ltd.
