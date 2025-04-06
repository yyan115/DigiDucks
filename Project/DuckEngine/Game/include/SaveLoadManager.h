/******************************************************************************/
/*!
\file       SaveLoadManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Declares the SaveLoadManager class, a static utility that manages
			game save data including volume settings, video settings, and
			player progress.

			This manager handles reading and writing JSON-based save files,
			creating default saves using ProjectSettings, and exposing runtime
			variables globally for easy access and persistence.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


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