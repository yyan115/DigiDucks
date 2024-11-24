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
}


void SoundSystem::Update() {
    AssetManager::GetFMODSystem()->update();
}


void SoundSystem::StopAllSounds() {
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
        SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
        if (soundComponent) {
            soundComponent->Stop();  // Stop each sound
		}
    }
    std::cout << "All sounds stopped." << std::endl;
}

void SoundSystem::PauseAllSounds() {
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
        SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
        if (soundComponent) {
            soundComponent->Pause();
        }
    }
    std::cout << "All sounds paused." << std::endl;
}

void SoundSystem::ResumeAllSounds() {
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
        SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
        if (soundComponent) {
            soundComponent->Resume();
        }
    }
    std::cout << "All sounds resumed." << std::endl;
}

