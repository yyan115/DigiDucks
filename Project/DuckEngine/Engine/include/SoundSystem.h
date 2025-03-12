/******************************************************************************/
/*!
\file     SoundSystem.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the SoundSystem class
		  which is responsible for managing the sound system in the engine.

Copyright (C) 2025 DigiPen Institute of Technology.
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

    /************************************************************************
	@brief Plays a sound effect with the specified sound ID
	@param soundID The ID of the sound to play
	@param loop Whether to loop the sound
	@param volume The volume of the sound
	@param category The category of the sound
	@return FMOD::Channel* The channel that the sound is playing on
    *************************************************************************/
	static FMOD::Channel* PlaySounds(const std::string& soundID, bool loop = false, float volume = 1.0f, const std::string& category = "Default", const std::pair<std::string, float>& _effects = { "Default", 0.0f });

	/************************************************************************
	@brief Stops a sound effect with the specified sound ID
	@param soundID The ID of the sound to stop
	@return void
	*************************************************************************/
	static DUCKENGINE_API void StopSounds(const std::string& soundID);


	/************************************************************************
	@brief Sets the volume of a sound effect with the specified sound ID
	@param soundID The ID of the sound
	@param volume The volume to set
	@return void
	*************************************************************************/
	static DUCKENGINE_API void SetSoundVolume(const std::string& soundID, float volume);


	/************************************************************************
	@brief Sets the master volume of the sound system
	@param volume The volume to set
	@return void
	*************************************************************************/
	static DUCKENGINE_API void SetMasterVolume(float volume);

	/************************************************************************
	@brief Sets the volume of a sound category
	@param category The category of the sound
	@param volume The volume to set
	@return void
	*************************************************************************/
	static DUCKENGINE_API void SetCategoryVolume(const std::string& category, float volume);

	/************************************************************************
	@brief Adds a sound to a category
	@param soundID The ID of the sound
	@param category The category to add the sound to
	@return void
	*************************************************************************/
	static void AddSoundToCategory(const std::string& soundID, const std::string& category);

	/************************************************************************
	@brief Checks if a sound is currently playing
	@param soundID The ID of the sound
	@return bool
	*************************************************************************/
	static bool IsSoundPlaying(const std::string& soundID);

	/************************************************************************
	@brief Resumes a sound from the specified file path
	@param soundID The ID of the sound
	@return void
	*************************************************************************/
	static void ResumeSound(const std::string& soundID);

	/************************************************************************
	@brief Pauses a sound from the specified file path
	@param soundID The ID of the sound
	@return void
	*************************************************************************/
	static void PauseSound(const std::string& soundID);

	/************************************************************************
	@brief Fades out a sound effect with the specified sound ID
	@param SoundComponent The sound component to fade out
	@param duration The duration of the fade out effect
	@return void
	*************************************************************************/
    static DUCKENGINE_API void FadeOutSound(SoundComponent* soundComponent, float duration);

	static void ApplyEffect(FMOD::Channel* channel, const std::pair<std::string, float>& effects);
	static void RemoveEffect(FMOD::Channel* channel);

private:
	static float masterVolume;
	static DUCKENGINE_API std::unordered_map<std::string, std::string> soundCategories;
	static DUCKENGINE_API std::unordered_map<std::string, float> categoryVolumes;
	static DUCKENGINE_API std::unordered_map<std::string, FMOD::Channel*> activeChannels;

	static DUCKENGINE_API std::unordered_map<std::string, FMOD::DSP*> dspCache;
	static DUCKENGINE_API std::unordered_map<std::string, float> soundVolumes;
};

