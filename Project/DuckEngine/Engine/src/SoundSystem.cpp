/******************************************************************************/
/*!
\file     SoundSystem.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the SoundSystem class
          which is responsible for managing the sound system in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "AssetManager.h"
#include "SoundSystem.h"
#include "SoundComponent.h"
#include "SceneWindow.h"
#include <iostream>
#include <thread>

float SoundSystem::masterVolume = 1.0f;
std::unordered_map<std::string, std::string> SoundSystem::soundCategories;
std::unordered_map<std::string, float> SoundSystem::categoryVolumes;
std::unordered_map<std::string, FMOD::Channel*> SoundSystem::activeChannels;


void SoundSystem::Start() {
    if (!AssetManager::GetFMODSystem()) {  // Check if fmodSystem is null
        FMOD_RESULT result = FMOD::System_Create(&AssetManager::GetFMODSystem());  // Pass reference
        if (result != FMOD_OK) {
            std::cerr << "FMOD system creation failed with error code: " << result << std::endl;
            return;
        }

        result = AssetManager::GetFMODSystem()->init(512, FMOD_INIT_NORMAL, 0);  // Initialize FMOD
        if (result != FMOD_OK) {
            std::cerr << "FMOD system initialization failed with error code: " << result << std::endl;
            return;
        }
    }
    else {
        std::cerr << "FMOD system is already initialized in AssetManager." << std::endl;
    }                                                                   

    masterVolume = 1.0f;
    categoryVolumes.clear();
    activeChannels.clear();

    // Set default volumes for categories
    categoryVolumes["Default"] = 1.0f;
    categoryVolumes["BGM"] = 1.0f;
    categoryVolumes["SFX"] = 1.0f;
    categoryVolumes["UI"] = 1.0f;
}


void SoundSystem::Update() {
    AssetManager::GetFMODSystem()->update();
}

FMOD::Channel* SoundSystem::PlaySounds(const std::string& soundID, bool loop, float volume, const std::string& category) {
    if (!AssetManager::GetFMODSystem()) return nullptr;

    FMOD::Sound* sound = AssetManager::GetSounds(soundID);
    if (!sound) {
        std::cerr << "Sound not found: " << soundID << std::endl;
        return nullptr;
    }

    // Assign soundID with its category
    if (soundCategories.find(soundID) == soundCategories.end()) {
        AddSoundToCategory(soundID, category);
    }

    sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

    FMOD::Channel* channel = nullptr;
    AssetManager::GetFMODSystem()->playSound(sound, nullptr, false, &channel);

    if (channel) {
        float categoryVolume = categoryVolumes[category]; // Get category volume
        channel->setVolume(volume * categoryVolume * masterVolume);

        // Store the channel in the active channels map
        activeChannels[soundID] = channel;
    }

    return channel; // Return the channel to the caller
}



void SoundSystem::StopSounds(const std::string& soundID) {
    auto it = activeChannels.find(soundID);
    if (it != activeChannels.end() && it->second) {
        it->second->stop();
        activeChannels.erase(it);
    }
}

void SoundSystem::StopAllSounds() {
    for (auto& [soundID, channel] : activeChannels) {
        if (channel) channel->stop();
    }
    activeChannels.clear();
}

void SoundSystem::PauseAllSounds() {
    for (auto& [soundID, channel] : activeChannels) {
        if (channel) channel->setPaused(true);
    }
}

void SoundSystem::ResumeAllSounds() {
    for (auto& [soundID, channel] : activeChannels) {
        if (channel) channel->setPaused(false);
    }
}

void SoundSystem::SetSoundVolume(const std::string& soundID, float volume) {
    auto it = activeChannels.find(soundID);
    if (it != activeChannels.end() && it->second) {
        float categoryVolume = 1.0f;
        if (soundCategories.find(soundID) != soundCategories.end()) {
            categoryVolume = categoryVolumes[soundCategories[soundID]];
        }
        it->second->setVolume(volume * categoryVolume * masterVolume);
    }
}

void SoundSystem::SetMasterVolume(float volume) {
    masterVolume = volume;
    for (auto& [soundID, channel] : activeChannels) {
        if (channel) {
            float categoryVolume = 1.0f;
            if (soundCategories.find(soundID) != soundCategories.end()) {
                categoryVolume = categoryVolumes[soundCategories[soundID]];
            }
            channel->setVolume(categoryVolume * masterVolume);
        }
    }
}

void SoundSystem::SetCategoryVolume(const std::string& category, float volume) {
    categoryVolumes[category] = volume;
    for (auto& [soundID, channel] : activeChannels) {
        if (soundCategories[soundID] == category && channel) {
            channel->setVolume(volume * masterVolume);
        }
    }
}

void SoundSystem::AddSoundToCategory(const std::string& soundID, const std::string& category) {
    soundCategories[soundID] = category;
    if (categoryVolumes.find(category) == categoryVolumes.end()) {
        categoryVolumes[category] = 1.0f;  // Default volume for new category
    }
}

bool SoundSystem::IsSoundPlaying(const std::string& soundID) {
    auto it = activeChannels.find(soundID);
    if (it != activeChannels.end() && it->second) {
        bool isPlaying = false;
        it->second->isPlaying(&isPlaying);
        return isPlaying;
    }
    return false;
}

void SoundSystem::PauseSound(const std::string& soundID) {
    auto it = activeChannels.find(soundID);
    if (it != activeChannels.end() && it->second) {
        it->second->setPaused(true);
    }
}

void SoundSystem::ResumeSound(const std::string& soundID) {
    auto it = activeChannels.find(soundID);
    if (it != activeChannels.end() && it->second) {
        it->second->setPaused(false);
    }
}

void SoundSystem::FadeOutSound(SoundComponent* soundComponent, float duration) {
    if (!soundComponent || soundComponent->soundID.empty()) return;

    const std::string& soundID = soundComponent->soundID[0];
    auto it = activeChannels.find(soundID);
    if (it == activeChannels.end() || !it->second) return;

    float initialVolume = soundComponent->volume;
    const float stepTime = 0.05f;                 // Time per step
    const int steps = static_cast<int>(duration / stepTime); // Calculate the number of steps
    const float volumeStep = initialVolume / steps;          // Volume decrement

    for (int i = 0; i < steps; ++i) {
        float newVolume = initialVolume - (volumeStep * i);
        if (newVolume < 0.0f) newVolume = 0.0f;

        SetSoundVolume(soundID, newVolume);

        // Add delay for each step
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stepTime * 1000)));
    }

    // Ensure volume is completely zero at the end
    SetSoundVolume(soundID, 0.0f);
    StopSounds(soundID);
}

