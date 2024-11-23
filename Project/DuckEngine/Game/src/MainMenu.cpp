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


//Entity* duck;
//TransformComponent* duckTrfm;
//RigidbodyComponent* duckRb;
//AnimatorComponent* duckAnimator;
SoundComponent* duckSound1;
//BoundingCircle* duckCollider;

////Test Messaging System
//InputEventManager inputEventManager;
//Player message;

/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
/// 

void MainMenu ::Load()
{
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(20);
}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void MainMenu ::Start()
{
	
		DuckEngine::showDebugDraw = false;
}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
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
	//if (DuckEngine::DUCKENGINE_AssetManager.LoadTexture("Resources/Sprites/MainMenu/mainmenu.png")) {
	//	std::cerr << "Failed to load background texture!" << std::endl;
	//}
}

/// <summary>
/// Performs any operations after the main update logic, such as cleanup or post-processing.
/// This function is called every frame, after the Update() method.
/// </summary>
void MainMenu::PostUpdate()
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (duckSound1) {
			std::cout << "Sound stopped\n";
			duckSound1->Stop();
		}
	}

	//if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	//{
	//	std::cout << "K is pressed!\n";
	//	DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
	//}
}

/// <summary>
/// Exits the scene, performing any cleanup necessary before the scene is unloaded.
/// </summary>
void MainMenu::Exit()
{

}

/// <summary>
/// Unloads the scene and frees any resources that were loaded during the Load() phase.
/// </summary>
void MainMenu::Unload()
{
	// base unload
	Scene::Unload();

}