#include "GameManager.h"
#include "GameLogicComponent.h"
#include "MovementLogic.h"
#include "PlayerLogic.h"
#include "HoldingLogic.h"
#include "BunStockLogic.h"
#include "CheeseStockLogic.h"
#include "LettuceStockLogic.h"
#include "MushroomStockLogic.h"
#include "ShrimpStockLogic.h"
#include "SteakStockLogic.h"
#include "TomatoStockLogic.h"
#include "BinLogic.h"

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
	GameLogicManager::AddLogic("StockLogic", std::make_shared<StockLogic>());
	GameLogicManager::AddLogic("BunStockLogic", std::make_shared<BunStockLogic>());
	GameLogicManager::AddLogic("CheeseStockLogic", std::make_shared<CheeseStockLogic>());
	GameLogicManager::AddLogic("LettuceStockLogic", std::make_shared<LettuceStockLogic>());
	GameLogicManager::AddLogic("MushroomStockLogic", std::make_shared<MushroomStockLogic>());
	GameLogicManager::AddLogic("ShrimpStockLogic", std::make_shared<ShrimpStockLogic>());
	GameLogicManager::AddLogic("SteakStockLogic", std::make_shared<SteakStockLogic>());
	GameLogicManager::AddLogic("TomatoStockLogic", std::make_shared<TomatoStockLogic>());
	GameLogicManager::AddLogic("BinLogic", std::make_shared<BinLogic>());

}

void GameManager::SetActiveScene(std::string sceneName)
{
    ActiveSceneName = sceneName;
    DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(sceneName);
}
