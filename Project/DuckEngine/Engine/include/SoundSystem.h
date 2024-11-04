

#pragma once
#include "System.h"
#include "DuckEngine.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API SoundSystem : public System {
public:
    

    void Start() override;
    void Update() override;
    static void StopAllSounds();
    static void PauseAllSounds();
    static void ResumeAllSounds();
};

