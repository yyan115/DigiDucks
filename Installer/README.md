# Windows installer

`InstallScript.iss` follows the DigiPen installer structure and EULA used by
the sibling GAM300 project. It installs the 64-bit game under Program Files,
creates Start Menu and optional desktop shortcuts, installs the required
Microsoft Visual C++ runtime silently, offers to launch the game, and registers
a complete uninstaller.

The release workflow fills these generated paths:

- `GAMEDIRECTORY/` — the result of `cmake --install` for Windows Release.
- `REDIST/` — Microsoft's official `VC_redist.x64.exe`.
- `INSTALLER/` — the compiled Inno Setup installer.
- `Version.iss` — the `GameVersion` define read by `InstallScript.iss`.

`InstallScript.iss` also copies `INSTALLERFILES/DigiPen_EULA.txt` into the
install directory. That is not the only copy any more: CMake installs the same
file into `Licenses/` for every package, because the Linux ones carried no EULA
at all. Removing the CMake line would take it off Linux; removing this one
would change nothing, since both write the same bytes to the same place.

The installer never stores its own version. `Version.iss` is generated from the
version declared in `Project/DuckEngine/CMakeLists.txt`, which is also the
version compiled into the executable, so the two cannot disagree.

To build it locally, stage a Windows Release build in `GAMEDIRECTORY`, download
the current Visual C++ x64 redistributable from Microsoft into `REDIST`,
generate the version define, then compile the script with Inno Setup 6:

```powershell
python tools/project_version.py --installer-define |
  Set-Content Installer\Version.iss -Encoding ASCII
& "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" Installer\InstallScript.iss
```
