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

/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
/// 
/// 

Entity* BackButton;

//bool shouldClose = false;

void HowToPlay::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	BackButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Back");
	auto back = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(BackButton->entityID);
	back->onClick = []() { std::cout << "Button clicked go back!!!!!!\n"; GameManager::SetActiveScene("MainMenu"); };
}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void HowToPlay::Start()
{
	Scene::Start();
	//DuckEngine::showDebugColliders = false;
}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
void HowToPlay::Update()
{
	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);
}

/// <summary>
/// Performs any operations after the main update logic, such as cleanup or post-processing.
/// This function is called every frame, after the Update() method.
/// </summary>
void HowToPlay::PostUpdate()
{

}

/// <summary>
/// Exits the scene, performing any cleanup necessary before the scene is unloaded.
/// </summary>
void HowToPlay::Exit()
{

}

/// <summary>
/// Unloads the scene and frees any resources that were loaded during the Load() phase.
/// </summary>
void HowToPlay::Unload()
{
	// base unload
	Scene::Unload();

}