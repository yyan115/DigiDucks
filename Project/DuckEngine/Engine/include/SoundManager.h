/******************************************************************************/
/*!
\file    SoundManager.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration to handle sound effects in the game

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    /****************************************************************
    * @brief Retrieves the instance of the SoundManager
    *
    * @return A reference to the SoundManager instance
    ***************************************************************/
    static SoundManager& GetInstance();

    /****************************************************************
    * @brief Initializes the FMOD sound system for managing audio
    *
    * @return void
    ***************************************************************/
    void Initialize();

    /****************************************************************
   * @brief Updates the FMOD sound system
   * 
   * @return void
   ***************************************************************/
    void Update();

    /****************************************************************
    * @brief Loads a sound file and associates it with a name for later playback
    *
    * @param soundName - The name to reference the sound
    * @param filePath - The file path to the sound file
    *
    * @return void
    ***************************************************************/
    void LoadSound(const std::string& soundName, const std::string& filePath);

    /****************************************************************
    * @brief Plays a sound that has been loaded into the SoundManager
    *
    * @param soundName - The name of the sound to play
    *
    * @return void
    ***************************************************************/
    void PlaySounds(const std::string& soundName);

    /****************************************************************
    * @brief Stops the playback of a specific sound or all sounds if no name is provided
    *
    * @param soundName - The name of the sound to stop. If empty, stops all sounds.
    *
    * @return void
    ***************************************************************/
    void StopSound(const std::string& soundName = "");

    /****************************************************************
    * @brief Exits the FMOD sound system
    * 
    * @return void
    * ***************************************************************/
    void Exit();

private:
    SoundManager() = default;
    ~SoundManager() = default;

    FMOD::System* fmodSystem = nullptr;
    std::unordered_map<std::string, FMOD::Sound*> soundMap;
    std::unordered_map<std::string, FMOD::Channel*> channelMap;  // Store channels for each sound
};

#endif
