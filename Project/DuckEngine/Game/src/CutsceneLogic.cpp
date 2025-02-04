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
#include "DuckEngine.h"
#include "AssetManager.h"

void CutSceneLogic::Start()
{
	auto CutSceneEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneManager").get();
	if (CutSceneEntity)
	{
		CutSceneSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneEntity->entityID);
		CutSceneSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneEntity->entityID);
	}

	auto FadeEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameFadeScreen");
	if (FadeEntity) {
		FadeOutSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeEntity->entityID);
		FadeOutSprite->isVisible = true;
		FadeOutSprite->color.a = 255; // Start fully visible
	}

	currentCutsceneIndex = 1;
	cutsceneTimer = 0.0f;
	fadeProgress = 0.0f;
	isFading = true; // Start with a fade-in effect
	isPlaying = true;
}

void CutSceneLogic::Update()
{
	if (!isPlaying) return;

	// Handle fade-in effect at the start
	if (isFading)
	{
		fadeProgress += DuckEngine::DeltaTime() / 0.5f; // 0.5s fade duration

		if (fadeProgress >= 1.0f)
		{
			FadeOutSprite->color.a = 0; // Fully transparent
			FadeOutSprite->isVisible = false;
			isFading = false;
			fadeProgress = 0.0f;
		}
		else
		{
			FadeOutSprite->color.a = static_cast<int>(255 * (1.0f - fadeProgress));
			return; // Don't progress the cutscene while fading in
		}
	}

	// Accumulate time for cutscene transition
	cutsceneTimer += DuckEngine::DeltaTime();

	// Change scene every 1.5 seconds
	if (cutsceneTimer >= 1.5f)
	{
		cutsceneTimer = 0.0f;  // Reset timer

		// Check if it's time to end the cutscene
		if (currentCutsceneIndex > 14)
		{
			// Start fade-out effect before finishing
			FadeOutSprite->isVisible = false;
			isFading = true;
			isPlaying = false;
			return;
		}

		// Update cutscene sprite
		if (CutSceneSprite)
		{
			std::string cutscenePath = "Resources/Sprites/cutscene/" + std::to_string(currentCutsceneIndex) + ".png";
			CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
		}

		// Play sound effect for the scene
		if (CutSceneSFX)
		{
			CutSceneSFX->Play(currentCutsceneIndex);
		}

		// Move to the next scene
		currentCutsceneIndex++;

		// Enable fade-in effect after each scene change
		FadeOutSprite->isVisible = false;
		FadeOutSprite->color.a = 255;
		isFading = true;
		fadeProgress = 0.0f;
	}
}

bool CutSceneLogic::CutscenePlay()
{
	return isPlaying;
}
