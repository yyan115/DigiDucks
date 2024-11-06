/******************************************************************************/
/*!
\file     SoundSystem.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the SoundSystem class
		  which is responsible for managing the sound system in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
    /****************************************************************
	* @brief Initializes the FMOD system
    *
    * @return void
    ***************************************************************/
    void Start() override;

    /****************************************************************
	* @brief Updates the FMOD system
    *
    * @return void
    ***************************************************************/
    void Update() override;

    /****************************************************************
	* @brief Stops all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static void StopAllSounds();

    /****************************************************************
	* @brief Pauses all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static void PauseAllSounds();

    /****************************************************************
	* @brief Resumes all sounds in the scene
    *
    * @return void
    ***************************************************************/
    static void ResumeAllSounds();
};

