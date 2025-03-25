#include "SaveLoadManager.h"
#include "ProjectSettings.h"
#include <iostream>

// Initialize static members
std::string SaveLoadManager::savePath = "Resources/save.json";
float SaveLoadManager::masterVolume = 1.0f;
float SaveLoadManager::musicVolume = 1.0f;
float SaveLoadManager::sfxVolume = 1.0f;
int SaveLoadManager::currentLevel = -1;
int SaveLoadManager::targetFPS = 60;
bool SaveLoadManager::useVSync = true;

bool SaveLoadManager::SaveFileExists()
{
	return std::filesystem::exists(savePath);
}

void SaveLoadManager::InitializeSaveFile()
{
	if (!SaveFileExists())
	{
		std::cout << "Save file not found. Creating a new one at: " << savePath << std::endl;
		CreateNewSaveFile();
	}
	else
	{
		// Load existing settings
		LoadGame();
	}
}

bool SaveLoadManager::SaveGame()
{
	try
	{
		nlohmann::json saveData;
		// Save volume settings from static variables
		saveData["volume"]["master"] = masterVolume;
		saveData["volume"]["music"] = musicVolume;
		saveData["volume"]["sfx"] = sfxVolume;

		// Save current level from static variable
		saveData["currentLevel"] = currentLevel;

		// Save video settings
		saveData["video"]["targetFPS"] = targetFPS;
		saveData["video"]["useVSync"] = useVSync;

		Serialization::SaveJsonFile(savePath, saveData);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error saving game: " << e.what() << std::endl;
		return false;
	}
}

bool SaveLoadManager::LoadGame()
{
	if (!SaveFileExists())
	{
		std::cout << "No save file found. Creating new save with ProjectSettings values." << std::endl;
		CreateNewSaveFile(); // Create new save with ProjectSettings values
		return true;
	}

	nlohmann::json saveData = Serialization::LoadJsonFile(savePath);

	// Load volume settings into static variables
	masterVolume = saveData["volume"]["master"];
	musicVolume = saveData["volume"]["music"];
	sfxVolume = saveData["volume"]["sfx"];

	// Load current level into static variable
	currentLevel = saveData["currentLevel"];

	// Load video settings
	if (saveData.contains("video")) {
		if (saveData["video"].contains("targetFPS")) {
			targetFPS = saveData["video"]["targetFPS"];
		}
		if (saveData["video"].contains("useVSync")) {
			useVSync = saveData["video"]["useVSync"];
		}
	}
	else {
		// If video settings don't exist in save file, use ProjectSettings
		targetFPS = ProjectSettings::GetTargetFPS();
		useVSync = ProjectSettings::GetUseVSync();
	}

	return true;
}

void SaveLoadManager::CreateNewSaveFile()
{
	// Use values from ProjectSettings
	masterVolume = ProjectSettings::GetMasterVolume();
	musicVolume = ProjectSettings::GetVolumeCategory("BGM");
	sfxVolume = ProjectSettings::GetVolumeCategory("SFX");
	targetFPS = ProjectSettings::GetTargetFPS();
	useVSync = ProjectSettings::GetUseVSync();
	currentLevel = -1; // Default starting level
	SaveGame();
}

void SaveLoadManager::SetSavePath(const std::string& path)
{
	savePath = path;
}