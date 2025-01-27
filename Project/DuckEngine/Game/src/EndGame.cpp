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
#include "GameScene.h"



Entity* MainMenuButton;
SoundComponent* MainMenuSound;
ButtonComponent* MainMenu;

Entity* Star_1;
SpriteRendererComponent* Star1;

Entity* Star_2;
SpriteRendererComponent* Star2;

Entity* Star_3;
SpriteRendererComponent* Star3;

Entity* Score;
TextComponent* ScoreText;
extern int scoreValue;

Entity* Background;
SoundComponent* BGMSound;

bool isQuitButtonClicked = false;
bool isHover = false;

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	MainMenuButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit").get();
	MainMenu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(MainMenuButton->entityID);
	MainMenuSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(MainMenuButton->entityID);
	MainMenu->onClick = []()
		{ 
			std::cout << "Button clicked QUIT!!!!!!\n"; 
			MainMenuSound->Play(1);
			isQuitButtonClicked = true;
		};

	MainMenu->onHover = []()
		{
			std::cout << "hover" << std::endl;
			MainMenuSound->Play();
		};

	Score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score").get();
	ScoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(Score->entityID);
	
	if (ScoreText) {
		ScoreText->text = "Score: " + std::to_string(scoreValue);
	}

	Star_1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_1").get();
	Star_2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_2").get();
	Star_3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_3").get();

	Star1 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_1->entityID);
	Star2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_2->entityID);
	Star3 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_3->entityID);

	Background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BackgroundGameObject").get();
	BGMSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(Background->entityID);

}

void EndScene::Start()
{
	Scene::Start();
	//DuckEngine::showDebugColliders = false;
}

void EndScene::Update()
{
	if (isQuitButtonClicked && !MainMenuSound->IsSoundPlaying())
	{
		GameManager::SetActiveScene("MainMenu");
		isQuitButtonClicked = false;
	}
	 



	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	if (scoreValue > 10) {

		Star1->texture = AssetManager::GetTextureByName("star");
	}

	if (scoreValue > 30) {

		//Star1->texture = AssetManager::GetTextureByName("star");
		Star2->texture = AssetManager::GetTextureByName("star");

	}
	
	if (scoreValue > 50) {
		//Star1->texture = AssetManager::GetTextureByName("star");
		//Star2->texture = AssetManager::GetTextureByName("star");
		Star3->texture = AssetManager::GetTextureByName("star");
	}
	
}

void EndScene::PostUpdate()
{

}

void EndScene::Exit()
{

}

void EndScene::Unload()
{
	// base unload
	Scene::Unload();
}