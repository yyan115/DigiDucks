#!/usr/bin/env python3
"""Validate Quack Kitchen's serialized runtime asset references.

The old Windows build tolerated filename-case mistakes and missing files in
scene data. Linux does not, so release builds run this check before packaging.
"""

from __future__ import annotations

import json
import re
import sys
from collections import defaultdict
from pathlib import Path
from typing import Any, Iterable


REPOSITORY = Path(__file__).resolve().parents[1]
PROJECT = REPOSITORY / "Project" / "DuckEngine"
RESOURCES = PROJECT / "Resources"
SOURCE_ROOTS = (PROJECT / "Engine", PROJECT / "Game")
TEXTURE_EXTENSIONS = {".jpeg", ".jpg", ".png"}
# These lookups are in dormant/error-handling code and intentionally resolve
# to the engine's empty texture. They do not correspond to shipped assets.
INTENTIONALLY_EMPTY_NAMED_TEXTURES = {"NULL", "confirm_off", "confirm_on"}
# These unresolved paths and the ambiguous "quit" lookup are part of the
# original shipped game data. Keep them visible as explicitly accepted legacy
# debt so the resume/Linux work does not silently redesign the game. Both
# belong to the HowToPlay scene, which is registered but never made active, so
# neither is ever loaded and neither can be compared against anything on
# screen.
INTENTIONALLY_UNRESOLVED_ASSET_PATHS = {
    "Resources/Sounds/gameplay bgm - unmastered.mp3",
    "Resources/Sprites/HowToPlay/background.png",
}
INTENTIONALLY_AMBIGUOUS_NAMED_TEXTURES = {"quit"}


def game_logic_errors() -> list[str]:
    """Every logic a scene or prefab names has to be registered.

    A GameLogicComponent names its logic as a string, and GameLogicManager
    looks that string up at load. A name with no registration is not an error
    anywhere: the entity simply has no behaviour, which on a kitchen station or
    a customer looks like a gameplay bug rather than a missing registration.
    """
    registered = set(re.findall(
        r'AddLogic\("([^"]+)"',
        (PROJECT / "Game" / "src" / "GameManager.cpp").read_text(encoding="utf-8")))
    errors = []
    for folder in ("Scenes", "Prefabs"):
        for path in sorted((RESOURCES / folder).glob("*.json")):
            text = path.read_text(encoding="utf-8", errors="replace")
            for block in re.findall(r'"logicNames"\s*:\s*\[([^\]]*)\]', text):
                for name in re.findall(r'"([^"]+)"', block):
                    if name not in registered:
                        errors.append(
                            f"{repository_relative(path)} asks for a game logic that "
                            f"GameManager never registers: {name}"
                        )
    return errors


def journal_page_errors() -> list[str]:
    """Every How To Play page the code can ask for has to exist.

    HowToPlayLogic builds these paths by joining a prefix, a number and an
    extension, and the check below skips any literal without a suffix because a
    concatenated path cannot be resolved from the string alone. That left the
    journal unchecked: a missing page would have gone out in a package and only
    shown up as a blank book. The upper bound is read from the header rather
    than repeated here, so adding a page cannot leave this behind.
    """
    header = PROJECT / "Game" / "include" / "HowToPlayLogic.h"
    source = header.read_text(encoding="utf-8")
    # The journal is opened on one of two runs of pages, how to play and the
    # credits, and the last page of the later run is the highest it can ask for.
    pages = [int(n) for n in re.findall(r"kLast\w+Page\s*=\s*(\d+)", source)]
    if not pages:
        return [f"no kLast...Page bound is declared in {repository_relative(header)}"]
    errors = []
    for page in range(1, max(pages) + 1):
        path = RESOURCES / "Sprites" / "HowToPlay" / f"journal_{page}.png"
        if not path.is_file():
            errors.append(
                f"HowToPlayLogic can ask for page {page} and it does not exist: "
                f"{repository_relative(path)}"
            )
    return errors


