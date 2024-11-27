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

Entity* QuitButton;
//ButtonComponent* quit;


void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit");
	auto quit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	quit->onClick = []() { std::cout << "Button clicked QUIT!!!!!!\n"; };
}

void EndScene::Start()
{
	Scene::Start();
	DuckEngine::showDebugDraw = false;
}

void EndScene::Update()
{
	
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