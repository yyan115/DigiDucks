/******************************************************************************/
/*!
\file     SoundSystem.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the SoundSystem class
		  which is responsible for managing the sound system in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "System.h"
#include "DuckEngine.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class SoundSystem : public System {
public:
    /****************************************************************
	* @brief Initializes the FMOD system
    *
    * @return void
    ***************************************************************/
    void Start() override;

    /****************************************************************
	* @brief Updates the FMOD system
    *
    * @return void
    ***************************************************************/
    void Update() override;

    /****************************************************************
	* @brief Stops all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static DUCKENGINE_API void StopAllSounds();

    /****************************************************************
	* @brief Pauses all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static DUCKENGINE_API void PauseAllSounds();

    /****************************************************************
	* @brief Resumes all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static DUCKENGINE_API void ResumeAllSounds();

	static void PlaySounds(const std::string& soundID, bool loop = false, float volume = 1.0f);

	static void StopSounds(const std::string& soundID);

	static void SetSoundVolume(const std::string& soundID, float volume);

	static void SetMasterVolume(float volume);

	static void SetCategoryVolume(const std::string& category, float volume);

	static void AddSoundToCategory(const std::string& soundID, const std::string& category);

	static bool IsSoundPlaying(const std::string& soundID);

	static void ResumeSound(const std::string& soundID);

	static void PauseSound(const std::string& soundID);

private:
	static float masterVolume;
	static DUCKENGINE_API std::unordered_map<std::string, std::string> soundCategories;
	static DUCKENGINE_API std::unordered_map<std::string, float> categoryVolumes;
	static DUCKENGINE_API std::unordered_map<std::string, FMOD::Channel*> activeChannels;
};

