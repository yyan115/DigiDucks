/******************************************************************************/
/*!
\file       ProjectSettings.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the ProjectSettings class, which manages global game
			settings such as window resolution, frame rate, VSync, and audio
			volumes. These settings are loaded from and saved to a JSON file,
			ensuring configurability within DuckEngine.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class ProjectSettings
{
public:
	// Loads settings from JSON file
	static DUCKENGINE_API void Load(const std::string& filePath);

	// Saves settings to JSON file
	static DUCKENGINE_API void Save(const std::string& filePath);

	// Basic project settings
	static DUCKENGINE_API const std::string& GetStartLevel();
	static DUCKENGINE_API void SetStartLevel(const std::string& level);

	static DUCKENGINE_API int  GetWindowWidth();
	static DUCKENGINE_API void SetWindowWidth(int width);

	static DUCKENGINE_API int  GetWindowHeight();
	static DUCKENGINE_API void SetWindowHeight(int height);

	static DUCKENGINE_API int  GetTargetFPS();
	static DUCKENGINE_API void SetTargetFPS(int fps);

	static DUCKENGINE_API bool GetUseVSync();
	static DUCKENGINE_API void SetUseVSync(bool enabled);

	// ---------- Volume Functions ------------
	// "Master" is a single float for overall volume
	static DUCKENGINE_API float GetMasterVolume();
	static DUCKENGINE_API void  SetMasterVolume(float volume);

	// Category volumes stored in a map: Default, BGM, SFX, UI, etc.
	static DUCKENGINE_API float GetVolumeCategory(const std::string& category);
	static DUCKENGINE_API void  SetVolumeCategory(const std::string& category, float volume);

private:
	// Default fallback values
	static constexpr const char* DEFAULT_START_LEVEL = "MainMenu";
	static constexpr int         DEFAULT_WINDOW_WIDTH = 1280;
	static constexpr int         DEFAULT_WINDOW_HEIGHT = 720;
	static constexpr int         DEFAULT_TARGET_FPS = 60;
	static constexpr float       DEFAULT_MASTER_VOLUME = 1.0f;

	// Internal stored values
	static DUCKENGINE_API std::string startLevel;
	static DUCKENGINE_API bool        useVSync;
	static int  windowWidth;
	static int  windowHeight;
	static int  targetFPS;

	// Volume
	static DUCKENGINE_API float masterVolume;
	// Per-category volumes: e.g. "Default", "BGM", "SFX", "UI", etc.
	static DUCKENGINE_API std::unordered_map<std::string, float> volumeCategories;

	static void LoadDefaults();
};
