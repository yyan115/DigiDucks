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

    // Stop the current sound on this channel if it's playing
    if (channel) {
        channel->stop();
    }

    // Play the new sound on a fresh channel
    FMOD::Channel* newChannel = nullptr;
    DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()->playSound(sound, nullptr, false, &newChannel);
    newChannel->setVolume(volume);
    channel = newChannel;
}

// Stop the sound if it is playing
void SoundComponent::Stop() {
    if (channel) {
        channel->stop();
        channel = nullptr;
    }
}