QUIT_DIALOGUE = ("Quit_Cfm_Bg", "Quit_Yes_Btn", "Quit_No_Btn", "Quit_Title", "Quit_Question")
# Every scene a player can reach, because the window's own close request can
# arrive on any of them. HowToPlay is registered and never made active.
QUIT_DIALOGUE_SCENES = ("Intro", "MainMenu", "Level0", "Level1", "Level1_5", "Level2",
                        "Level2_5", "Level3", "Level3_5", "EndScene")


def quit_dialogue_errors() -> list[str]:
    """Every reachable scene carries the quit confirmation, identical to the menu's.

    Alt+F4 has to ask the same question wherever it lands, and this engine
    keeps a scene's interface in the scene file, so the dialogue is copied into
    each one. A copy that drifts from the others would ask the question in a
    different place or a different way on one screen; one that is missing
    would let the window close without asking.
    """
    scenes = RESOURCES / "Scenes"
    reference = json.loads((scenes / "MainMenu.json").read_text(encoding="utf-8"))["gameObjects"]
    errors = []
    for name in QUIT_DIALOGUE_SCENES:
        objects = json.loads((scenes / f"{name}.json").read_text(encoding="utf-8"))["gameObjects"]
        for part in QUIT_DIALOGUE:
            if part not in objects:
                errors.append(f"{name}.json has no {part}, so the close request would not ask first")
            elif objects[part] != reference.get(part):
                errors.append(f"{name}.json's {part} differs from MainMenu.json's; the quit dialogue must match everywhere")
    return errors


OPTIONS_PANEL = ("Settings_Menu", "CloseSettings_Button",
                 "MasterVolume_Slider", "MasterVolume_SliderFill", "MasterVolume_Value",
                 "BGMVolume_Slider", "BGMVolume_SliderFill", "BGMVolume_Value",
                 "SFXVolume_Slider", "SFXVolume_SliderFill", "SFXVolume_Value",
                 "FPSTarget_Slider", "FPSTarget_SliderFill", "FPSTarget_Value",
                 "VSync_Toggle")
OPTIONS_PANEL_SCENES = ("MainMenu", "Level0", "Level1", "Level1_5", "Level2", "Level2_5", "Level3", "Level3_5")


def options_panel_errors() -> list[str]:
    """Every level, and the main menu, lays out its options panel the same way as Level 1.

    The labels are painted into the panel's art and the bars, values and the
    VSYNC box are placed over them, so the two only line up at one panel size.
    Six levels once drew the panel taller than Level 1 while keeping Level 1's
    controls, and every row sat off its label in those six. The drawing order
    is compared too: the panel has to be drawn over the dialogue in every
    level, not only in the one that was checked.
    """
    scenes = RESOURCES / "Scenes"

    def layout(objects: dict, part: str) -> Any:
        placed = drawn = None
        for component in objects[part].get("components", []):
            properties = component.get("properties", {})
            if component.get("type") == "TransformComponent":
                placed = properties.get("position"), properties.get("scale")
            elif "sortingOrder" in properties:
                drawn = properties["sortingOrder"]
        return placed, drawn

    reference = json.loads((scenes / "Level1.json").read_text(encoding="utf-8"))["gameObjects"]
    errors = []
    for name in OPTIONS_PANEL_SCENES:
        objects = json.loads((scenes / f"{name}.json").read_text(encoding="utf-8"))["gameObjects"]
        for part in OPTIONS_PANEL:
            if part not in objects:
                errors.append(f"{name}.json has no {part} in its options panel")
            elif layout(objects, part) != layout(reference, part):
                errors.append(f"{name}.json's {part} is placed or layered differently from Level1.json's; the options panel must match in every level")
    return errors


def strings_in(value: Any) -> Iterable[str]:
    if isinstance(value, str):
        yield value
    elif isinstance(value, list):
        for item in value:
            yield from strings_in(item)
    elif isinstance(value, dict):
        for item in value.values():
            yield from strings_in(item)


def repository_relative(path: Path) -> str:
    return path.relative_to(REPOSITORY).as_posix()


