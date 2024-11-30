/******************************************************************************/
/*!
\file       HowToPlay.cpp
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Implementation of the HowToPlay class, it loads up the screen to show the instructions on how to play the game whenever it is triggered.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "HowToPlay.h"
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

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

Entity* ExitButton;
Entity* NextButton;
Entity* BackButton;
int pageNum;

Entity* JournalPage;
SpriteRendererComponent* JournalSprite;

void HowToPlay::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	ExitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit");
	if(ExitButton)
	{
		auto back = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(ExitButton->entityID);
		back->onClick = []() { std::cout << "Button clicked go back!!!!!!\n"; GameManager::SetActiveScene("MainMenu"); };
	}

	NextButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Next");
	if(NextButton)
	{
		auto next = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(NextButton->entityID);
		next->onClick = []() { if (pageNum < 3) { pageNum++; } };
	}

	BackButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Back");
	if (BackButton) {
		auto backBtn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(BackButton->entityID);
		backBtn->onClick = []() { if (pageNum > 1) { pageNum--; } };
	}

	JournalPage = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Journal");
	if(JournalPage)
	{
		JournalSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(JournalPage->entityID);
	}

	pageNum = 1;
}

/****************************************************************
* @brief Starts the scene, initializing game objects and setting up the initial state.
* This function is called when the scene begins running.
* ****************************************************************/
void HowToPlay::Start()
{
	Scene::Start();
}

/****************************************************************
* @brief Updates the scene logic each frame, including processing input and updating game objects.
* This function is called every frame.
* ****************************************************************/
void HowToPlay::Update()
{
	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	switch (pageNum)
	{
	case 1:
		if (JournalSprite)
			JournalSprite->texture = AssetManager::GetTextureByName("journal_1");
		break;
	case 2:
		if (JournalSprite)
			JournalSprite->texture = AssetManager::GetTextureByName("journal_2");
		break;
	case 3:
		if (JournalSprite)
			JournalSprite->texture = AssetManager::GetTextureByName("journal_3");
		break;
	default:
		break;
	};
}

/****************************************************************
* @brief Performs any operations after the main update logic, such as cleanup or post-processing. 
* This function is called every frame, after the Update() method.
* ****************************************************************/
void HowToPlay::PostUpdate()
{

}

/****************************************************************
* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
* ****************************************************************/
void HowToPlay::Exit()
{

}

/****************************************************************
* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
* ****************************************************************/
void HowToPlay::Unload()
{
	// base unload
	Scene::Unload();

}