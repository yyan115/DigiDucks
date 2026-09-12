#!/usr/bin/env python3
"""Validate repository evidence for the DigiPen Game Gallery checklist.

This check covers requirements that can be established from source, data, and
packaging definitions. Subjective playability and final installed-Windows
behavior remain acceptance-test items documented in docs/GAME_GALLERY_READINESS.md.
"""

from __future__ import annotations

import json
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ERRORS: list[str] = []


def read(relative: str) -> str:
    path = ROOT / relative
    if not path.is_file():
        ERRORS.append(f"missing required file: {relative}")
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def require_text(relative: str, values: tuple[str, ...], reason: str) -> None:
    content = read(relative)
    missing = [value for value in values if value not in content]
    if missing:
        ERRORS.append(
            f"{relative}: missing {reason}: " + ", ".join(repr(item) for item in missing)
        )


def require_guarded(relative: str, values: tuple[str, ...]) -> None:
    content = read(relative)
    guard = "#ifdef QUACK_KITCHEN_ENABLE_DEVELOPER_SHORTCUTS"
    for value in values:
        start = 0
        found = False
        while True:
            position = content.find(value, start)
            if position < 0:
                break
            found = True
            guard_position = content.rfind(guard, 0, position)
            guard_end = content.find("#endif", guard_position)
            if guard_position < 0 or guard_end < position:
                ERRORS.append(
                    f"{relative}: developer behavior {value!r} is not debug-guarded"
                )
            start = position + len(value)
        if not found:
            ERRORS.append(f"{relative}: expected developer behavior not found: {value!r}")


def sprite_texture(scene: dict, entity_name: str) -> tuple[str, bool] | None:
    entity = scene.get("gameObjects", {}).get(entity_name)
    if not isinstance(entity, dict):
        ERRORS.append(f"Intro scene is missing entity {entity_name!r}")
        return None
    for component in entity.get("components", []):
        if component.get("type") == "SpriteRendererComponent":
            properties = component.get("properties", {})
            return str(properties.get("texture", "")), bool(properties.get("isVisible"))
    ERRORS.append(f"Intro entity {entity_name!r} has no sprite renderer")
    return None


def validate_settings_and_intro() -> None:
    settings_path = ROOT / "Project/DuckEngine/Resources/settings.json"
    try:
        settings = json.loads(read(str(settings_path.relative_to(ROOT))))
    except json.JSONDecodeError as error:
        ERRORS.append(f"settings.json is invalid: {error}")
        return

    project = settings.get("project", {})
    if project.get("startLevel") != "Intro":
        ERRORS.append("settings.json must launch the Intro scene")
    if not isinstance(project.get("targetFPS"), int) or project["targetFPS"] < 60:
        ERRORS.append("settings.json targetFPS must be at least 60")

    intro_relative = "Project/DuckEngine/Resources/Scenes/Intro.json"
    try:
        intro = json.loads(read(intro_relative))
    except json.JSONDecodeError as error:
        ERRORS.append(f"Intro.json is invalid: {error}")
        return

    expected_sprites = {
        "Logo": ("Resources/Sprites/Intro/DigiPen_Singapore_WEB_WHITE.png", True),
        "Logo2": ("Resources/Sprites/Intro/digiducks_logo.png", False),
        "FMODLogo": ("Resources/Sprites/Intro/fmod_logo_white.png", False),
        "GamepadNotice": ("Resources/Sprites/Intro/gamepad_notice.png", False),
    }
    for entity, expected in expected_sprites.items():
        actual = sprite_texture(intro, entity)
        if actual is not None and actual != expected:
            ERRORS.append(
                f"Intro {entity} expected texture/visibility {expected!r}, got {actual!r}"
            )

    require_text(
        "Project/DuckEngine/Game/include/Intro.h",
        ("float displayDuration = 2.0f;",),
        "two-second logo hold",
    )
    require_text(
        "Project/DuckEngine/Game/src/Intro.cpp",
        (
            "MOUSE_BUTTON_LEFT",
            "MOUSE_BUTTON_RIGHT",
            "KEY_ENTER",
            "KEY_SPACE",
            "KEY_ESCAPE",
            "GAMEPAD_BUTTON_START",
            "GAMEPAD_BUTTON_A",
            'GameManager::SetActiveScene("MainMenu")',
        ),
        "required intro bypass and destination",
    )
    require_text(
        "Project/DuckEngine/Game/main.cpp",
        (
            'DuckEngine::SetWindowTitle("Quack Kitchen")',
            "firstFrameSeconds > 3.0",
            'GetActiveSceneName() ==\n\t\t\t\t"MainMenu"',
            "Smoke test reached and rendered MainMenu.",
        ),
        "startup timing and complete-intro smoke assertions",
    )


