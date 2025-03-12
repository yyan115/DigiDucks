/******************************************************************************/
/*!
\file     SoundComponent.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the SoundComponent class
		  which is responsible for playing and stopping sounds in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
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
	for (const auto& sound : soundID) {
		if (SoundSystem::IsSoundPlaying(sound)) {
			return true;
		}
	}
	return false;
}

// Play a sound from the list
void SoundComponent::Play(int index) {
    if (soundID.empty()) {
        return;
    }

    // If index is -1, pick a random sound
    if (index == -1) {
        index = std::rand() % soundID.size();
    }

    // Ensure index is valid
    if (index >= 0 && index < soundID.size()) {
        // Check if a sound is already playing on this component
        bool isPlaying = false;
        if (channel) {
            channel->isPlaying(&isPlaying);
        }

        // Play a new sound only if the current one has finished
        if (!isPlaying) {
            channel = SoundSystem::PlaySounds(soundID[index], loop, volume, category, effects);
        }
    }
    else {
        std::cerr << "Invalid sound index: " << index << std::endl;
    }
}


//void SoundComponent::PlayHold() {
//    FMOD::Sound* sound = DuckEngine::DUCKENGINE_AssetManager.GetSounds(soundID);
//    if (!sound || !DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()) return;
//
//    if (IsSoundPlaying()) return;
//
//    sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
//
//    // Play the new sound on a fresh channel
//    FMOD::Channel* newChannel = nullptr;
//    DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()->playSound(sound, nullptr, false, &newChannel);
//    if (newChannel) {
//        newChannel->setVolume(volume);
//    }
//    channel = newChannel;
//}

// Stop all sounds
void SoundComponent::Stop() {
    if (soundID.empty()) return;

    for (const auto& sound : soundID) {
        SoundSystem::StopSounds(sound);
    }

    channel = nullptr;  // Clear channel reference
}

// Pause all sounds
void SoundComponent::Pause() {
    for (const auto& sound : soundID) {
        SoundSystem::PauseSound(sound);
    }
}

// Resume all sounds
void SoundComponent::Resume() {
    for (const auto& sound : soundID) {
        SoundSystem::ResumeSound(sound);
    }
}