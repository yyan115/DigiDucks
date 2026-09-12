/******************************************************************************/
/*!
\file       SaveLoadManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Implements the SaveLoadManager class. This class provides static
			methods to manage save data such as volume levels, VSync setting,
			current level progress, and target FPS.

			Save data is stored as a JSON file on disk, and loaded or
			initialized during game startup. If a save file does not exist, a
			new one is created based on default ProjectSettings values.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "SaveLoadManager.h"
#include "ProjectSettings.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace
{
	constexpr int MinimumSavedLevel = -1;
	constexpr int MaximumSavedLevel = 5;
	constexpr int MinimumTargetFPS = 0;
	constexpr int MaximumTargetFPS = 240;

	const nlohmann::json* FindObject(
		const nlohmann::json& parent, const char* key, bool& repaired)
	{
		const auto value = parent.find(key);
		if (value == parent.end() || !value->is_object())
		{
			repaired = true;
			return nullptr;
		}
		return &*value;
	}

	float ReadVolume(
		const nlohmann::json* object, const char* key, float fallback,
		bool& repaired)
	{
		if (!object)
		{
			return fallback;
		}

		const auto value = object->find(key);
		if (value == object->end() || !value->is_number())
		{
			repaired = true;
			return fallback;
		}

		try
		{
			const double loadedValue = value->get<double>();
			if (!std::isfinite(loadedValue))
			{
				repaired = true;
				return fallback;
			}

			const float clampedValue = static_cast<float>(
				std::clamp(loadedValue, 0.0, 1.0));
			repaired |= loadedValue != clampedValue;
			return clampedValue;
		}
		catch (const std::exception&)
		{
			repaired = true;
			return fallback;
		}
	}

	int ReadInteger(
		const nlohmann::json* object, const char* key, int fallback,
		int minimum, int maximum, bool& repaired)
	{
		if (!object)
		{
			return fallback;
		}

		const auto value = object->find(key);
		if (value == object->end() ||
			(!value->is_number_integer() && !value->is_number_unsigned()))
		{
			repaired = true;
			return fallback;
		}

		try
		{
			const long long loadedValue = value->get<long long>();
			const long long clampedValue = std::clamp(
				loadedValue, static_cast<long long>(minimum),
				static_cast<long long>(maximum));
			repaired |= loadedValue != clampedValue;
			return static_cast<int>(clampedValue);
		}
		catch (const std::exception&)
		{
			repaired = true;
			return fallback;
		}
	}

	bool ReadBoolean(
		const nlohmann::json* object, const char* key, bool fallback,
		bool& repaired)
	{
		if (!object)
		{
			return fallback;
		}

		const auto value = object->find(key);
		if (value == object->end() || !value->is_boolean())
		{
			repaired = true;
			return fallback;
		}
		return value->get<bool>();
	}
}

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

	try
	{
		const nlohmann::json saveData = Serialization::LoadJsonFile(savePath);
		if (!saveData.is_object())
		{
			throw std::runtime_error("save root must be a JSON object");
		}

		bool repaired = false;
		const nlohmann::json* volume = FindObject(saveData, "volume", repaired);
		const nlohmann::json* video = FindObject(saveData, "video", repaired);

		// Read into locals so one bad field cannot leave half-applied settings.
		const float loadedMasterVolume = ReadVolume(
			volume, "master", ProjectSettings::GetMasterVolume(), repaired);
		const float loadedMusicVolume = ReadVolume(
			volume, "music", ProjectSettings::GetVolumeCategory("BGM"), repaired);
		const float loadedSfxVolume = ReadVolume(
			volume, "sfx", ProjectSettings::GetVolumeCategory("SFX"), repaired);
		const int loadedCurrentLevel = ReadInteger(
			&saveData, "currentLevel", MinimumSavedLevel,
			MinimumSavedLevel, MaximumSavedLevel, repaired);
		const int loadedTargetFPS = ReadInteger(
			video, "targetFPS", ProjectSettings::GetTargetFPS(),
			MinimumTargetFPS, MaximumTargetFPS, repaired);
		const bool loadedUseVSync = ReadBoolean(
			video, "useVSync", ProjectSettings::GetUseVSync(), repaired);

		masterVolume = loadedMasterVolume;
		musicVolume = loadedMusicVolume;
		sfxVolume = loadedSfxVolume;
		currentLevel = loadedCurrentLevel;
		targetFPS = loadedTargetFPS;
		useVSync = loadedUseVSync;

		if (repaired)
		{
			std::cerr << "Save data was incomplete or invalid; repaired "
				"values will be written to: " << savePath << std::endl;
			SaveGame();
		}
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Could not load save data from " << savePath << ": "
			<< e.what() << ". Restoring defaults." << std::endl;
		CreateNewSaveFile();
		return false;
	}
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
