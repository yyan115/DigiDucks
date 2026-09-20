# DigiPen Game Gallery readiness

What this repository can check by itself, and what is left to a person.

`tools/validate_assets.py` checks the game's own data: every asset a scene
names, the How to Play pages, the quit dialogue in every scene, and the options
panel in every level. `tools/verify_release.py` checks a staged build: the
licence files each package must carry, the absence of development files, and
the 500 MiB installed-size limit. Both run in CI on Linux and Windows, and the
release workflow refuses to publish a package that fails them.

`tools/validate_gallery.py` is a checklist left from an earlier approach to
this work. Several of its entries look for an implementation that was solved
differently here, so it fails on a healthy tree and is not part of CI. Read its
output as a list to argue with, not as a report.

The rest cannot be judged from the repository and is left as acceptance
testing on an installed Windows build:

- the installer and the uninstaller on Windows 10 or 11, and the EULA page
- the first screen inside three seconds on the target machine
- frame rate on real hardware
- stability across a full session, including Ctrl-Alt-Del
- proper shutdown and release of resources

One requirement is knowingly unmet: the How to Play pages show keyboard
controls only, so a player on a gamepad does not get controller art. That
needs drawing, not code.

One requirement is knowingly not followed: the game pauses when it is
minimised, but not when it merely loses focus, because a game left in view
while the player clicks another window should keep running. Reverting the
commit that made that change restores the strict behaviour.
