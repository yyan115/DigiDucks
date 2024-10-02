/******************************************************************************/
/*!
\file    DuckEngine_Sound.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration to handle sound effects in the Engine

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "string"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif



class DUCKENGINE_API DuckEngine_Sound
{
public:
    /****************************************************************
    * @brief Loads a sound file and associates it with a name for later playback
    *
    * @param soundName - The name to reference the sound
    * @param filePath - The file path to the sound file
    *
    * @return void
    ***************************************************************/
	static void LoadSound(const std::string& soundName, const std::string& filePath);

    /****************************************************************
    * @brief Plays a sound that has been loaded into the SoundManager
    *
    * @param soundName - The name of the sound to play
    *
    * @return void
    ***************************************************************/
	static void PlaySounds(const std::string& soundName);

    /****************************************************************
    * @brief Stops the playback of a specific sound or all sounds if no name is provided
    *
    * @param soundName - The name of the sound to stop. If empty, stops all sounds.
    *
    * @return void
    ***************************************************************/
	static void StopSound(const std::string& soundName = "");
};