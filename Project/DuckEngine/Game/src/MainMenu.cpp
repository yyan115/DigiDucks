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
#include "DuckEngine.h"
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


SoundComponent* menusound;

Entity* StartButton;
Entity* QuitButton;
Entity* HtpButton;
Entity* FadeOutScreen;

SpriteRendererComponent* startButtonSpriteRenderer;
SpriteRendererComponent* quitButtonSpriteRenderer;
SpriteRendererComponent* htpButtonSpriteRenderer;
SpriteRendererComponent* FadeOutSpriteRenderer;

Texture startNormalTexture;
Texture startHoverTexture;

Texture quitNormalTexture;
Texture quitHoverTexture;

Texture htpNormalTexture;
Texture htpHoverTexture;

SoundComponent* StartSound;
SoundComponent* QuitSound;
SoundComponent* HtpSound;

float fadeOutDuration = 3.0f;
float fadeElapsedTime = 0.0f;
bool isFadingOut = false;

bool shouldClose = false;

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

void MainMenu ::Load()
{
	Scene::Load();
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	StartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Start").get();
	auto start = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(StartButton->entityID);
	StartSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(StartButton->entityID);
	menusound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MenuBGM").get()->entityID);
	FadeOutScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeOutMenu").get();
	start->onClick = [this]() {
		StartSound->Play(1);
		if (menusound) {
			fadeOutDuration = 3.0f; // 3 seconds fade duration
			fadeElapsedTime = 0.0f;
			isFadingOut = true;
		}
		
	};
	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit").get();
	auto exit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	QuitSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(QuitButton->entityID);
	exit->onClick = []() { QuitSound->Play(1); GameManager::DuckEngine.CloseWindow(); };
	HtpButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlay").get();
	auto htp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(HtpButton->entityID);
	HtpSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(HtpButton->entityID);
	htp->onClick = []() { HtpSound->Play(1); GameManager::SetActiveScene("HowToPlay"); };

	startButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(StartButton->entityID);
	quitButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(QuitButton->entityID);
	htpButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HtpButton->entityID);
	FadeOutSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeOutScreen->entityID);

	startNormalTexture = AssetManager::GetTextureByName("start");
	startHoverTexture = AssetManager::GetTextureByName("start_click");

	quitNormalTexture = AssetManager::GetTextureByName("quit");
	quitHoverTexture = AssetManager::GetTextureByName("quit_click");

	htpNormalTexture = AssetManager::GetTextureByName("howtoplay");
	htpHoverTexture = AssetManager::GetTextureByName("howtoplay_click");

	start->onHover = []()
		{
			StartSound->Play();
			startButtonSpriteRenderer->texture = startHoverTexture;
		};

	start->onFinishHover = []()
		{
			startButtonSpriteRenderer->texture = startNormalTexture;
		};

	exit->onHover = []()
		{
			QuitSound->Play();
			quitButtonSpriteRenderer->texture = quitHoverTexture;
		};

	exit->onFinishHover = []()
		{
			quitButtonSpriteRenderer->texture = quitNormalTexture;
		};

	htp->onHover = []()
		{
			HtpSound->Play();
			htpButtonSpriteRenderer->texture = htpHoverTexture;
		};

	htp->onFinishHover = []()
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

			GameManager::SetActiveScene("GameScene");
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