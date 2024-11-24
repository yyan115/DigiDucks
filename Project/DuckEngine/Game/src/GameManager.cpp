#include "GameManager.h"
#include "GameLogicComponent.h"
#include "MovementLogic.h"
#include "PlayerLogic.h"
#include "HoldingLogic.h"

DuckEngine GameManager::DuckEngine;

std::string GameManager::ActiveSceneName = "SpriteMovementScene";

void GameManager::InitScenes()
{
    DuckEngine.DUCKENGINE_SceneManager.AddScene("MaxLoadScene", std::make_shared<MaxLoadScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("SpriteMovementScene", std::make_shared<SpriteMovementScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("GameScene", std::make_shared<GameScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("MainMenu", std::make_shared<MainMenu>());


    // add game logic
    GameLogicManager::AddLogic("MovementLogic", std::make_shared<MovementLogic>());
	GameLogicManager::AddLogic("PlayerLogic", std::make_shared<PlayerLogic>());
	GameLogicManager::AddLogic("HoldingLogic", std::make_shared<HoldingLogic>());
}

void GameManager::SetActiveScene(std::string sceneName)
{
    ActiveSceneName = sceneName;
    DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(sceneName);
}
