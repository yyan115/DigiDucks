/******************************************************************************/
/*!
\file       MainMenu.cpp
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Implementation of the MainMenu class, it loads up the necessary assets needed for the UI before going into the GameScene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "MainMenu.h"
#include "ButtonSystem.h"
#include "ButtonComponent.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>
#include "Scene.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include "HowToPlayLogic.h"
#include "SaveLoadManager.h"

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

void MainMenu::Load()
{
	Scene::Load();
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);
	SaveLoadManager::InitializeSaveFile();

	// Apply the saved audio settings
	SoundSystem::SetMasterVolume(SaveLoadManager::masterVolume);
	SoundSystem::SetCategoryVolume("BGM", SaveLoadManager::musicVolume);
	SoundSystem::SetCategoryVolume("SFX", SaveLoadManager::sfxVolume);

	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectScreen").get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();
	HTPScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlayScreen").get();

	StartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Start").get();
	auto start = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(StartButton->entityID);
	StartSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(StartButton->entityID);
	
	//level select button
	LevelSelectButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectButton").get();
	auto levelSelect = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(LevelSelectButton->entityID);
	levelSelect->onClick = [this]()
		{
			if (!isFadingOut)
			{
				StartSound->Play(1);
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = true;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
			}
		};

	menusound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MenuBGM").get()->entityID);
	if (menusound)
	{
		SoundSystem::SetSoundVolume(menusound->soundID[0], 0.0f); // Start at 0 volume
	}
	FadeOutScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeOutMenu").get();
	// Ensure the FadeOutSprite is reset
	FadeOutSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeOutScreen->entityID);
	FadeOutSpriteRenderer->color.a = 0;
	FadeOutSpriteRenderer->isVisible = true; 
	fadeOutDuration = 3.0f;
	fadeElapsedTime = 0.0f;
	isFadingOut = false;
	fadeInDuration = 3.0f;
	fadeInElapsedTime = 0.0f;
	isFadingIn = true;

	start->onClick = [this]()
		{
			StartSound->Play(1);

			std::string sceneToLoad;

			if (LevelSelectScreenLogic::currentStage >= 5)
			{
				sceneToLoad = "Level35";
				std::cout << "Starting Level 3.5" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 4)
			{
				sceneToLoad = "Level3";
				std::cout << "Starting Level 3" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 3)
			{
				sceneToLoad = "Level25";
				std::cout << "Starting Level 2.5" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 2)
			{
				sceneToLoad = "Level2";
				std::cout << "Starting Level 2" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 1)
			{
				sceneToLoad = "Level15";
				std::cout << "Starting Level 1.5" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 0)
			{
				sceneToLoad = "GameScene";
				std::cout << "Starting Level 1" << std::endl;
			}
			else
			{
				sceneToLoad = "Level0"; // Default to tutorial if no progress
				std::cout << "Starting Tutorial Level 0" << std::endl;
			}

			OnPlayButtonClicked(sceneToLoad);
		};
	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit").get();
	auto exit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	QuitSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(QuitButton->entityID);
	exit->onClick = [this]() 
		{ 
			if (!isFadingOut)
			{
				QuitSound->Play(1); GameManager::DuckEngine.CloseWindow();
			}
		};
	HtpButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlay").get();
	auto htp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(HtpButton->entityID);
	HtpSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(HtpButton->entityID);
	htp->onClick = [this]() 
		{
			if (!isFadingOut)
			{
				HtpSound->Play(1); 
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible = true;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
			}
		};

	startButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(StartButton->entityID);
	levelSelectButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(LevelSelectButton->entityID);
	quitButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(QuitButton->entityID);
	htpButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HtpButton->entityID);
	

	startNormalTexture = AssetManager::GetTextureByName("start");
	startHoverTexture = AssetManager::GetTextureByName("start_click");

	levelSelectNormalTexture = AssetManager::GetTextureByName("levelselect");
	levelSelectHoverTexture = AssetManager::GetTextureByName("levelselect_hover");

	quitNormalTexture = AssetManager::GetTextureByName("quit");
	quitHoverTexture = AssetManager::GetTextureByName("quit_click");

	htpNormalTexture = AssetManager::GetTextureByName("mainmenu_howtoplay");
	htpHoverTexture = AssetManager::GetTextureByName("mainmenu_howtoplay_hover");

	start->onHover = [this]()
		{
			StartSound->Play();
			startButtonSpriteRenderer->texture = startHoverTexture;
		};

	start->onFinishHover = [this]()
		{
			startButtonSpriteRenderer->texture = startNormalTexture;
		};

	levelSelect->onHover = [this]()
		{
			StartSound->Play();
			levelSelectButtonSpriteRenderer->texture = levelSelectHoverTexture;
		};
	
	levelSelect->onFinishHover = [this]()
		{
			levelSelectButtonSpriteRenderer->texture = levelSelectNormalTexture;
		};

	exit->onHover = [this]()
		{
			QuitSound->Play();
			quitButtonSpriteRenderer->texture = quitHoverTexture;
		};

	exit->onFinishHover = [this]()
		{
			quitButtonSpriteRenderer->texture = quitNormalTexture;
		};

	htp->onHover = [this]()
		{
			HtpSound->Play();
			htpButtonSpriteRenderer->texture = htpHoverTexture;
		};

	htp->onFinishHover = [this]()
		{
			htpButtonSpriteRenderer->texture = htpNormalTexture;
		};

	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
		if (GameManager::GetGlobalVariable("ShowFPS").empty()) { FPSText->isEnabled = false; } 
		else if (GameManager::GetGlobalVariable("ShowFPS") == "true") { FPSText->isEnabled = true; }
		else { FPSText->isEnabled = false; }
	}
}


/****************************************************************
* @brief Starts the scene, initializing game objects and setting up the initial state.
* This function is called when the scene begins running.
* ****************************************************************/
void MainMenu ::Start()
{
	Scene::Start();
}

