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
                    errors.append(
                        f"ambiguous named texture in {repository_relative(source_file)}: "
                        f"{texture_name} -> {', '.join(sorted(matches))}"
                    )

    if errors:
        print("Asset validation failed:", file=sys.stderr)
        for error in sorted(set(errors)):
            print(f"  - {error}", file=sys.stderr)
        return 1

    print(
        f"Validated {len(json_files)} JSON files, {reference_count} serialized "
        f"asset references, and named texture lookups."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
