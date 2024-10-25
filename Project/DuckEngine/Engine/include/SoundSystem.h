

#pragma once
#include "System.h"
#include "DuckEngine.h"
#include <fmod.hpp>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API SoundSystem : public System {
public:
    FMOD::System* fmodSystem = nullptr;

    void Start() override;
    void Update() override;

    // Load sound data for a sound component
    void LoadSoundForComponent(class SoundComponent* soundComp);

    // Play sound attached to the entity
    void PlaySounds(class SoundComponent* soundComp);

    // Stop sound attached to the entity
    void StopSounds(class SoundComponent* soundComp);
};

