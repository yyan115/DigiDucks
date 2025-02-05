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
#include "DuckEngine_Input.h"
#include "AssetManager.h"

void CutSceneLogic::Start()
{
	// Fetch necessary game objects
	auto CutSceneEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneManager").get();
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
	if (FadeEntity)
	{
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

	// Initialize state variables
	currentCutsceneIndex = 0;
	currentDialogueIndex = 0;
	cutsceneTimer = 0.0f;
	dialogueTimer = 0.0f;
	fadeProgress = 0.0f;
	DialoguefadeProgress = 0.0f;

	isFading = true; // Start with a fade-in effect
	isCutSceneFading = false;
	isPlaying = true;
	isShowingDialogue = false;
}

void CutSceneLogic::Update()
{
	if (!isPlaying && !isShowingDialogue) return;

	// **Handle fade-in effect at the start**
	if (isFading)
	{
		if (!CutSceneBGM->IsSoundPlaying()) CutSceneBGM->Play(); // Play the cutscene BGM once

		fadeProgress += DuckEngine::DeltaTime() / 0.5f; // 0.5s fade duration
		FadeOutSprite->color.a = static_cast<int>(255 * (1.0f - fadeProgress));

		if (fadeProgress >= 1.0f)
		{
			FadeOutSprite->color.a = 0; // Fully transparent
			FadeOutSprite->isVisible = false;
			isFading = false;
		}
		else
		{
			return; // Prevent progressing while fading in
		}
	}

	// **Handle cutscene sequence**
	if (isPlaying && currentCutsceneIndex < 14)
	{
		cutsceneTimer += DuckEngine::DeltaTime();

		if (cutsceneTimer >= 1.5f) // Change cutscene every 1.5s
		{
			currentCutsceneIndex++;
			cutsceneTimer = 0.0f; // Reset timer

			// Update cutscene sprite only if it exists
			if (CutSceneSprite)
			{
				std::string cutscenePath = "Resources/Sprites/cutscene/" + std::to_string(currentCutsceneIndex) + ".png";
				CutSceneSprite->texture = *AssetManager::GetTexture(cutscenePath).get();
			}

			// Play sound effect for this scene
			if (CutSceneSFX) CutSceneSFX->Play(currentCutsceneIndex);

			// Prepare to transition to dialogues
			if (currentCutsceneIndex == 14)
			{
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0;
				FadeOutSprite->isVisible = true;
			}
		}
	}
	// **Handle fade-out before dialogue starts**
	else if (isCutSceneFading)
	{
		DialoguefadeProgress += DuckEngine::DeltaTime();

		// Fade-out completed
		if (DialoguefadeProgress >= 6.0f)
		{
			FadeOutSprite->color.a = 255; // Fully visible
			FadeOutSprite->isVisible = false;
			isCutSceneFading = false;
			isShowingDialogue = true;
			CutSceneSprite->isVisible = false; // Hide cutscene
			dialogueTimer = 0.0f;
			DialogueSprite->isVisible = true; // Show dialogue box
		}
		else if (DialoguefadeProgress >= 2.0f) // Start fading out at 2s
		{
			float fadeRatio = (DialoguefadeProgress - 2.0f) / 4.0f; // Normalize fade (2s - 6s)
			FadeOutSprite->color.a = static_cast<int>(fadeRatio * 255.0f);
		}
	}

	// **Handle dialogues after cutscene**
	if (isShowingDialogue)
	{

		// Handle dialogue progression on click
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) || DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_SPACE))
		{
			if (++currentDialogueIndex >= 31) // End cutscene at dialogue 31
			{
				FadeOutSprite->isVisible = false;
				DialogueSprite->isVisible = false;
				isShowingDialogue = false;
				isPlaying = false;
				CutSceneBGM->Stop();
				return;
			}

			// Update dialogue texture
			std::string dialoguePath = "Resources/Sprites/Dialogues/intro/" + std::to_string(currentDialogueIndex) + ".png";
			DialogueSprite->texture = *AssetManager::GetTexture(dialoguePath).get();

			// Play dialogue sound effect (randomly select between 15-16)
			if (CutSceneSFX) CutSceneSFX->Play(15 + (std::rand() % 2));
		}
	}
}

bool CutSceneLogic::CutscenePlay()
{
	return isPlaying;
}
