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
		FadeOutSprite->isVisible = false;
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
	skipped = false;

	if (auto settingsPanel = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Menu"))
	{
		SettingsPanelSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(settingsPanel->entityID);
	}
	if (auto settingsGear = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn"))
	{
		SettingsGearButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(settingsGear->entityID);
		SettingsGearSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(settingsGear->entityID);
	}
	menuWasUp = false;

	CutSceneButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneSkip").get();

	if (CutSceneButton)
	{
		auto CutSceneSkip = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(CutSceneButton->entityID);
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
		auto CutSceneSkipSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
		CutSceneSkip->onClick = [this, CutSceneSkipSound]()
			{
				// The options panel covers this button's corner when it is open.
				if (MenuHasInput()) return;

				CutSceneSkipSound->Play();
				SkipPictures();
			};
	}

	DialogueButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("DialogueSkip").get();


	if (DialogueButton)
	{
		auto DialogueSkip = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(DialogueButton->entityID);
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = false;
		DialogueSkip->onClick = [this]()
			{
				// The options panel covers this button's corner when it is open.
				if (MenuHasInput()) return;

				if (isShowingDialogue) EndDialogue();
			};
	}
}

bool CutSceneLogic::MenuHasInput() const
{
	const bool up = (SettingsPanelSprite && SettingsPanelSprite->isVisible) || DuckEngine::isGamePaused;
	// A hidden button keeps the hover it had when it was hidden, since
	// ButtonSystem stops looking at it, so the gear only counts while shown.
	const bool onGear = SettingsGearButton && SettingsGearButton->isEnabled && SettingsGearButton->isHovered &&
		SettingsGearSprite && SettingsGearSprite->isVisible;
	// Up last frame counts as well. Buttons are handled before this logic
	// runs, so on the frame of a click on CLOSE or RESUME, or of the Escape
	// that shuts a menu, it already reads closed by the time that press
	// arrives here.
	return up || menuWasUp || onGear;
}

void CutSceneLogic::Update()
{
	const bool settingsHasInput = MenuHasInput();
	menuWasUp = (SettingsPanelSprite && SettingsPanelSprite->isVisible) || DuckEngine::isGamePaused;

	// No gear while the cutscene's pictures are up, and back when they go.
	if (SettingsGearSprite)
	{
		const bool pictures = isPlaying && !isShowingDialogue && CutSceneSprite && CutSceneSprite->isVisible;
		SettingsGearSprite->isVisible = !pictures;
	}

	if (!isPlaying && !isShowingDialogue) return;

	// Check for gamepad input to skip cutscene or advance dialogue
	if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) && !settingsHasInput)
	{
		// Skip cutscene with Start or A button when the skip button is visible
		if (isPlaying && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible)
		{
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				// Get the sound component for skip sound
				auto CutSceneSkipSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CutSceneButton->entityID);
				if (CutSceneSkipSound) CutSceneSkipSound->Play();
				SkipPictures();
			}
		}

		// Skip dialogue with Start button or advance with A button
		if (isShowingDialogue && lastPlayedSceneName == "Level0" &&
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible)
		{
			// Skip all dialogue with Start button
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START))
			{
				EndDialogue();
				return;
			}
			// Advance dialogue with A button
			else if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) &&
				currentDialogueIndex < 14)
			{
				currentDialogueIndex++;

				if (currentDialogueIndex == 14)
				{
					EndDialogue();
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

	// Handle fade-in effect at the start
	if (isFading && FadeOutSprite && !GameManager::GameCleared)
	{
		FadeOutSprite->isVisible = true;
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
		CutSceneSprite->isVisible = true;
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();

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
				CutSceneSFX->Stop();
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

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();

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
				std::string cutscenePath = "Resources/Sprites/cutscene/Level2/" + std::to_string(currentCutsceneIndex) + ".jpg";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
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

		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = true;
		cutsceneTimer += DuckEngine::DeltaTime();

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
				std::string cutscenePath = "Resources/Sprites/cutscene/Level3/" + std::to_string(currentCutsceneIndex) + ".jpg";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}

			

			if (currentCutsceneIndex == 4) {
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0; // Start fade
				cutsceneTimer = 0.0f;
			}
		}
	}
	else if (isCutSceneFading) // Handle fade before dialogue starts
	{
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
		DialoguefadeProgress += DuckEngine::DeltaTime() * (skipped ? 4.0f : 1.0f);

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
				GameManager::GameCleared = false;
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
		// SKIP is drawn above the pause menu, so it steps aside while paused.
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = !DuckEngine::isGamePaused;
		// Check for user input to progress dialogue
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) && currentDialogueIndex < 14 &&
			!settingsHasInput)
		{
			currentDialogueIndex++;

			if (currentDialogueIndex == 14)
			{
				EndDialogue();
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

void CutSceneLogic::SkipPictures()
{
	// Once the fade has begun there is nothing left to skip, and SKIP has
	// gone with it.
	if (!isPlaying || isCutSceneFading || isShowingDialogue) return;

	int lastPicture = -1;
	if (lastPlayedSceneName == "Level0") lastPicture = 13;
	else if (lastPlayedSceneName == "Level1_5") lastPicture = 3;
	else if (lastPlayedSceneName == "Level2_5") lastPicture = 4;
	if (lastPicture < 0) return;

	currentCutsceneIndex = lastPicture;
	cutsceneTimer = 0.0f;
	isCutSceneFading = true;
	skipped = true;
	// The closing fade holds for its first two seconds and darkens over the
	// next four. A skip starts it where the darkening starts.
	DialoguefadeProgress = 2.0f;
	if (FadeOutSprite) FadeOutSprite->color.a = 0;
	if (CutSceneButton)
	{
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneButton->entityID)->isVisible = false;
	}
}

void CutSceneLogic::EndDialogue()
{
	if (FadeOutSprite)
	{
		FadeOutSprite->isVisible = false;
	}
	DialogueSprite->isVisible = false;
	isShowingDialogue = false;
	isPlaying = false;
	DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(DialogueButton->entityID)->isVisible = false;
	DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CutSceneEntity->entityID)->isVisible = false;
	CutSceneBGM->Stop();
}

bool CutSceneLogic::CutscenePlay()
{
	return isPlaying;
}