def validate_installer_and_packages() -> None:
    require_text(
        "Installer/InstallScript.iss",
        (
            '#define GameName "Quack Kitchen"',
            '#define GameVersion "1.0.0"',
            "AppPublisher=DigiPen Institute of Technology",
            r"DefaultDirName={autopf}\DigiPen\{#GameName}",
            r"DefaultGroupName=DigiPen\{#GameName}",
            "LicenseFile=INSTALLERFILES\\DigiPen_EULA.txt",
            "SetupIconFile=..\\Project\\DuckEngine\\Resources\\GameIcon.ico",
            "RestartIfNeededByRun=no",
            "MinVersion=10.0.17763",
            'Name: "desktopicon";',
            'Source: "REDIST\\VC_redist.x64.exe";',
            'Filename: "{tmp}\\VC_redist.x64.exe";',
            'Parameters: "/install /quiet /norestart";',
            "waituntilterminated runhidden",
            "postinstall skipifsilent",
        ),
        "DigiPen installer policy",
    )
    require_text(
        "Installer/INSTALLERFILES/DigiPen_EULA.txt",
        (
            "The games software was created by students of DigiPen Institute of Technology",
            "THE GAMES SOFTWARE IS MADE AVAILABLE BY DIGIPEN AS-IS",
            "YOU HEREBY AGREE TO INDEMNIFY, DEFEND AND HOLD HARMLESS DIGIPEN",
        ),
        "DigiPen EULA precedent text",
    )
    require_text(
        "Project/DuckEngine/CMakeLists.txt",
        (
            "project(QuackKitchen VERSION 1.0.0",
            "add_executable(QuackKitchen WIN32",
            "PATTERN \"EditorIcons\" EXCLUDE",
            "PATTERN \"save.json\" EXCLUDE",
            "$<$<CONFIG:Debug>:QUACK_KITCHEN_ENABLE_DEVELOPER_SHORTCUTS>",
        ),
        "release identity and exclusions",
    )
    require_text(
        "packaging/windows/QuackKitchen.rc.in",
        (
            'VALUE "CompanyName", "DigiPen Institute of Technology\\0"',
            'VALUE "ProductName", "Quack Kitchen\\0"',
            'VALUE "ProductVersion", "@PROJECT_VERSION@\\0"',
        ),
        "Windows executable metadata",
    )
    require_text(
        "packaging/linux/edu.digipen.quackkitchen.desktop",
        ("Name=Quack Kitchen", "Terminal=false", "Exec=quack-kitchen"),
        "Linux desktop metadata",
    )
    require_text(
        "packaging/linux/edu.digipen.quackkitchen.metainfo.xml",
        (
            "<id>edu.digipen.quackkitchen</id>",
            "<content_rating type=\"oars-1.1\" />",
            '<release version="1.0.0"',
        ),
        "Linux package identity and rating metadata",
    )
    require_text(
        "tools/verify_release.py",
        (
            "MAX_INSTALL_SIZE = 500 * 1024 * 1024",
            '".cpp"',
            '".pdb"',
            '".sln"',
            '"save.json"',
            '"editoricons"',
        ),
        "release size and development-file checks",
    )


