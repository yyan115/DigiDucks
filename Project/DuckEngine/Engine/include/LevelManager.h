/******************************************************************************/
/*
\file:		LevelManager.h
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:		yankai.tan@digipen.edu


\brief:     Contains the declarations that are used to load the level

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/
#pragma once

#include <string>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class DUCKENGINE_API LevelManager
{
public:

	/****************************************************************
	* @brief Load level using data from json file
	*
	* @param levelFile - json file containing data for the level
	*
	***************************************************************/
	static void LoadLevel(const std::string& levelFile);
	static std::string OpenFileDialog(const std::string& filetype);
	static void OpenLevelDialog();

	static void SaveSceneChanges(const std::string& sceneName);
	static void SaveEntityChanges(int entityID, std::string& sceneName);
	static void OverwritePrefab(int entityID);


};