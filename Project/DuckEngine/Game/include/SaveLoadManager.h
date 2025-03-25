#pragma once
#include "Serialization.h"
#include <string>
#include <filesystem>

class SaveLoadManager
{
public:
	// Static public member variables
	static float masterVolume;
	static float musicVolume;
	static float sfxVolume;
	static int currentLevel;
	static int targetFPS;
	static bool useVSync;
	static std::string savePath;

	// Static public methods
	static bool SaveFileExists();
	static void InitializeSaveFile();
	static bool SaveGame();
	static bool LoadGame();
	static void SetSavePath(const std::string& path);

private:
	// Static private method
	static void CreateNewSaveFile();
};