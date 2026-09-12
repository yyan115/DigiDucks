#!/usr/bin/env python3
"""Print the Quack Kitchen release version.

The version is declared once, by the CMake project command in
Project/DuckEngine/CMakeLists.txt. Installer, packaging, and workflow steps
read it from here instead of repeating the number.
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROJECT_FILE = ROOT / "Project" / "DuckEngine" / "CMakeLists.txt"
VERSION_PATTERN = re.compile(
    r"^project\(QuackKitchen\s+VERSION\s+(\d+\.\d+\.\d+)\b", re.MULTILINE
)


def read_version() -> str:
    """Return the declared version, or raise SystemExit when it is missing."""
    match = VERSION_PATTERN.search(PROJECT_FILE.read_text(encoding="utf-8"))
    if not match:
        raise SystemExit(
            f"no project(QuackKitchen VERSION <x.y.z>) declaration in {PROJECT_FILE}"
        )
    return match.group(1)


if __name__ == "__main__":
    import sys

    version = read_version()
    if "--installer-define" in sys.argv[1:]:
        # Inno Setup reads this as Installer/Version.iss.
        print(f'#define GameVersion "{version}"')
    else:
        print(version)
