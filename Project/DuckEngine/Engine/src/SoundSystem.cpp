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
            //soundComponent.pause(true);  // Stop each sound
        }
    }
    std::cout << "All sounds paused." << std::endl;
}

void SoundSystem::ResumeAllSounds() {
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
        SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
        if (soundComponent) {
            //soundComponent.pause(false);  // Stop each sound
        }
    }
    std::cout << "All sounds resumed." << std::endl;
}

