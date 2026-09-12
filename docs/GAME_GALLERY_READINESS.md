# DigiPen Game Gallery readiness

This document maps Quack Kitchen 1.0.0 to the supplied DigiPen Game Gallery
requirements (last updated May 4, 2020). `tools/validate_gallery.py` protects
the source-, data-, and package-level evidence below. CI additionally performs
clean Windows and Linux Release builds, tests the staged runtime, and produces
all three distributable formats.

## Installer and packaging

| Requirement | Implementation and evidence |
|---|---|
| Installer below 500 MiB | Inno Setup output is measured and rejected at 500 MiB; staged content has the same limit. |
| Real installer and custom icons | Inno Setup 6 uses the Quack Kitchen icon for the installer, executable, and shortcuts. |
| Supported Windows version | The package targets 64-bit Windows 10 build 1809 or newer, satisfying the Windows 7-or-10 requirement. |
| DigiPen EULA | The installer displays the DigiPen text taken from the GAM300 precedent before installation. Its current official status must be confirmed before publication. |
| No reboot | The installer and mandatory Microsoft Visual C++ redistributable use no-restart behavior. |
| Release-only files | CMake stages Release binaries; the verifier rejects source, symbols, project files, editor assets, and debug libraries. |
| Correct install directories | Default is `Program Files\DigiPen\Quack Kitchen`, with a changeable destination and `DigiPen\Quack Kitchen` Start-menu group. |
| User-writable data | Saves and settings use LocalAppData on Windows and XDG data directories on Linux; no save is installed under Program Files. |
| Shortcuts and completion launch | Desktop shortcut is selected by default with opt-out; Start-menu and optional post-install launch entries are defined. |
| Clean fullscreen launch | The Windows Release target is a GUI-subsystem application and opens directly at the desktop's fullscreen resolution. |
| Uninstall, publisher, version | Inno owns installed files/shortcuts and registers DigiPen Institute of Technology with version 1.0.0. |

## Runtime behavior

| Requirement | Implementation and evidence |
|---|---|
| Title and first screen | Release title is `Quack Kitchen`; the first scene is the standalone DigiPen logo. The Linux cold-container smoke reaches its first rendered frame in under three seconds, and CI enforces the limit. |
| Logo duration and bypass | DigiPen has an isolated two-second hold. Left/right click, Enter, Space, Escape, Start, and A advance intro screens while preserving the controller notice. |
| Responsiveness and focus loss | GLFW messages continue to be processed. Losing focus/minimizing freezes time, pauses audio, releases the cursor, suppresses rendering, and waits at 10 Hz. |
| Performance | Release defaults to a 70 FPS target. Representative Windows gameplay remains a final acceptance measurement. |
| Release presentation | Release builds have no console and diagnostic/cheat shortcuts are compiled only in Debug configurations. |
| Fullscreen restoration | Fullscreen uses the current desktop video mode, so it does not replace the desktop resolution. |
| Proper game completion | Results provide restart, next-level, and main-menu choices; completion never silently exits or stalls. |
| Shutdown | Engine systems, resources, audio, window, and graphics context are released; smoke and sanitizer runs exit cleanly. |
| Network and vibration | Not applicable: the game has neither networking nor controller vibration. |

## Menus and input

| Requirement | Implementation and evidence |
|---|---|
| Main and pause menus | Both are present with understandable labels. How to Play is available from both. Credits is available from the main menu. |
| Destructive confirmations | Main-menu quit, pause-menu return to main, pause-menu quit, and operating-system close requests require confirmation. |
| Pause controls | Escape and gamepad Start toggle pause; the pause menu includes both Return to Main Menu and Quit Game. |
| Complete gamepad menu navigation | Main, level select, options, How to Play, confirmations, pause, and results accept the D-pad plus both analog sticks. |
| Keyboard and mouse coexistence | Keyboard/mouse maps remain active when a gamepad is connected, and mouse callbacks remain available on all menus. |
| Cursor behavior | Visible menu cursor is confined to the game window; gameplay hides and captures it; focus loss releases it. |
| Controller notice and hot-plug | A pre-main-menu controller notice is always shown. GLFW connection callbacks and per-frame detection support controllers attached after launch. |
| Adaptive instructions | How to Play and tutorial art switch between keyboard and controller instructions based on live connection state. |

## Credits, licenses, content, and provenance

The main-menu Credits screen names the complete team, DigiPen Singapore,
DuckEngine, FMOD/Firelight, and the other shipped libraries and asset sources.
The installed `Licenses` directory contains software/font notices and the
detailed `ASSET_PROVENANCE.md` audit. The shipped game is a non-networked,
cartoon cooking game; the content and distributed filenames contain no
identified material above an E10+ rating or unprofessional naming.

The provenance audit distinguishes team-integrated media, CC0/OFL fonts,
official marks, course-library sound effects, and audio without conclusive
embedded origin metadata. Game Gallery staff approval of applicable
online/course assets is a submission decision, not something build automation
can grant.

## Automated and final acceptance evidence

Every push to `main` runs Windows and Linux compilation/tests plus automatic
Windows installer, AppImage, and Flatpak construction. Linux CI executes the
real OpenGL path through the complete intro and into the main menu. Windows CI
validates the native executable/DLL load, runtime layout, GUI subsystem, and
version metadata; hosted Windows runners do not provide the required OpenGL
4.5 context.

Before publishing, the final installed Windows artifact should receive one
hands-on acceptance pass for wizard appearance, install/upgrade/uninstall,
fullscreen and focus behavior, all physical input routes, representative
60+ FPS gameplay, a full completion path, and the customer-arrival regression.
These are final acceptance observations, not missing implementation tasks.
