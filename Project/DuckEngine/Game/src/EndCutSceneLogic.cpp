/******************************************************************************/
/*!
\file     EndCutSceneLogic.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the EndCutSceneLogic class
		  which is responsible for handling the Endcutscene logic in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "EndCutSceneLogic.h"
#include "GameLogicManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "AssetManager.h"
#include "Level0.h"
#include "GameManager.h"

void EndCutSceneLogic::Start()
{
	lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
	CutSceneEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneManager").get();
	if (CutSceneEntity)
	{
		CutSceneSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneEntity->entityID);
		CutSceneSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneEntity->entityID);
	}
	auto CutSceneSound = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneBGM").get();
	if (CutSceneSound)
	{
		CutSceneBGM = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneSound->entityID);
	}

	auto FadeEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameFadeScreen");
	if (FadeEntity) {
		FadeOutSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeEntity->entityID);
		FadeOutSprite->isVisible = false;
		FadeOutSprite->color.a = 255; // Start fully visible
	}

	currentCutsceneIndex = 0;
	cutsceneTimer = 0.0f;
	fadeProgress = 0.0f;
	DialoguefadeProgress = 0.0f;
	isFading = true; // Start with a fade-in effect
	isCutSceneFading = false;
	isPlaying = true;

	CutSceneButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneSkip").get();

	if (CutSceneButton)
	{
		auto CutSceneSkip = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(CutSceneButton->entityID);
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
		auto CutSceneSkipSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
		CutSceneSkip->onClick = [this, CutSceneSkipSound]()
			{
				CutSceneSkipSound->Play();

				if (lastPlayedSceneName == "Level3") currentCutsceneIndex = 7;
			};
	}
}

void EndCutSceneLogic::Update()
{
	if (!isPlaying) return;

	// Check for gamepad input to skip end cutscene
	if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		// Skip cutscene with Start or A button when the skip button is visible
		if (isPlaying && lastPlayedSceneName == "Level3" &&
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible)
		{
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				// Get the sound component for skip sound
				auto CutSceneSkipSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
				if (CutSceneSkipSound) CutSceneSkipSound->Play();

				// Skip to end of cutscene
				currentCutsceneIndex = 7;
				std::cout << "End cutscene skipped with gamepad!" << std::endl;
			}
		}
	}

	// If playing cutscene ( MUST SELECT LEVEL FROM LEVEL SELECT SCREEN FIRST )
	if (isPlaying && lastPlayedSceneName == "Level3" && currentCutsceneIndex < 8) // ENDSCENE
	{
		if (!GameManager::GameCleared)
		{
			CutSceneBGM->Stop();
			CutSceneSprite->isVisible = false;
			return;
		}

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();
		CutSceneSprite->isVisible = true;
		CutSceneBGM->Play();

		// Change scene every 1.5 seconds
		if (cutsceneTimer >= 1.5f)
		{
			// Play sound effect for the scene
			if (CutSceneSFX)
			{
				CutSceneSFX->Stop();
				CutSceneSFX->Play(currentCutsceneIndex);
			}

			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer
			// Update cutscene sprite
			if (CutSceneSprite)
			{
				std::string cutscenePath = "Resources/Sprites/cutscene/Clear/" + std::to_string(currentCutsceneIndex) + ".png";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}


			if (currentCutsceneIndex == 8) {
				isCutSceneFading = true;

				cutsceneTimer = 0.0f;
			}
		}
	}

	else if (isCutSceneFading) // Handle fade before dialogue starts
	{
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
		DialoguefadeProgress += DuckEngine::DeltaTime();

		if (DialoguefadeProgress >= 6.0f)
		{
			if (FadeOutSprite)
			{
				FadeOutSprite->color.a = 255;
				FadeOutSprite->isVisible = false;

			}
			isCutSceneFading = false;
			CutSceneSprite->isVisible = false; // Hide cutscene sprite
			CutSceneBGM->Stop();
			isPlaying = false; // End cutscene
			GameManager::GameCleared = false;
		}
		else if (DialoguefadeProgress >= 1.0f)
		{
			if (FadeOutSprite)
			{
				FadeOutSprite->isVisible = true;
				float fadeRatio = (DialoguefadeProgress - 2.0f) / (6.0f - 2.0f);
				FadeOutSprite->color.a = fadeRatio * 255.0f;
			}
		}

	}
}

bool EndCutSceneLogic::CutscenePlay()
{
	return isPlaying;
}