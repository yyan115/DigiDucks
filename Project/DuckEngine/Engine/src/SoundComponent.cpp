/******************************************************************************/
/*!
\file     SoundComponent.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the SoundComponent class
		  which is responsible for playing and stopping sounds in the game.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "DuckEngine.h"
#include "SoundComponent.h"

// Check if the sound is currently playing
bool SoundComponent::IsSoundPlaying() const {
    bool isPlaying = false;
    if (channel) {
        channel->isPlaying(&isPlaying);
    }
    return isPlaying;
}

// Play the sound associated with this component
void SoundComponent::Play() {
    FMOD::Sound* sound = DuckEngine::DUCKENGINE_AssetManager.GetSounds(soundID);
    if (!sound || !DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()) return;

    // Stop the current sound on this channel if playing
    if (channel) {
        channel->stop();
    }

    sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

    // Play the new sound on a fresh channel
    FMOD::Channel* newChannel = nullptr;
    DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()->playSound(sound, nullptr, false, &newChannel);
    if (newChannel) {
        newChannel->setVolume(volume);
    }
    channel = newChannel;
}

// Stop the sound if it is playing
void SoundComponent::Stop() {
    if (channel) {
        channel->stop();
        channel = nullptr;
    }
}
