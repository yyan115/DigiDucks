/******************************************************************************/
/*!
\file       GameScene.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Implementation of the GameScene class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameScene.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "Scene.h"


Entity* duck;
TransformComponent* duckTrans;
SoundComponent* duckSound;


Entity* timer;
TextComponent* timerText;
float timeLeft{};


/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
void GameScene::Load() 
{
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(20);

	// instantiate prefabs
	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	duckTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
	duckSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(duck->entityID);

	timer = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Timer");
	timerText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(timer->entityID);
	timeLeft = 600.f;
}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void GameScene::Start() 
{
	Scene::Start();
	DuckEngine::showDebugDraw = false;
}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
void GameScene::Update() 
{	

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(duckTrans->GetPosition().x, duckTrans->GetPosition().y);

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	// Update the timer
	if (timeLeft > 0.f) {
		timeLeft -= DuckEngine::DeltaTime();
		int minutes = static_cast<int>(timeLeft) / 60;
		int seconds = static_cast<int>(timeLeft) % 60;
		timerText->text = "Time: " + std::to_string(minutes) + ":" + std::to_string(seconds);
	}
	else {
		timerText->text = "Time's up!";
		// Change to End Scene.
		GameManager::SetActiveScene("EndScene");
	}


}

/// <summary>
/// Performs any operations after the main update logic, such as cleanup or post-processing.
/// This function is called every frame, after the Update() method.
/// </summary>
void GameScene::PostUpdate() 
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (duckSound) {
			std::cout << "Sound stopped\n";
			duckSound->Stop();
		}
	}


	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
	}
}

/// <summary>
/// Exits the scene, performing any cleanup necessary before the scene is unloaded.
/// </summary>
void GameScene::Exit() 
{

}

/// <summary>
/// Unloads the scene and frees any resources that were loaded during the Load() phase.
/// </summary>
void GameScene::Unload() 
{
	// base unload
	Scene::Unload();

}