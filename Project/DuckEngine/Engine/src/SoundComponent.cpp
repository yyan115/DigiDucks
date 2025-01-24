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
#include "SoundSystem.h"
#include "SoundComponent.h"

// Check if the sound is currently playing
bool SoundComponent::IsSoundPlaying() const {
	return SoundSystem::IsSoundPlaying(soundID);
}

// Play the sound associated with this component
void SoundComponent::Play() {    
    SoundSystem::PlaySounds(soundID, loop, volume);
}

void SoundComponent::PlayHold() {
    FMOD::Sound* sound = DuckEngine::DUCKENGINE_AssetManager.GetSounds(soundID);
    if (!sound || !DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()) return;

    if (IsSoundPlaying()) return;

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
    SoundSystem::StopSounds(soundID);
}

// Pause the sound if it is playing
void SoundComponent::Pause() {
    SoundSystem::PauseSound(soundID);
}

// Resume the sound if it is paused
void SoundComponent::Resume() {
	SoundSystem::ResumeSound(soundID);
}
