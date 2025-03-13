#include "LevelSelectScreenLogic.h"
#include "MainMenu.h"
#include "DuckEngine_Input.h"

int LevelSelectScreenLogic::currentStage = -1;
int LevelSelectScreenLogic::stageLevel = 0;

//LevelSelectScreen
void LevelSelectScreenLogic::Start()
{
	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

	Entity* XButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectXButton").get();
	ButtonComponent* XButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(XButtonEntity->entityID);
	SoundComponent* SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(XButtonEntity->entityID);

	// Get all level buttons
	Entity* level0ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level0Button").get();
	ButtonComponent* level0Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level0ButtonEntity->entityID);
	SpriteRendererComponent* level0Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level0ButtonEntity->entityID);

	Entity* level1ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1Button").get();
	ButtonComponent* level1Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1ButtonEntity->entityID);
	SpriteRendererComponent* level1Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level1ButtonEntity->entityID);

	Entity* level1_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1.5Button").get();
	ButtonComponent* level1_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1_5ButtonEntity->entityID);
	SpriteRendererComponent* level1_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level1_5ButtonEntity->entityID);

	Entity* level2ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level2Button").get();
	ButtonComponent* level2Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level2ButtonEntity->entityID);
	SpriteRendererComponent* level2Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level2ButtonEntity->entityID);

	Entity* level2_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level2.5Button").get();
	ButtonComponent* level2_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level2_5ButtonEntity->entityID);
	SpriteRendererComponent* level2_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level2_5ButtonEntity->entityID);

	Entity* level3ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level3Button").get();
	ButtonComponent* level3Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level3ButtonEntity->entityID);
	SpriteRendererComponent* level3Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level3ButtonEntity->entityID);

	Entity* level3_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level3.5Button").get();
	ButtonComponent* level3_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level3_5ButtonEntity->entityID);
	SpriteRendererComponent* level3_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level3_5ButtonEntity->entityID);

	levelSelectScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID);
	mainMenuScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID);

	mainMenu = DuckEngine::DUCKENGINE_SceneManager.GetScene<MainMenu>("MainMenu").get();

	// X Button to return to main menu
	XButton->onClick = [this, SFX]()
		{
			SFX->Play();
			levelSelectScreenSpriteRenderer->isVisible = false;
			mainMenuScreenSpriteRenderer->isVisible = true;
		};

	// Set up level button visibility based on current stage
	// Level 0 (Tutorial) - Always available
	level0Button->onClick = [this, SFX]()
		{
			std::cout << "Level 0 button clicked!" << std::endl;
			SFX->Play();
			stageLevel = 0;
			mainMenu->OnPlayButtonClicked("Level0");
		};

	// Level 1 - Available if currentStage >= 0
	level1Button->onClick = [this, SFX]()
		{
			if (currentStage >= 0) {
				std::cout << "Level 1 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 1;
				mainMenu->OnPlayButtonClicked("Level1");
			}
		};

	// Level 1.5 - Available if currentStage >= 1
	level1_5Button->onClick = [this, SFX]()
		{
			if (currentStage >= 1) {
				std::cout << "Level 1.5 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 2;
				mainMenu->OnPlayButtonClicked("Level15");
			}
		};

	// Level 2 - Available if currentStage >= 2
	level2Button->onClick = [this, SFX]()
		{
			if (currentStage >= 2) {
				std::cout << "Level 2 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 3;
				mainMenu->OnPlayButtonClicked("Level2");
			}
		};

	// Level 2.5 - Available if currentStage >= 3
	level2_5Button->onClick = [this, SFX]()
		{
			if (currentStage >= 3) {
				std::cout << "Level 2.5 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 4;
				mainMenu->OnPlayButtonClicked("Level25");
			}
		};

	// Level 3 - Available if currentStage >= 4
	level3Button->onClick = [this, SFX]()
		{
			if (currentStage >= 4) {
				std::cout << "Level 3 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 5;
				mainMenu->OnPlayButtonClicked("Level3");
			}
		};

	// Level 3.5 - Available if currentStage >= 
	level3_5Button->onClick = [this, SFX]()
		{
			if (currentStage >= 5) {
				std::cout << "Level 3.5 button clicked!" << std::endl;
				SFX->Play();
				stageLevel = 6;
				mainMenu->OnPlayButtonClicked("Level35");
			}
		};

	UpdateLevelButtonVisuals(level0Sprite, level1Sprite, level1_5Sprite, level2Sprite,
		level2_5Sprite, level3Sprite, level3_5Sprite);
}

void LevelSelectScreenLogic::Update()
{

	// unlock every level
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_0))
	{
		currentStage = 100;
		Start();
	}

}

void LevelSelectScreenLogic::FixedUpdate()
{
}

void LevelSelectScreenLogic::UpdateLevelButtonVisuals(
	SpriteRendererComponent* level0Sprite,
	SpriteRendererComponent* level1Sprite,
	SpriteRendererComponent* level1_5Sprite,
	SpriteRendererComponent* level2Sprite,
	SpriteRendererComponent* level2_5Sprite,
	SpriteRendererComponent* level3Sprite,
	SpriteRendererComponent* level3_5Sprite)
{
	std::string lockedTexture = "lvlselectbutton_locked";
	std::string unlockedTexture = "lvlselectbutton";

	level0Sprite->texture = AssetManager::GetTextureByName(unlockedTexture);

	level1Sprite->texture = AssetManager::GetTextureByName((currentStage >= 0) ? unlockedTexture : lockedTexture);
	level1_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 1) ? unlockedTexture : lockedTexture);
	level2Sprite->texture = AssetManager::GetTextureByName((currentStage >= 2) ? unlockedTexture : lockedTexture);
	level2_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 3) ? unlockedTexture : lockedTexture);
	level3Sprite->texture = AssetManager::GetTextureByName((currentStage >= 4) ? unlockedTexture : lockedTexture);
	level3_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 5) ? unlockedTexture : lockedTexture);
}


void LevelSelectScreenLogic::LevelCompleted() 
{
	currentStage++;

	if (currentStage > 5)
	{
		currentStage = 5;
	}

}