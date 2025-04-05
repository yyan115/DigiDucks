/******************************************************************************/
/*!
\file     CutSceneLogic.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the CutSceneLogic class
		  which is responsible for handling the cutscene logic in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "CutSceneLogic.h"
#include "GameLogicManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "AssetManager.h"
#include "Level0.h"
#include "GameManager.h"

bool clearCutScene = false;

void CutSceneLogic::Start()
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
		FadeOutSprite->isVisible = true;
		FadeOutSprite->color.a = 255; // Start fully visible
	}

	auto DialogueEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("DialogueManager").get();
	if (DialogueEntity)
	{
		DialogueSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueEntity->entityID);
		DialogueSprite->isVisible = false; // Hide initially
	}

	

	currentCutsceneIndex = 0;
	currentDialogueIndex = 0;
	cutsceneTimer = 0.0f;
	fadeProgress = 0.0f;
	DialoguefadeProgress = 0.0f;
	isFading = true; // Start with a fade-in effect
	isCutSceneFading = false;
	isPlaying = true;
	isShowingDialogue = false;

	CutSceneButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneSkip").get();

	if (CutSceneButton)
	{
		auto CutSceneSkip = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(CutSceneButton->entityID);
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
		auto CutSceneSkipSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
		CutSceneSkip->onClick = [this, CutSceneSkipSound]()
		{
			CutSceneSkipSound->Play();
			
			if (lastPlayedSceneName == "Level0")
			{
				currentCutsceneIndex = 12;
				std::cout << "Cutscene skipped! " << lastPlayedSceneName << " " << currentCutsceneIndex << std::endl;
			}
			else if (lastPlayedSceneName == "Level1_5") currentCutsceneIndex = 2;
			else if (lastPlayedSceneName == "Level2_5") currentCutsceneIndex = 3;
			
		};
	}

	DialogueButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("DialogueSkip").get();
	

	if (DialogueButton)
	{
		auto DialogueSkip = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(DialogueButton->entityID);
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = false;
		DialogueSkip->onClick = [this]()
		{
			currentDialogueIndex = 12;
		};
	}
}

void CutSceneLogic::Update()
{
	if (!isPlaying && !isShowingDialogue) return;

	// Handle fade-in effect at the start
	if (isFading && FadeOutSprite)
	{
		CutSceneBGM->Play(); // Play the cutscene BGM
		fadeProgress += DuckEngine::DeltaTime() / 0.5f; // 0.5s fade duration

		if (fadeProgress >= 1.0f)
		{
			FadeOutSprite->color.a = 0; // transparent
			FadeOutSprite->isVisible = false;
			isFading = false;
			fadeProgress = 0.0f;
		}
		else
		{
			FadeOutSprite->color.a = 255.0f * (1.0f - fadeProgress);
			return;
		}
	}

	// If playing cutscene ( MUST SELECT LEVEL FROM LEVEL SELECT SCREEN FIRST )
	if (isPlaying && lastPlayedSceneName == "Level0" && currentCutsceneIndex < 13) // Tutorial 0
	{
		// Check for gamepad Start or A button to skip cutscene
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
			(DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A)))
		{
			auto sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
			if (sound) sound->Play();
			currentCutsceneIndex = 12;
			std::cout << "Cutscene skipped with gamepad! " << lastPlayedSceneName << " " << currentCutsceneIndex << std::endl;
		}

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();
		std::cout << "custscene timer: " << cutsceneTimer << std::endl;
		// Change scene every 1.5 seconds
		if (cutsceneTimer >= 1.5f)
		{
			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer
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

			if (currentCutsceneIndex == 13) {
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0; // Start fade
				cutsceneTimer = 0.0f;
			}
		}
	}
	else if (isPlaying && lastPlayedSceneName == "Level1_5" && currentCutsceneIndex < 3) // Tutorial 1.5
	{
		// Check for gamepad Start or A button to skip cutscene
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
			(DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A)))
		{
			auto sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
			if (sound) sound->Play();
			currentCutsceneIndex = 2;
			std::cout << "Cutscene skipped with gamepad! " << lastPlayedSceneName << " " << currentCutsceneIndex << std::endl;
		}

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();
		std::cout << "custscene timer: " << cutsceneTimer << std::endl;
		// Change scene every 1.5 seconds
		if (cutsceneTimer >= 1.5f)
		{
			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer
			// Update cutscene sprite
			if (CutSceneSprite)
			{
				std::string cutscenePath = "Resources/Sprites/cutscene/Level2/" + std::to_string(currentCutsceneIndex) + ".jpg";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}

			// Play sound effect for the scene
			if (CutSceneSFX)
			{
				//CutSceneSFX->Play(currentCutsceneIndex);
			}

			if (currentCutsceneIndex == 3) {
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0; // Start fade
				cutsceneTimer = 0.0f;
			}
		}
	}
	else if (isPlaying && lastPlayedSceneName == "Level2_5" && currentCutsceneIndex < 4) // Tutorial 2.5
	{
		// Check for gamepad Start or A button to skip cutscene
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
			(DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A)))
		{
			auto sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
			if (sound) sound->Play();
			currentCutsceneIndex = 3;
			std::cout << "Cutscene skipped with gamepad! " << lastPlayedSceneName << " " << currentCutsceneIndex << std::endl;
		}

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();
		std::cout << "custscene timer: " << cutsceneTimer << std::endl;
		// Change scene every 1.5 seconds
		if (cutsceneTimer >= 1.5f)
		{
			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer
			// Update cutscene sprite
			if (CutSceneSprite)
			{
				std::string cutscenePath = "Resources/Sprites/cutscene/Level3/" + std::to_string(currentCutsceneIndex) + ".jpg";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}

			// Play sound effect for the scene
			if (CutSceneSFX)
			{
				//CutSceneSFX->Play(currentCutsceneIndex);
			}

			if (currentCutsceneIndex == 4) {
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0; // Start fade
				cutsceneTimer = 0.0f;
			}
		}
	}

	else if (isPlaying && lastPlayedSceneName == "Level3" && currentCutsceneIndex < 8 && GameManager::GameCleared) // Tutorial 2.5
	{
		//DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();
		CutSceneSprite->isVisible = true;
		clearCutScene = true;

		//std::cout << "custscene timer: " << cutsceneTimer << std::endl;
		// Change scene every 1.5 seconds
		if (cutsceneTimer >= 1.5f)
		{
			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer
			// Update cutscene sprite
			if (CutSceneSprite)
			{
				std::string cutscenePath = "Resources/Sprites/cutscene/Clear/" + std::to_string(currentCutsceneIndex) + ".png";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}

			// Play sound effect for the scene
			if (CutSceneSFX)
			{
				//CutSceneSFX->Play(currentCutsceneIndex);
			}

			if (currentCutsceneIndex == 4) {
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
			if (lastPlayedSceneName == "Level0")
			{
				isShowingDialogue = true;
				DialogueSprite->isVisible = true; // Show dialogue box
			}
			else
			{
				CutSceneBGM->Stop();
				isPlaying = false; // End cutscene
			}
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


	// Handle dialogues
	if (isShowingDialogue && lastPlayedSceneName == "Level0")
	{
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = true;

		// Check for gamepad Start button to skip all dialogue
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
			(DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START)))
		{
			currentDialogueIndex = 12;
			std::cout << "Dialogue skipped with gamepad Start button!" << std::endl;
		}

		// Check for user input to progress dialogue
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
			(DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A)))
		{
			currentDialogueIndex++;

			if (currentDialogueIndex == 14)
			{
				FadeOutSprite->isVisible = false;
				DialogueSprite->isVisible = false;
				isShowingDialogue = false;
				isPlaying = false;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = false;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneEntity->entityID)->isVisible = false;
				CutSceneBGM->Stop();

				return;
			}

			// Update dialogue texture
			std::string dialoguePath = "Resources/Sprites/Dialogues/intro/" + std::to_string(currentDialogueIndex) + ".png";
			DialogueSprite->texture = *AssetManager::GetTexture(dialoguePath).get();

			// Play sound effect for dialogues between soundtrack 15 and 16
			if (CutSceneSFX)
			{
				CutSceneSFX->Play(14 + (std::rand() % 2));
			}


		}
	}
}
bool CutSceneLogic::CutscenePlay()
{
	return isPlaying;
}
