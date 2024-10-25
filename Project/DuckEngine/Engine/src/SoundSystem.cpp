#include "SoundSystem.h"
#include "SoundComponent.h"
#include <iostream>

void SoundSystem::Start() {
    FMOD_RESULT result = FMOD::System_Create(&fmodSystem);
    if (result != FMOD_OK) {
        std::cerr << "FMOD system creation failed with error code: " << result << std::endl;
        return;
    }
    result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
        std::cerr << "FMOD system initialization failed with error code: " << result << std::endl;
        return;
    }
}

void SoundSystem::Update() {
    
    // Loop through all entities that have a SoundComponent
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
        SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
        if (soundComponent && soundComponent->sound == nullptr) {
            // Load sound if it hasn't been loaded yet
            LoadSoundForComponent(soundComponent);
        }

        if (soundComponent && soundComponent->playOnStart && soundComponent->channel == nullptr) {
            // Play sound if it's set to play on start and isn't playing already
            PlaySounds(soundComponent);
        }
    }

    // Update FMOD system to handle sound updates
    fmodSystem->update();
}


void SoundSystem::LoadSoundForComponent(SoundComponent* soundComp) {
    FMOD_MODE mode = soundComp->loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    fmodSystem->createSound(soundComp->soundName.c_str(), mode, nullptr, &soundComp->sound);
}

void SoundSystem::PlaySounds(SoundComponent* soundComp) {
    fmodSystem->playSound(soundComp->sound, nullptr, false, &soundComp->channel);
    soundComp->channel->setVolume(soundComp->volume);
}

void SoundSystem::StopSounds(SoundComponent* soundComp) {
    if (soundComp->channel) {
        soundComp->channel->stop();
        soundComp->channel = nullptr;
    }
}
