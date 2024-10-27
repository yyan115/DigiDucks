#pragma once
#include "Component.h"
#include <string>
#include <fmod.hpp>

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
    DUCKENGINE_API bool IsSoundPlaying() const;

    // Play the sound associated with this component
    DUCKENGINE_API void Play();

    // Stop the sound if it is playing
    DUCKENGINE_API void Stop();
};