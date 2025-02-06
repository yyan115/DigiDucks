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
#include "ScoreLogic.h"

SoundComponent* MainMenuSound = nullptr;
bool isQuitButtonClicked = false;

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	MainMenuButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit").get();
	if (MainMenuButton != nullptr) {
		MainMenu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(MainMenuButton->entityID);
		MainMenuSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(MainMenuButton->entityID);
	}
	
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
		ScoreText->text = "Score: " + std::to_string(ScoreLogic::scoreValue);
		ScoreText->isEnabled = true;
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

	std::string sceneName = GameManager::GetGlobalVariable("LastPlayedScene");
	std::cout << "sceneName " << sceneName << std::endl;
	if (!sceneName.empty() && sceneName.find("Level") != std::string::npos)
	{
		int day = std::stoi(sceneName.substr(5, 1));
		std::string EndScenePath = "Resources/Sprites/EndScene/DAY" + std::to_string(day) + ".png";
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Background->entityID)->texture = *AssetManager::GetTexture(EndScenePath).get();

	}
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

	if (ScoreLogic::scoreValue > 10) {

		Star1->texture = AssetManager::GetTextureByName("star");
	}

	if (ScoreLogic::scoreValue > 30) {

		//Star1->texture = AssetManager::GetTextureByName("star");
		Star2->texture = AssetManager::GetTextureByName("star");

	}
	
	if (ScoreLogic::scoreValue >= 40) {
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
	ScoreText->isEnabled = false;
}

void EndScene::Unload()
{
	// base unload
	Scene::Unload();
}