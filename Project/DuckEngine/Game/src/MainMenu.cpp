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
SoundComponent* menusound;
//BoundingCircle* duckCollider;

////Test Messaging System
//InputEventManager inputEventManager;
//Player message;

/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
/// 
/// 

Entity* StartButton;
Entity* ExitButton;
Entity* JournalBookButton;

SpriteRendererComponent* startButtonSpriteRenderer;
SpriteRendererComponent* exitButtonSpriteRenderer;

Texture startNormalTexture;
Texture startHoverTexture;

Texture quitNormalTexture;
Texture quitHoverTexture;

bool shouldClose = false;

void MainMenu ::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	StartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Start");
	auto start = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(StartButton->entityID);
	start->onClick = []() { std::cout << "Button clicked START!!!!!!\n"; GameManager::SetActiveScene("GameScene"); };
	ExitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit");
	auto exit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(ExitButton->entityID);
	exit->onClick = []() { std::cout << "Button clicked QUIT!!!!!!\n"; GameManager::DuckEngine.CloseWindow(); };
	JournalBookButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlay");
	auto book = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(JournalBookButton->entityID);
	book->onClick = []() { std::cout << "Button clicked BOOK!!!!!!\n"; GameManager::SetActiveScene("HowToPlay"); };

	startButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(StartButton->entityID);
	exitButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ExitButton->entityID);

	startNormalTexture = AssetManager::GetTextureByName("start");
	startHoverTexture = AssetManager::GetTextureByName("start_click");

	quitNormalTexture = AssetManager::GetTextureByName("quit");
	quitHoverTexture = AssetManager::GetTextureByName("quit_click");

	start->onHover = []()
		{
			startButtonSpriteRenderer->texture = startHoverTexture;
		};

	start->onFinishHover = []()
		{
			startButtonSpriteRenderer->texture = startNormalTexture;
		};

	exit->onHover = []()
		{
			exitButtonSpriteRenderer->texture = quitHoverTexture;
		};

	exit->onFinishHover = []()
		{
			exitButtonSpriteRenderer->texture = quitNormalTexture;
		};

}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void MainMenu ::Start()
{
	Scene::Start();
	//DuckEngine::showDebugColliders = false;
}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
void MainMenu ::Update()
{
	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);
	// For each sound component, play the sound if it is set to play on start
	//for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
	//	SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

	//	if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
	//		soundComponent->Play();
	//	}
	//}
	//if (DuckEngine::DUCKENGINE_AssetManager.LoadTexture("Resources/Sprites/MainMenu/mainmenu.png")) {
	//	std::cerr << "Failed to load background texture!" << std::endl;
	//}
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}
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
		if (menusound) {
			std::cout << "Sound stopped\n";
			menusound->Stop();
		}
	}

	//if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	//{
	//	std::cout << "K is pressed!\n";
	//	DuckEngine::showDebugColliders = !DuckEngine::showDebugColliders;
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