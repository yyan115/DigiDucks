//---------------------------------------------------------
// File:    SoundManager.h
//authors:	Muhammad Zikry Bin Zakaria
// email:	muhammadzikry.b\@digipen.edu
// 
//
// Brief:     Contains the declaration that is used to manage sound in the game
//
// Copyright © 2024 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <fmod.hpp>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    static SoundManager& GetInstance();

    void Initialize();
    void Update();
    void LoadSound(const std::string& soundName, const std::string& filePath);
    void PlaySound(const std::string& soundName);
    void StopSound(const std::string& soundName = "");  // Stop a specific sound or all sounds
    void Exit();

private:
    SoundManager() = default;
    ~SoundManager() = default;

    FMOD::System* fmodSystem = nullptr;
    std::unordered_map<std::string, FMOD::Sound*> soundMap;
    std::unordered_map<std::string, FMOD::Channel*> channelMap;  // Store channels for each sound
};

#endif
