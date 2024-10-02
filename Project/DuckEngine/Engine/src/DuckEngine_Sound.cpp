/******************************************************************************/
/*!
\file    DuckEngine_Sound.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation to handle sound effects in the Engine

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
