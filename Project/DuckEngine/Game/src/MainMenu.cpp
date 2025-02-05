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
#include "thread"

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

void MainMenu::Load()
{
	Scene::Load();
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectScreen").get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

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
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = true;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
			}
		};

	menusound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MenuBGM").get()->entityID);
	FadeOutScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeOutMenu").get();
	// Ensure the FadeOutSprite is reset
	FadeOutSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeOutScreen->entityID);
	FadeOutSpriteRenderer->color.a = 0;
	FadeOutSpriteRenderer->isVisible = true; 
	fadeOutDuration = 3.0f;
	fadeElapsedTime = 0.0f;
	isFadingOut = false;
	start->onClick = [this]() {
		OnPlayButtonClicked("GameScene");
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
				GameManager::SetActiveScene("HowToPlay"); 
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

	htpNormalTexture = AssetManager::GetTextureByName("howtoplay");
	htpHoverTexture = AssetManager::GetTextureByName("howtoplay_click");

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
	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);
	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	if (isFadingOut && menusound) {
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
		if (menusound) {
			std::cout << "Sound stopped\n";
			menusound->Stop();
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

void MainMenu::OnPlayButtonClicked(std::string sceneName)
{
	if (menusound && !isFadingOut) 
	{
		StartSound->Play(1);
		fadeOutDuration = 3.0f; // 3 seconds fade duration
		fadeElapsedTime = 0.0f;
		isFadingOut = true;
		nextScene = sceneName;
	}
}
