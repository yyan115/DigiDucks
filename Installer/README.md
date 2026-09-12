# Windows installer

`InstallScript.iss` follows the DigiPen installer structure and EULA used by
the sibling GAM300 project. It installs the 64-bit game under Program Files,
creates Start Menu and optional desktop shortcuts, installs the required
Microsoft Visual C++ runtime silently, offers to launch the game, and registers
a complete uninstaller.

The release workflow fills these generated directories:

- `GAMEDIRECTORY/` — the result of `cmake --install` for Windows Release.
- `REDIST/` — Microsoft's official `VC_redist.x64.exe`.
- `INSTALLER/` — the compiled Inno Setup installer.

To build it locally, stage a Windows Release build in `GAMEDIRECTORY`, download
the current Visual C++ x64 redistributable from Microsoft into `REDIST`, then
compile `InstallScript.iss` with Inno Setup 6.
