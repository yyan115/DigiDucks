/******************************************************************************/
/*!
\file     SoundSystem.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the SoundSystem class
          which is responsible for managing the sound system in the engine.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "AssetManager.h"
#include "SoundSystem.h"
#include "SoundComponent.h"
#include "SceneWindow.h"
#include "ProjectSettings.h"
#include <iostream>
#include <thread>

float SoundSystem::masterVolume;
std::unordered_map<std::string, std::string> SoundSystem::soundCategories;
std::unordered_map<std::string, float> SoundSystem::categoryVolumes;
std::unordered_map<std::string, FMOD::Channel*> SoundSystem::activeChannels;
std::unordered_map<std::string, FMOD::DSP*> SoundSystem::dspCache;

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

	masterVolume = ProjectSettings::GetMasterVolume();
	SetMasterVolume(masterVolume);
    categoryVolumes.clear();
    activeChannels.clear();

    // Set default volumes for categories
	categoryVolumes["Default"] = ProjectSettings::GetVolumeCategory("Default");
    categoryVolumes["BGM"] = ProjectSettings::GetVolumeCategory("BGM");
    categoryVolumes["SFX"] = ProjectSettings::GetVolumeCategory("SFX");
    categoryVolumes["UI"] = ProjectSettings::GetVolumeCategory("UI");
	for (const auto& [category, volume] : categoryVolumes) {
		SetCategoryVolume(category, volume);
		//.std::cout << "Category: " << category << " Volume: " << volume << std::endl;
	}
}


void SoundSystem::Update() {
    AssetManager::GetFMODSystem()->update();

    SetMasterVolume(masterVolume);
    for (const auto& [category, volume] : categoryVolumes) SetCategoryVolume(category, volume);
}

FMOD::Channel* SoundSystem::PlaySounds(const std::string& soundID, bool loop, float volume, const std::string& category, const std::pair<std::string, float>& effects) {
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
        float categoryVolume = categoryVolumes[category];
        channel->setVolume(volume * categoryVolume * masterVolume);

        sound->setMode(FMOD_2D);

        RemoveEffect(channel); // Remove any existing DSP effects
        ApplyEffect(channel, effects);

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

void SoundSystem::ApplyEffect(FMOD::Channel* channel, const std::pair<std::string, float>& effects) {
    if (!channel || effects.first == "Default") return;  // Skip if no effect needed

    const std::string& effectName = effects.first;  // Effect type (e.g., "Reverb")
    float effectValue = effects.second;            // Effect intensity (e.g., 0.3)

    // Check if the effect is already created in the cache
    if (dspCache.find(effectName) == dspCache.end()) {
        FMOD::DSP* dspEffect = nullptr;
        FMOD_RESULT result;

        if (effectName == "Reverb") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, effectValue); // Scale decay time
            }
        }
        else if (effectName == "LowPass") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, effectValue); // Lower cutoff for stronger effect
            }
        }
        else if (effectName == "HighPass") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, effectValue); // Increase cutoff for stronger effect
            }
        }
        else if (effectName == "Flange") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_FLANGE_MIX, effectValue);
            }
        }
        else if (effectName == "Distortion") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, effectValue);
            }
        }
        else if (effectName == "Delay") {
            result = AssetManager::GetFMODSystem()->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEffect);
            if (result == FMOD_OK) {
                dspEffect->setParameterFloat(FMOD_DSP_ECHO_DELAY, effectValue); // Scale delay time
            }
        }

        // If DSP effect is valid, store it in the cache
        if (dspEffect) {
            dspCache[effectName] = dspEffect;
        }
    }

    // Apply the cached DSP effect
    if (dspCache[effectName]) {
        bool dspAlreadyAdded = false;
        int numDSPs = 0;
        channel->getNumDSPs(&numDSPs);

        // Check if DSP is already applied to the channel
        for (int i = 0; i < numDSPs; i++) {
            FMOD::DSP* existingDSP = nullptr;
            channel->getDSP(i, &existingDSP);
            if (existingDSP == dspCache[effectName]) {
                dspAlreadyAdded = true;
                break;
            }
        }

        if (!dspAlreadyAdded) {
            channel->addDSP(0, dspCache[effectName]);  // Add DSP at the start of the DSP chain
        }
    }
}

void SoundSystem::RemoveEffect(FMOD::Channel* channel) {
    if (!channel) return;

    int numDSPs = 0;
    channel->getNumDSPs(&numDSPs);

    if (numDSPs == 0) return; // No DSPs to remove

    // Iterate through all DSPs and remove them
    for (int i = 0; i < numDSPs; i++) {
        FMOD::DSP* existingDSP = nullptr;
        if (channel->getDSP(i, &existingDSP) == FMOD_OK && existingDSP) {
            channel->removeDSP(existingDSP);
        }
    }
}
