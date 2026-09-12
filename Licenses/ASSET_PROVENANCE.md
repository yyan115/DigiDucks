# Shipped asset provenance

Audit date: 2026-09-12

This record is based on the current release tree, embedded metadata, and Git
history. It documents evidence; it does not create or expand any license.

## Visual assets

- Gameplay art, character animation, UI, backgrounds, cutscenes, food, and
  station sprites were introduced through commits by named DigiDucks team
  contributors. No third-party stock-pack attribution or origin metadata was
  found in those files.
- `Resources/Sprites/Intro/DigiPen_Singapore_WEB_WHITE.png` is the official
  institutional logo used for the required startup screen.
- `Resources/Sprites/Intro/fmod_logo_white.png` is the official FMOD mark used
  for required attribution.
- The controller notice and controller-specific instruction image were made
  for this project. Their editable sources are retained under `artwork/`.
- No Kenney sprite or texture is present in the shipped runtime resources.

## Fonts

- The Kenney font package is distributed under CC0; its bundled notice is in
  `Fonts.txt`.
- Inter is distributed under the SIL Open Font License 1.1; the license is in
  `Fonts.txt`.
- The previously bundled Mikado files were removed because their metadata said
  "all rights reserved" and no redistribution license was present.

## Audio

- 55 of the 67 shipped audio files directly identify Sound Ideas, Cartoon
  Express, or sound-ideas.com in embedded metadata. They are course-library
  assets covered by the institutional source described in `Sound-Ideas.txt`.
- The remaining 12 files are four BGM exports, one Pro Tools cooking export,
  and seven cutscene effects. Git history records all of them as additions or
  exports by the team's audio contributor. Earlier source WAV files for several
  tracks retain Pro Tools origin metadata. None contains an external creator,
  publisher, website, or commercial stock-pack attribution.

The 12 files without a named embedded library are:

- `Resources/Sounds/bgm/Kitchen Ambience_Loop.ogg`
- `Resources/Sounds/bgm/end of level bgm.ogg`
- `Resources/Sounds/bgm/gameplay bgm.ogg`
- `Resources/Sounds/bgm/start menu bgm.ogg`
- `Resources/Sounds/chop and cook/raw_patty_sizzle.wav`
- `Resources/Sounds/cutscene/End/car pull in from left.ogg`
- `Resources/Sounds/cutscene/End/paper tear.ogg`
- `Resources/Sounds/cutscene/End/run.ogg`
- `Resources/Sounds/cutscene/Level2/doorbell.ogg`
- `Resources/Sounds/cutscene/Level2/sweeping.ogg`
- `Resources/Sounds/cutscene/Level3/paper slide.ogg`
- `Resources/Sounds/cutscene/Level3/pick up recipe paper.ogg`

## Software marks and generated packaging art

- Product icons and team branding were introduced by DigiDucks contributors.
- Linux desktop/package icons are derived from the game's own icon.
- No editor-only icons are included in release packages.

