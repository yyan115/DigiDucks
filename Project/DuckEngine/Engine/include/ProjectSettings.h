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
	static constexpr const char* DEFAULT_START_LEVEL = "GameScene";
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
