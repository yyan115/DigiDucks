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

// Component to represent audio data attached to an entity
class DUCKENGINE_API SoundComponent : public Component {
public:
    std::string soundName;   // Name of the sound
    FMOD::Sound* sound;      // The actual FMOD sound object
    FMOD::Channel* channel;  // The channel that the sound will be played on

    bool loop;               // Whether the sound should loop
    bool playOnStart;        // Whether the sound should play when the entity is created
    float volume;            // Sound volume

    // Constructor
    SoundComponent(const std::string& _soundName, bool _loop = false, bool _playOnStart = false, float _volume = 1.0f)
        : soundName(_soundName), sound(nullptr), channel(nullptr), loop(_loop), playOnStart(_playOnStart), volume(_volume) {}

    std::shared_ptr<Component> Clone() const override {
        return std::make_shared<SoundComponent>(*this);
    }

    // Destructor to release sound resources
    ~SoundComponent() {
        if (sound) {
            sound->release();
        }
    }
};