def main() -> int:
    errors: list[str] = []

    required_paths = (
        RESOURCES / "Fonts",
        RESOURCES / "Prefabs",
        RESOURCES / "Scenes",
        RESOURCES / "Shaders",
        RESOURCES / "Sounds",
        RESOURCES / "Sprites",
        RESOURCES / "settings.json",
    )
    for path in required_paths:
        if not path.exists():
            errors.append(f"missing required runtime path: {repository_relative(path)}")

    resource_entries = {
        "Resources/" + path.relative_to(RESOURCES).as_posix()
        for path in RESOURCES.rglob("*")
    }

    json_files = sorted(RESOURCES.rglob("*.json"))
    reference_count = 0
    for json_file in json_files:
        try:
            document = json.loads(json_file.read_text(encoding="utf-8-sig"))
        except (OSError, UnicodeError, json.JSONDecodeError) as exception:
            errors.append(f"invalid JSON: {repository_relative(json_file)}: {exception}")
            continue

        for value in strings_in(document):
            if not value.startswith(("Resources/", "Resources\\")):
                continue
            reference_count += 1
            if "\\" in value:
                errors.append(
                    f"non-portable separator in {repository_relative(json_file)}: {value}"
                )
                continue
            if value in INTENTIONALLY_UNRESOLVED_ASSET_PATHS:
                continue
            if value not in resource_entries:
                errors.append(
                    f"missing or wrong-case asset in {repository_relative(json_file)}: {value}"
                )

    textures_by_stem: dict[str, list[str]] = defaultdict(list)
    for path in (RESOURCES / "Sprites").rglob("*"):
        if path.is_file() and path.suffix.lower() in TEXTURE_EXTENSIONS:
            textures_by_stem[path.stem].append(
                "Resources/Sprites/" + path.relative_to(RESOURCES / "Sprites").as_posix()
            )

    lookup_pattern = re.compile(r'GetTextureByName\(\s*"([^"]+)"\s*\)')
    resource_literal_pattern = re.compile(r'"(Resources[/\\][^"\r\n]+)"')
    source_reference_count = 0
    for source_root in SOURCE_ROOTS:
        for source_file in sorted(source_root.rglob("*")):
            if source_file.suffix.lower() not in {".cpp", ".h", ".hpp"}:
                continue
            try:
                source = source_file.read_text(encoding="utf-8")
            except UnicodeDecodeError:
                source = source_file.read_text(encoding="utf-8", errors="replace")
            for texture_name in lookup_pattern.findall(source):
                if texture_name in INTENTIONALLY_EMPTY_NAMED_TEXTURES:
                    continue
                matches = textures_by_stem.get(texture_name, [])
                if not matches:
                    errors.append(
                        f"named texture does not exist in {repository_relative(source_file)}: "
                        f"{texture_name}"
                    )
                elif len(matches) > 1:
                    if texture_name in INTENTIONALLY_AMBIGUOUS_NAMED_TEXTURES:
                        continue
                    errors.append(
                        f"ambiguous named texture in {repository_relative(source_file)}: "
                        f"{texture_name} -> {', '.join(sorted(matches))}"
                    )

            for resource_path in resource_literal_pattern.findall(source):
                # Directory names and path prefixes are completed dynamically.
                # A suffix means the source contains a complete, checkable file path.
                if not Path(resource_path).suffix:
                    continue
                source_reference_count += 1
                if "\\" in resource_path:
                    errors.append(
                        f"non-portable source asset path in "
                        f"{repository_relative(source_file)}: {resource_path}"
                    )
                elif resource_path not in resource_entries:
                    errors.append(
                        f"missing or wrong-case source asset in "
                        f"{repository_relative(source_file)}: {resource_path}"
                    )

    errors.extend(journal_page_errors())
    errors.extend(game_logic_errors())
    errors.extend(quit_dialogue_errors())
    errors.extend(options_panel_errors())

    if errors:
        print("Asset validation failed:", file=sys.stderr)
        for error in sorted(set(errors)):
            print(f"  - {error}", file=sys.stderr)
        return 1

    print(
        f"Validated {len(json_files)} JSON files, {reference_count} serialized "
        f"asset references, {source_reference_count} source asset references, "
        f"every How To Play page, every named game logic, the quit dialogue in every scene, "
        f"one options panel layout in every level and the main menu, and named texture lookups."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
