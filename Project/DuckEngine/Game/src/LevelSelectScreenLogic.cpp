#include "LevelSelectScreenLogic.h"
#include "MainMenu.h"


//LevelSelectScreen
void LevelSelectScreenLogic::Start()
{
	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

	Entity* XButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectXButton").get();
	ButtonComponent* XButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(XButtonEntity->entityID);
	SoundComponent* SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(XButtonEntity->entityID);

	Entity* level0ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level0Button").get();
	ButtonComponent* level0Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level0ButtonEntity->entityID);

	Entity* level1ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1Button").get();
	ButtonComponent* level1Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1ButtonEntity->entityID);

	levelSelectScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID);
	mainMenuScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID);

	mainMenu = DuckEngine::DUCKENGINE_SceneManager.GetScene<MainMenu>("MainMenu").get();

	XButton->onClick = [this, SFX]()
		{
			SFX->Play();
			levelSelectScreenSpriteRenderer->isVisible = false;
			mainMenuScreenSpriteRenderer->isVisible = true;
			
		};

	level0Button->onClick = [this, SFX]()
		{
			std::cout << "Level 0 button clicked!" << std::endl;
			SFX->Play();
			mainMenu->OnPlayButtonClicked("Level0");
		};

	level1Button->onClick = [this, SFX]()
		{
			std::cout << "Level 1 button clicked!" << std::endl;
			SFX->Play();
			mainMenu->OnPlayButtonClicked("GameScene");
		};
}

void LevelSelectScreenLogic::Update()
{

}

void LevelSelectScreenLogic::FixedUpdate()
{

}