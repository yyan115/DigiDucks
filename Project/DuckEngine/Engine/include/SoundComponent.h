/******************************************************************************/
/*!
\file     SoundComponent.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the SoundComponent class
          which is responsible for playing and stopping sounds in the game.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Component.h"
#include <string>
#include <fmod.hpp>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class SoundComponent : public Component {
public:
    std::vector<std::string> soundID;          // Identifier or path for preloaded sound
    FMOD::Channel* channel;       // Channel for playback control
	std::string category;		  // Sound category
    bool loop;                    // Whether to loop the sound
    bool playOnStart;             // Play sound when the entity is created
    float volume;                 // Sound volume

    /****************************************************************
	* @brief Constructor for the SoundComponent class
    *
    * @return void
    ***************************************************************/
    SoundComponent(const std::vector<std::string>& _soundID = {}, const std::string& _category = "Default",bool _loop = false, bool _playOnStart = false, float _volume = 1.0f)
        : soundID(_soundID), category(_category), channel(nullptr), loop(_loop), playOnStart(_playOnStart), volume(_volume) {}

    /****************************************************************
	* @brief Clone function for the SoundComponent class
    *
	* @return std::shared_ptr<Component> A shared pointer to the cloned component
    ***************************************************************/
    std::shared_ptr<Component> Clone() const override {
        return std::make_shared<SoundComponent>(*this);
    }

    /****************************************************************
	* @brief Check if the sound is currently playing
    *
	* @return bool True if the sound is playing, false otherwise
    ***************************************************************/
    DUCKENGINE_API bool IsSoundPlaying() const;

    /****************************************************************
	* @brief Play the sound associated with this component
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void Play(int index = 0);

    /****************************************************************
	* @brief Stop the sound if it is playing
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void Stop();

    /****************************************************************
    * @brief Pause the sound if it is playing
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void Pause();

    /****************************************************************
    * @brief Resume the sound if it is not playing
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void Resume();
};