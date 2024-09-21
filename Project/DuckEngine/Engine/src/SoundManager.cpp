#include "SoundManager.h"
#include "InputManager.h"
#include <iostream>

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::Initialize() {
    FMOD_RESULT result = FMOD::System_Create(&fmodSystem);  // Create the FMOD system object
    if (result != FMOD_OK) {
        std::cerr << "FMOD system creation failed with error code: " << result << std::endl;
        return;
    }
    result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);  // Initialize FMOD with max 512 channels
    if (result != FMOD_OK) {
        std::cerr << "FMOD system initialization failed with error code: " << result << std::endl;
        return;
    }
}


void SoundManager::LoadSound(const std::string& soundName, const std::string& filePath) {
    FMOD::Sound* newSound = nullptr;
    FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT, 0, &newSound);
    if (result != FMOD_OK) {
        std::cerr << "Error loading sound: " << filePath << std::endl;
        return;
    }
    soundMap[soundName] = newSound;

    // Disable looping if not needed
    newSound->setMode(FMOD_LOOP_OFF);
}


void SoundManager::PlaySound(const std::string& soundName) {
    // Check if the sound exists
    if (soundMap.find(soundName) == soundMap.end()) {
        std::cerr << "Sound not found: " << soundName << std::endl;
        return;
    }

    FMOD::Sound* sound = soundMap[soundName];
    FMOD::Channel* channel = nullptr;

    // Play the sound and store the channel
    fmodSystem->playSound(sound, 0, false, &channel);

    // Add the channel to the map so it can be stopped later
    channelMap[soundName] = channel;
}



void SoundManager::StopSound(const std::string& soundName) {
    if (soundName.empty()) {
        // Stop all sounds if no soundName is provided
        for (auto& pair : channelMap) {
            if (pair.second) {
                bool isPlaying = false;
                pair.second->isPlaying(&isPlaying);

                if (isPlaying) {
                    pair.second->stop();
                    std::cout << "Stopped sound: " << pair.first << std::endl;
                }
            }
        }
        std::cout << "All sounds stopped." << std::endl;
    }
    else {
        // Stop all channels playing the specific sound
        for (auto& pair : channelMap) {
            if (pair.first == soundName && pair.second) {
                bool isPlaying = false;
                pair.second->isPlaying(&isPlaying);

                if (isPlaying) {
                    pair.second->stop();
                    std::cout << "Stopped sound: " << soundName << std::endl;
                }
            }
        }
    }
}




void SoundManager::Update() {
    static bool isKeyPressedH = false;  // Track state of 'H' key
    static bool isKeyPressedK = false;  // Track state of 'K' key

    // Handle 'H' key to play "TestSound"
    if (InputManager::IsKeyPressed(74)) {
        std::cout << "Key H is pressed" << std::endl;
        if (!isKeyPressedH) {
            // Play the sound only once when the key is initially pressed
            SoundManager::GetInstance().PlaySound("TestSound2");
            isKeyPressedH = true;  // Set flag to indicate key is pressed
        }
    }
    else {
        isKeyPressedH = false;  // Reset flag when key is released
    }

    // Handle 'K' key to stop "TestSound2"
    if (InputManager::IsKeyReleased(75)) {
        if (!isKeyPressedK) {
            // Stop the sound only once when the key is initially pressed
            SoundManager::GetInstance().StopSound();
            isKeyPressedK = true;  // Set flag to indicate key is pressed
        }
    }
    else {
        isKeyPressedK = false;  // Reset flag when key is released
    }

    // Update FMOD system
    fmodSystem->update();
}

void SoundManager::Exit() {
    for (auto& pair : soundMap) {
        pair.second->release();  // Release all sounds
    }

    // Release all channels by stopping them first
    for (auto& pair : channelMap) {
        if (pair.second) {
            pair.second->stop();
        }
    }

    fmodSystem->close();  // Close FMOD system
    fmodSystem->release();  // Release FMOD system
}

