#!/usr/bin/env python3
"""Check a staged Quack Kitchen release before packaging it."""

from __future__ import annotations

import argparse
import sys
from pathlib import Path


MAX_INSTALL_SIZE = 500 * 1024 * 1024
FORBIDDEN_SUFFIXES = {
    ".ai",
    ".ase",
    ".aseprite",
    ".blend",
    ".blend1",
    ".cpp",
    ".exp",
    ".h",
    ".hpp",
    ".ilk",
    ".kra",
    ".lib",
    ".obj",
    ".pdb",
    ".psd",
    ".sln",
    ".vcxproj",
    ".xcf",
}
FORBIDDEN_DIRECTORY_NAMES = {
    ".git",
    ".github",
    ".vs",
    ".vscode",
    "cmakefiles",
    "editoricons",
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("stage", type=Path)
    parser.add_argument("--platform", choices=("linux", "windows"), required=True)
    arguments = parser.parse_args()

    stage = arguments.stage.resolve()
    appdir = (
        arguments.platform == "linux"
        and stage.name == "bin"
        and stage.parent.name == "usr"
        and (stage.parent.parent / "AppRun").is_file()
    )
    release_root = stage.parent.parent if appdir else stage
    library_root = stage.parent / "lib" if appdir else stage
    repository = Path(__file__).resolve().parents[1]
    source_resources = repository / "Project" / "DuckEngine" / "Resources"
    staged_resources = stage / "Resources"
    errors: list[str] = []

    expected_top_level = ["Quack Kitchen.exe", "DuckEngine.dll", "fmod.dll"]
    expected_libraries: tuple[str, ...] = ()
    if arguments.platform == "linux":
        expected_top_level = ["Quack Kitchen"]
        expected_libraries = ("libDuckEngine.so", "libfmod.so.14")
        if not appdir:
            expected_top_level.extend(expected_libraries)

    for name in expected_top_level:
        if not (stage / name).is_file():
            errors.append(f"missing runtime file: {name}")

    if appdir:
        for name in expected_libraries:
            if not (library_root / name).is_file():
                errors.append(f"missing runtime library: {name}")

    allowed_top_level = set(expected_top_level) | {"Licenses", "Resources"}
    if arguments.platform == "linux":
        # AppImages add this small launcher beside the game so desktop files
        # never need to quote the executable's space-containing filename.
        allowed_top_level.add("quack-kitchen")
    if stage.is_dir():
        for path in stage.iterdir():
            if path.name not in allowed_top_level:
                errors.append(f"unexpected top-level release entry: {path.name}")

    for relative in (
        "Fonts",
        "Prefabs",
        "Scenes",
        "Shaders",
        "Sounds",
        "Sprites",
        "License.txt",
        "settings.json",
    ):
        if not (staged_resources / relative).exists():
            errors.append(f"missing packaged resource: Resources/{relative}")

    if not (stage / "Licenses").is_dir():
        errors.append("missing packaged license directory: Licenses")

    required_licenses = (
        "ASSET_PROVENANCE.md",
        "Dear-ImGui.txt",
        "FMOD.txt",
        "Fonts.txt",
        "FreeType-FTL.txt",
        "FreeType.txt",
        "GLEW.txt",
        "GLFW.txt",
        "GLM.txt",
        "Nlohmann-JSON.txt",
        "README.md",
        "Sound-Ideas.txt",
        "stb_image.txt",
    )
    for name in required_licenses:
        if not (stage / "Licenses" / name).is_file():
            errors.append(f"missing required license file: Licenses/{name}")

    if (staged_resources / "save.json").exists():
        errors.append("Resources/save.json is writable user data and must not be packaged")
    if (staged_resources / "EditorIcons").exists():
        errors.append("editor-only Resources/EditorIcons must not be packaged")

    expected_resources = {
        path.relative_to(source_resources).as_posix()
        for path in source_resources.rglob("*")
        if path.is_file()
        and path.name != "save.json"
        and "EditorIcons" not in path.relative_to(source_resources).parts
    }
    actual_resources = {
        path.relative_to(staged_resources).as_posix()
        for path in staged_resources.rglob("*")
        if path.is_file()
    }
    for missing in sorted(expected_resources - actual_resources):
        errors.append(f"resource omitted from package: Resources/{missing}")
    for unexpected in sorted(actual_resources - expected_resources):
        errors.append(f"unexpected packaged resource: Resources/{unexpected}")

    files = [path for path in release_root.rglob("*") if path.is_file()]
    install_size = sum(path.stat().st_size for path in files)
    if install_size >= MAX_INSTALL_SIZE:
        errors.append(
            f"installed size is {install_size / 1024 / 1024:.1f} MiB; limit is 500 MiB"
        )
    for path in files:
        if path.suffix.lower() in FORBIDDEN_SUFFIXES:
            errors.append(
                f"development file in release: {path.relative_to(release_root)}"
            )
        if any(
            part.lower() in FORBIDDEN_DIRECTORY_NAMES
            for part in path.relative_to(release_root).parts[:-1]
        ):
            errors.append(
                f"development directory in release: {path.relative_to(release_root)}"
            )

    if errors:
        print("Release verification failed:", file=sys.stderr)
        for error in errors:
            print(f"  - {error}", file=sys.stderr)
        return 1

    print(
        f"Verified {arguments.platform} release: {len(files)} files, "
        f"{install_size / 1024 / 1024:.1f} MiB installed."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