def validate_runtime_behavior() -> None:
    require_text(
        "Project/DuckEngine/Engine/src/DuckEngine.cpp",
        (
            "WindowManager::IsWindowMinimized()",
            "!WindowManager::IsWindowFocused()",
            "DuckEngine::PauseGame(true)",
            "SoundSystem::PauseAllSounds()",
            "glfwWaitEventsTimeout(0.1)",
        ),
        "focus-loss pause, audio pause, and throttling",
    )
    require_text(
        "Project/DuckEngine/Engine/src/WindowManager.cpp",
        (
            "glfwSetWindowFocusCallback",
            "GLFW_CURSOR_NORMAL",
            "GLFW_CURSOR_DISABLED",
            "GLFW_CURSOR_CAPTURED",
            "window_close_callback",
        ),
        "focus cursor handling and confirmed OS close flow",
    )
    require_text(
        "Project/DuckEngine/Engine/src/InputManager.cpp",
        (
            "glfwSetJoystickCallback(joystickCB)",
            "GLFW_CONNECTED",
            "GLFW_DISCONNECTED",
            "glfwSetCursorPos(pwin, xpos, ypos)",
        ),
        "gamepad hot-plug and cursor confinement",
    )
    require_text(
        "Project/DuckEngine/Game/src/GameManager.cpp",
        ("RequestQuitConfirmation", "ApplyCursorModeForScene"),
        "scene-aware quit confirmation and cursor mode",
    )
    require_text(
        "Project/DuckEngine/Game/src/GameLoopLogic.cpp",
        ("KEY_ESCAPE", "GAMEPAD_BUTTON_START", "PauseGame"),
        "keyboard/gamepad pause",
    )

    menu_sources = (
        "Project/DuckEngine/Game/src/MainMenu.cpp",
        "Project/DuckEngine/Game/src/LevelSelectScreenLogic.cpp",
        "Project/DuckEngine/Game/src/GameSettingsLogic.cpp",
        "Project/DuckEngine/Game/src/HowToPlayLogic.cpp",
        "Project/DuckEngine/Game/src/ExitConfirmLogic.cpp",
        "Project/DuckEngine/Game/src/PauseMenuLogic.cpp",
        "Project/DuckEngine/Game/src/EndGame.cpp",
    )
    for relative in menu_sources:
        require_text(
            relative,
            ("GAMEPAD_AXIS_LEFT_", "GAMEPAD_AXIS_RIGHT_", "GAMEPAD_BUTTON_DPAD_"),
            "D-pad and both-stick menu navigation",
        )

    require_text(
        "Project/DuckEngine/Game/src/MainMenu.cpp",
        (
            '"CREDITS"',
            "ShowCredits(true)",
            "ShowQuitConfirmation(true)",
            "FMOD Studio by Firelight Technologies Pty Ltd",
            "DigiPen Institute of Technology Singapore",
            "All Rights Reserved.",
        ),
        "credits, attribution, and confirmed main-menu quit",
    )
    require_text(
        "Project/DuckEngine/Game/src/PauseMenuLogic.cpp",
        (
            "CreateQuitGameControls",
            "ShowQuitGameConfirmation(true)",
            "ExitConfirm(true)",
        ),
        "pause-menu return and quit confirmations",
    )
    require_text(
        "Project/DuckEngine/Game/src/Level0.cpp",
        ("controls_controller.png", "controls.png"),
        "adaptive in-game tutorial art",
    )
    require_text(
        "Project/DuckEngine/Game/src/HowToPlayLogic.cpp",
        ("journal_1_controller.png", "IsGamepadConnected"),
        "adaptive controller How-to-Play art",
    )
    require_text(
        "Project/DuckEngine/Resources/Scenes/HowToPlay.json",
        ("Resources/Sprites/HowToPlay/journal_1.png",),
        "keyboard How-to-Play art",
    )
    require_text(
        "Project/DuckEngine/Game/src/EndGame.cpp",
        (
            "EndButtonSelection::RESTART",
            "EndButtonSelection::MENU",
            "EndButtonSelection::NEXT",
        ),
        "post-game replay/menu choices",
    )

    require_guarded(
        "Project/DuckEngine/Game/src/GameLoopLogic.cpp",
        ("increaseScore(10)", "timeLeft = 11.f", "ToggleShowDebugColliders()"),
    )
    require_guarded(
        "Project/DuckEngine/Game/src/PlayerLogic.cpp",
        ("ItemType::SALAD_PLATE", "ItemType::CHEESE_BURGER_PLATE"),
    )
    require_guarded(
        "Project/DuckEngine/Game/src/LevelSelectScreenLogic.cpp",
        ("currentStage = 100",),
    )


def validate_licenses_and_ci() -> None:
    for relative in (
        "Licenses/ASSET_PROVENANCE.md",
        "Licenses/FMOD.txt",
        "Licenses/Fonts.txt",
        "Licenses/Sound-Ideas.txt",
        "Licenses/GLFW.txt",
        "Licenses/GLEW.txt",
        "Licenses/FreeType.txt",
        "Licenses/GLM.txt",
        "Licenses/Dear-ImGui.txt",
        "Licenses/Nlohmann-JSON.txt",
        "Licenses/stb_image.txt",
        "docs/GAME_GALLERY_READINESS.md",
    ):
        read(relative)

    require_text(
        ".github/workflows/ci.yml",
        (
            "branches: [main]",
            "runs-on: ubuntu-24.04",
            "runs-on: windows-2022",
            "--smoke-test",
            "--headless-smoke-test",
        ),
        "cross-platform automatic CI",
    )
    require_text(
        ".github/workflows/release.yml",
        (
            "branches: [main]",
            "Windows installer",
            "Linux AppImage and Flatpak",
            "installer.Length -ge 500MB",
            "if: github.event_name == 'workflow_dispatch'",
        ),
        "automatic package validation and gated release publication",
    )


def main() -> int:
    validate_settings_and_intro()
    validate_installer_and_packages()
    validate_runtime_behavior()
    validate_licenses_and_ci()

    if ERRORS:
        print("Game Gallery validation failed:", file=sys.stderr)
        for error in ERRORS:
            print(f"  - {error}", file=sys.stderr)
        return 1

    print("Validated automated DigiPen Game Gallery repository evidence.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
