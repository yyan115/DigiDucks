#include "DuckEngine_Sound.h"
#include "SoundManager.h"


void DuckEngine_Sound::LoadSound(const std::string& soundName, const std::string& filePath)
{
	SoundManager::GetInstance().LoadSound(soundName, filePath);
}

void DuckEngine_Sound::PlaySounds(const std::string& soundName)
{
	SoundManager::GetInstance().PlaySounds(soundName);
}

void DuckEngine_Sound::StopSound(const std::string& soundName)
{
	SoundManager::GetInstance().StopSound(soundName);
}
