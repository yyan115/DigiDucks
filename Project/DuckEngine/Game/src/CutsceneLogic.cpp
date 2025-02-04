/******************************************************************************/
/*!
\file       RestockLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Definition of the Restock Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "CutSceneLogic.h"
#include "GameLogicManager.h"

/****************************************************************
* @brief Start function for the Restock Station Logic.
* ****************************************************************/
void CutSceneLogic::Start()
{
	auto CutSceneEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneManager").get();
	if (CutSceneEntity)
	{
		CutSceneSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneEntity->entityID);
		CutSceneSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneEntity->entityID);
	}

	currentCutsceneIndex = 1;  // Start at scene 1
	cutsceneTimer = 0.0f;      // Reset timer
	isPlaying = true;
}


/****************************************************************
* @brief Update function for the Restock Station Logic.
* ****************************************************************/
void CutSceneLogic::Update()
{
	if (!isPlaying) return;

	// Accumulate time
	cutsceneTimer += DuckEngine::DeltaTime();

	// Change cutscene every 1.5 seconds (adjust timing as needed)
	if (cutsceneTimer >= 1.5f)
	{
		cutsceneTimer = 0.0f;  // Reset timer

		// Update cutscene sprite
		if (CutSceneSprite)
		{
			std::string cutscenePath = "Resources/Sprites/cutscene/" + std::to_string(currentCutsceneIndex) + ".png";
			CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
		}

		// Play sound effect
		if (CutSceneSFX)
		{
			CutSceneSFX->Play(currentCutsceneIndex);
		}

		// Move to the next scene
		currentCutsceneIndex++;

		// If reached the last scene, stop playing
		if (currentCutsceneIndex > 14)
		{
			isPlaying = false;
		}
	}
}

bool CutSceneLogic::CutscenePlay()
{
	std::cout << "Cutscene playing = " << isPlaying << std::endl;
	return isPlaying;
}