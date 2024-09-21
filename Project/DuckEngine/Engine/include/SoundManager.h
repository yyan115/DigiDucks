#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <string>
#include <map>

class SoundManager {
public:
    static SoundManager& GetInstance();

    void Initialize();
    void LoadSound(const std::string& soundName, const std::string& filePath);
    void PlaySound(const std::string& soundName);
    void Exit();

private:
    SoundManager() = default;
    ~SoundManager() = default;

    FMOD::System* fmodSystem;
    std::map<std::string, FMOD::Sound*> soundMap;
};

#endif
