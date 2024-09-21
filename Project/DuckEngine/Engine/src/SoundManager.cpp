#include "SoundManager.h"
#include <iostream>

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::Initialize() {
    FMOD::System_Create(&fmodSystem);  // Create the FMOD system object
    fmodSystem->init(512, FMOD_INIT_NORMAL, 0);  // Initialize FMOD with max 512 channels
}

void SoundManager::LoadSound(const std::string& soundName, const std::string& filePath) {
    FMOD::Sound* newSound = nullptr;
    FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT, 0, &newSound);
    if (result != FMOD_OK) {
        std::cerr << "Error loading sound: " << filePath << std::endl;
        return;
    }
    soundMap[soundName] = newSound;
}

void SoundManager::PlaySound(const std::string& soundName) {

    if (soundMap.find(soundName) != soundMap.end()) {
        FMOD::Sound* sound = soundMap[soundName];
        fmodSystem->playSound(sound, 0, false, 0);
    }
    else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}

void SoundManager::Exit() {
    for (auto& pair : soundMap) {
        pair.second->release();  // Release all sounds
    }
    fmodSystem->close();  // Close FMOD system
    fmodSystem->release();  // Release FMOD system
}
