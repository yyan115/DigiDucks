#include "LevelSelectScreenLogic.h"


//LevelSelectScreen
void LevelSelectScreenLogic::Start()
{
	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

	Entity* XButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectXButton").get();
	ButtonComponent* XButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(XButtonEntity->entityID);

	Entity* level0ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level0Button").get();
	ButtonComponent* level0Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level0ButtonEntity->entityID);

	Entity* level1ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1Button").get();
	ButtonComponent* level1Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1ButtonEntity->entityID);


	XButton->onClick = [this]()
		{
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = false;
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = true;
		};

	level0Button->onClick = [this]()
		{
			std::cout << "Level 0 button clicked!" << std::endl;
		};

	level1Button->onClick = [this]()
		{
			std::cout << "Level 1 button clicked!" << std::endl;
		};
}

void LevelSelectScreenLogic::Update()
{

}

void LevelSelectScreenLogic::FixedUpdate()
{

}