#pragma once
#include "Component.h"
#include <string>
#include <fmod.hpp>
//#include "DuckEngine.h"
//#include "AssetManager.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class SoundComponent : public Component {
public:
    std::string soundID;          // Identifier or path for preloaded sound
    FMOD::Channel* channel;       // Channel for playback control
    bool loop;                    // Whether to loop the sound
    bool playOnStart;             // Play sound when the entity is created
    float volume;                 // Sound volume

    // Constructor
    SoundComponent(const std::string& _soundID, bool _loop = false, bool _playOnStart = false, float _volume = 1.0f)
        : soundID(_soundID), channel(nullptr), loop(_loop), playOnStart(_playOnStart), volume(_volume) {}

    std::shared_ptr<Component> Clone() const override {
        return std::make_shared<SoundComponent>(*this);
    }

    // Check if the sound is currently playing
    bool IsSoundPlaying() const {
        bool isPlaying = false;
        if (channel) {
            channel->isPlaying(&isPlaying);
        }
        return isPlaying;
    }

    // Play the sound associated with this component
    void Play() {
        //FMOD::Sound* sound = DuckEngine::DUCKENGINE_AssetManager.GetSounds(soundID);
        //if (!sound || !DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()) return;

        //// Stop the current sound on this channel if it's playing
        //if (channel) {
        //    channel->stop();
        //}

        //// Play the new sound on a fresh channel
        //FMOD::Channel* newChannel = nullptr;
        //DuckEngine::DUCKENGINE_AssetManager.GetFMODSystem()->playSound(sound, nullptr, false, &newChannel);
        //newChannel->setVolume(volume);
        //channel = newChannel;
    }

    //// Stop the sound if it is playing
    //void Stop() {
    //    if (channel) {
    //        channel->stop();
    //        channel = nullptr;
    //    }
    //}
};