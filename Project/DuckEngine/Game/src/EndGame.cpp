/******************************************************************************/
/*!
\file       EndGame.cpp
\author     Tan Yan Kai, yankai.tan, 2301312
\par        yankai.tan@digipen.edu
\date       November 30 2024
\brief      Implementation of the EndScene class, it loads up the necessary assets needed for the UI and display of the game score before going back to main menu.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "EndGame.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>
#include "Scene.h"
#include "GameManager.h"
#include "ScoreLogic.h"
#include "LevelSelectScreenLogic.h"
#include "SoundSystem.h"
SoundComponent* MainMenuSound = nullptr;
bool isQuitButtonClicked = false;

SpriteRendererComponent* backgroundSR;

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	CameraManager::SetHeight(20);

	MainMenuButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit").get();
	if (MainMenuButton != nullptr) {
		MainMenu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(MainMenuButton->entityID);
		MainMenuSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(MainMenuButton->entityID);
	}

	// Find fade-in screen entity
	FadeInScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeInMenu").get();
	if (FadeInScreen)
	{
		FadeInSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeInScreen->entityID);
		if (FadeInSpriteRenderer)
		{
			FadeInSpriteRenderer->color.a = 0; // Start fully transparent
			FadeInSpriteRenderer->isVisible = true;
			fadeInElapsedTime = 0.0f;
			isFadingIn = false;
		}
	}

	MainMenu->onClick = [this]()
		{
			std::cout << "Button clicked QUIT!!!!!!\n";
			MainMenuSound->Play(1);
			isQuitButtonClicked = true;
			isFadingIn = true; // Start fade-in
			fadeInElapsedTime = 0.0f;
		};

	MainMenu->onHover = []()
		{
			std::cout << "hover" << std::endl;
			MainMenuSound->Play();
		};

	Score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score").get();
	ScoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(Score->entityID);

	if (ScoreText) {
		ScoreText->text = "Score: " + std::to_string(ScoreLogic::scoreValue);
		ScoreText->isEnabled = true;
	}

	Entity* background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("EndBGM").get();
	backgroundSR = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(background->entityID);
	backgroundSR->texture = AssetManager::GetTextureByName("DAY" + ScoreLogic::dayNumber);

	Star_1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_1").get();
	Star_2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_2").get();
	Star_3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_3").get();

	Star1 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_1->entityID);
	Star2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_2->entityID);
	Star3 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_3->entityID);

	Background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("EndBGM").get();
	BGMSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(Background->entityID);

	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
		if (GameManager::GetGlobalVariable("ShowFPS").empty()) { FPSText->isEnabled = false; }
		else if (GameManager::GetGlobalVariable("ShowFPS") == "true") { FPSText->isEnabled = true; }
		else { FPSText->isEnabled = false; }
	}

	RestartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restart").get();
	Restart = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(RestartButton->entityID);
	
	Restart_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(RestartButton->entityID);

	NextButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Next").get();
	Next = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(NextButton->entityID);
	

}

void EndScene::Start()
{
	Scene::Start();

	std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
	std::cout << "sceneName " << lastPlayedSceneName << std::endl;
	if (!lastPlayedSceneName.empty() && lastPlayedSceneName.find("Level") != std::string::npos)
	{
		int day = std::stoi(lastPlayedSceneName.substr(5, 1));
		std::string EndScenePath = "Resources/Sprites/EndScene/DAY" + std::to_string(day) + ".png";
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Background->entityID)->texture = *AssetManager::GetTexture(EndScenePath).get();
	}

	std::string dayTextureName = "DAY" + std::to_string(ScoreLogic::dayNumber);
	backgroundSR->texture = AssetManager::GetTextureByName(dayTextureName);

	LevelSelectScreenLogic::LevelCompleted();




	Restart->onClick = [this, lastPlayedSceneName]() {
		std::cout << "Restart button clicked!" << std::endl;
		GameManager::SetActiveScene(lastPlayedSceneName);
		std::cout << "current scene:" << lastPlayedSceneName << std::endl;
	};

	Next->onClick = [this, lastPlayedSceneName]() {
		
		if (lastPlayedSceneName == "Level0")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level1");
			GameManager::SetActiveScene("Level1");
		}
		else if (lastPlayedSceneName == "Level1")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level1_5");
			GameManager::SetActiveScene("Level1_5");
		}
		else if (lastPlayedSceneName == "Level1_5")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level2");
			GameManager::SetActiveScene("Level2");
		}
		else if (lastPlayedSceneName == "Level2")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level2_5");
			GameManager::SetActiveScene("Level2_5");
		}
		else if (lastPlayedSceneName == "Level2_5")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level3");
			GameManager::SetActiveScene("Level3");
		}
		else if (lastPlayedSceneName == "Level3")
		{
			GameManager::SetGlobalVariable("LastPlayedScene", "Level3_5");
			GameManager::SetActiveScene("Level3_5");
		}
	};
	
	Next_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(NextButton->entityID);


}

void EndScene::Update()
{
	if (isFadingIn && FadeInSpriteRenderer && BGMSound)
	{
		if ((fadeInElapsedTime += DuckEngine::DeltaTime()) >= fadeInDuration)
		{
			// Ensure full fade-in effect
			FadeInSpriteRenderer->color.a = 255;
			isFadingIn = false;
			fadeInElapsedTime = 0.0f;

			// Ensure music volume is fully off and stop the sound
			SoundSystem::SetSoundVolume(BGMSound->soundID[0], 0.0f);
			SoundSystem::StopSounds(BGMSound->soundID[0]);

			// Switch to MainMenu scene after fade-in
			GameManager::SetActiveScene("MainMenu");
			return;
		}

		// Calculate fade-in progress (0.0 to 1.0)
		float fadeProgress = fadeInElapsedTime / fadeInDuration;

		// Update transparency
		FadeInSpriteRenderer->color.a = static_cast<unsigned char>(fadeProgress * 255);

		// Gradually decrease the background music volume
		SoundSystem::SetSoundVolume(BGMSound->soundID[0], BGMSound->volume * (1.0f - fadeProgress));
	}
	
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");

	// Tutorial Level
	if (lastPlayedSceneName == "Level0") 
	{ 
		iStar_1 = iStar_2 = iStar_3 = 10; 
	}
	else if (lastPlayedSceneName == "Level1" || lastPlayedSceneName == "Level1_5")
	{
		iStar_1 = 50;
		iStar_2 = 100;
		iStar_3 = 150;
	}
	else if (lastPlayedSceneName == "Level2" || lastPlayedSceneName == "Level2_5")
	{
		iStar_1 = 100;
		iStar_2 = 150;
		iStar_3 = 200;
	}	
	else if (lastPlayedSceneName == "Level3" || lastPlayedSceneName == "Level3_5")
	{
		iStar_1 = 150;
		iStar_2 = 200;
		iStar_3 = 250;
	}


	if (ScoreLogic::scoreValue < iStar_1)
	{
		Restart_Spt->isVisible = true;
		backgroundSR->texture = AssetManager::GetTextureByName("DAYLOSE");
		ScoreText->isEnabled = false;
		Star1->isVisible = false;
		Star2->isVisible = false;
		Star3->isVisible = false;
	}
	else
	{
		Next_Spt->isVisible = true;
		LevelSelectScreenLogic::stageLevel = LevelSelectScreenLogic::currentStage;
	}

	if (ScoreLogic::scoreValue >= iStar_1)
	{
		Star1->texture = AssetManager::GetTextureByName("star");
	}

	if (ScoreLogic::scoreValue >= iStar_2)
	{
		Star2->texture = AssetManager::GetTextureByName("star");
	}
	
	if (ScoreLogic::scoreValue >= iStar_3)
	{
		Star3->texture = AssetManager::GetTextureByName("star");
	}
	
	if (FPSText != nullptr) {
		FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	}
}

void EndScene::PostUpdate()
{
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (FPSText) {
			FPSText->isEnabled = !FPSText->isEnabled;
			GameManager::SetGlobalVariable("ShowFPS", FPSText->isEnabled ? "true" : "false");
		}
	}
}

void EndScene::Exit()
{
	ScoreText->isEnabled = false;
	fadeInElapsedTime = 0.0f;
	isFadingIn = false;
}

void EndScene::Unload()
{
	// base unload
	Scene::Unload();
}