/****************************************************************
* @brief Updates the scene logic each frame, including processing input and updating game objects.
* This function is called every frame.
* ****************************************************************/
void MainMenu ::Update()
{
	if (isFadingIn && FadeOutSpriteRenderer && menusound)
	{
		if ((fadeInElapsedTime += DuckEngine::DeltaTime()) >= fadeInDuration)
		{
			// Fadein
			FadeOutSpriteRenderer->color.a = 0;
			isFadingIn = false;
			fadeInElapsedTime = 0.0f;

			// Set the final volume for the background music
			SoundSystem::SetSoundVolume(menusound->soundID[0], menusound->volume);
		}
		else
		{
			// Gradually decrease alpha from 255 to 0
			float fadeProgress = fadeInElapsedTime / fadeInDuration;
			FadeOutSpriteRenderer->color.a = static_cast<unsigned char>((1.0f - fadeProgress) * 255);
		}
	}


	if (isFadingOut && FadeOutSpriteRenderer && menusound) {
		if ((fadeElapsedTime += DuckEngine::DeltaTime()) >= fadeOutDuration) {
			SoundSystem::SetSoundVolume(menusound->soundID[0], 0.0f);
			SoundSystem::StopSounds(menusound->soundID[0]);
			FadeOutSpriteRenderer->color.a = 0;
			isFadingOut = false;
			fadeElapsedTime = 0.0f;

			GameManager::SetActiveScene(nextScene);
			return;
		}

		// Calculate fade progress
		float fadeProgress = fadeElapsedTime / fadeOutDuration;
		SoundSystem::SetSoundVolume(menusound->soundID[0], menusound->volume * (1.0f - fadeProgress));
		FadeOutSpriteRenderer->color.a = static_cast<unsigned char>(fadeProgress * 255);
	}

	if (FPSText != nullptr) {
		FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	}

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

}

/****************************************************************
* @brief Performs any operations after the main update logic, such as cleanup or post-processing.
* This function is called every frame, after the Update() method.
* ****************************************************************/
void MainMenu::PostUpdate()
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (FPSText) {
			FPSText->isEnabled = !FPSText->isEnabled;
			GameManager::SetGlobalVariable("ShowFPS", FPSText->isEnabled ? "true" : "false");
		}
	}
}

/****************************************************************
* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
* ****************************************************************/
void MainMenu::Exit()
{

}

/****************************************************************
* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
* ****************************************************************/
void MainMenu::Unload()
{
	// base unload
	Scene::Unload();
}

void MainMenu::OnPlayButtonClicked(std::string sceneNaming)
{
	if (menusound && !isFadingOut) 
	{
		StartSound->Play(1);
		fadeOutDuration = 3.0f; // 3 seconds fade duration
		fadeElapsedTime = 0.0f;
		isFadingOut = true;
		nextScene = sceneNaming;
		std::cout << "LOADING SCENE NAME: " << sceneNaming << std::endl;
		GameManager::SetGlobalVariable("LastPlayedScene", nextScene);
	}
}
