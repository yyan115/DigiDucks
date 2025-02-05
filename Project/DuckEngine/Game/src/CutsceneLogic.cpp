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
#include "Level0.h"

void CutSceneLogic::Start()
{
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

	// Handle fade-in effect at the start
	if (isFading)
	{
		CutSceneBGM->Play(); // Play the cutscene BGM
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

	// If playing cutscene
	if (isPlaying && currentCutsceneIndex < 14)
	{
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
				CutSceneSFX->Play(currentCutsceneIndex);
			}
			
			if (currentCutsceneIndex == 14) {
				isCutSceneFading = true;
				FadeOutSprite->color.a = 0; // Start fade-out
			}
		}
	}	
	else if (isCutSceneFading) // Handle fade-out before dialogue starts
	{
		DialoguefadeProgress += DuckEngine::DeltaTime();

		if (DialoguefadeProgress >= 6.0f)
		{
			FadeOutSprite->color.a = 255; // Fully visible fade-out
			FadeOutSprite->isVisible = false;
			isCutSceneFading = false;
			isShowingDialogue = true;
			CutSceneSprite->isVisible = false; // Hide cutscene sprite
			dialogueTimer = 0.0f;
			DialogueSprite->isVisible = true; // Show dialogue box
		}
		else if (DialoguefadeProgress >= 2.0f)
		{
			FadeOutSprite->isVisible = true;

			float fadeRatio = (DialoguefadeProgress - 2.0f) / (6.0f - 2.0f);
			FadeOutSprite->color.a = static_cast<int>(fadeRatio * 255.0f);

		}
	}


	// **Handle dialogues after cutscene**
	if (isShowingDialogue)
	{
		// Wait 2 seconds before allowing click to progress
		dialogueTimer += DuckEngine::DeltaTime();
		if (dialogueTimer < 2.0f) return;

		// Check for user input to progress dialogue
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) && currentDialogueIndex < 31)
		{
			currentDialogueIndex++;
			std::cout << "Dialogue index: " << currentDialogueIndex << std::endl;
			
			if (currentDialogueIndex == 31)
			{
				// End cutscene entirely and start game
				Level0* level0Scene = DuckEngine::DUCKENGINE_SceneManager.GetScene<Level0>("Level0").get();
				level0Scene->FinishedTutorial();
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

			// Play sound effect for dialogues between soundtrack 15 and 16
			if (CutSceneSFX)
			{
				CutSceneSFX->Play(15 + (std::rand() % 2));
			}

			
		}
	}
}

bool CutSceneLogic::CutscenePlay()
{
	return isPlaying;
}
