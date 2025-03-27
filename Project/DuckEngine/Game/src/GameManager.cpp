/******************************************************************************/
/*!
\file       GameManager.cpp
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Implementation of the GameManager class, it handles the changing of scenes whenever it is called.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameManager.h"
#include "GameLogicComponent.h"
#include "MovementLogic.h"
#include "PlayerLogic.h"
#include "HoldingLogic.h"

#include "StockLogic.h"
#include "BunStockLogic.h"
#include "CheeseStockLogic.h"
#include "LettuceStockLogic.h"
#include "MushroomStockLogic.h"
#include "ShrimpStockLogic.h"
#include "SteakStockLogic.h"
#include "TomatoStockLogic.h"
#include "WhitePlateStockLogic.h"
#include "BowlStockLogic.h"
#include "BinLogic.h"
#include "TableLogic.h"
#include "StoveLogic.h"
#include "ChopBoardLogic.h"
#include "SubmitLogic.h"
#include "RobotLogic.h"
#include "RestockLogic.h"
#include "SliderLogic.h"
#include "ScoreLogic.h"
#include "CustomerLogic.h"
#include "CutSceneLogic.h"
#include "LevelSelectScreenLogic.h"
#include "OrderTabLogic.h"
#include "HowToPlayLogic.h"
#include "PauseMenuLogic.h"
#include "GetPanLogic.h"
#include "GetPotLogic.h"
#include "UISliderLogic.h"
#include "GameSettingsLogic.h"
#include "GameLoopLogic.h"
#include "MiniGameLogic.h"
#include "PotLogic.h"
#include "HighlightLogic.h"
#include "ExitConfirmLogic.h"

// scenes
#include "MainMenu.h"
#include "HowToPlay.h"
#include "Level0.h"
#include "Level1.h"
#include "Level1_5.h"
#include "Level2.h"
#include "Level2_5.h"
#include "Level3.h"
#include "Level3_5.h"
#include "EndGame.h"


DuckEngine GameManager::DuckEngine;

std::string GameManager::ActiveSceneName = "SpriteMovementScene";
bool GameManager::ShouldChangeScene = false;
std::unordered_map<std::string, std::string> GameManager::GlobalVariables;

bool GameManager::GamepadDown = false;


 /****************************************************************
 * @brief Initializes all the scenes and gameplay logic for the game and is responsible for setting up all the scenes and game logic components that
 * will be used throughout the game.
 * ****************************************************************/
void GameManager::InitScenes()
{
	DuckEngine.DUCKENGINE_SceneManager.AddScene("MainMenu", std::make_shared<MainMenu>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("HowToPlay", std::make_shared<HowToPlay>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("GameScene", std::make_shared<Level1>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("EndScene", std::make_shared<EndScene>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level0", std::make_shared<Level0>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level1", std::make_shared<Level1>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level1_5", std::make_shared<Level1_5>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level2", std::make_shared<Level2>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level2_5", std::make_shared<Level2_5>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level3", std::make_shared<Level3>());
	DuckEngine.DUCKENGINE_SceneManager.AddScene("Level3_5", std::make_shared<Level3_5>());


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
	GameLogicManager::AddLogic("WhitePlateStockLogic", std::make_shared<WhitePlateStockLogic>());
	GameLogicManager::AddLogic("BowlStockLogic", std::make_shared<BowlStockLogic>());
	GameLogicManager::AddLogic("BinLogic", std::make_shared<BinLogic>());
	GameLogicManager::AddLogic("TableLogic", std::make_shared<TableLogic>());
	GameLogicManager::AddLogic("StoveLogic", std::make_shared<StoveLogic>());
	GameLogicManager::AddLogic("ChopBoardLogic", std::make_shared<ChopBoardLogic>());
	GameLogicManager::AddLogic("SubmitLogic", std::make_shared<SubmitLogic>());
	GameLogicManager::AddLogic("RestockLogic", std::make_shared<RestockLogic>());
	GameLogicManager::AddLogic("RobotLogic", std::make_shared<RobotLogic>());
	GameLogicManager::AddLogic("SliderLogic", std::make_shared<SliderLogic>());
	GameLogicManager::AddLogic("ScoreLogic", std::make_shared<ScoreLogic>());
	GameLogicManager::AddLogic("LevelSelectScreenLogic", std::make_shared<LevelSelectScreenLogic>());
	GameLogicManager::AddLogic("OrderTabLogic", std::make_shared<OrderTabLogic>());
	GameLogicManager::AddLogic("PauseMenuLogic", std::make_shared<PauseMenuLogic>());
	GameLogicManager::AddLogic("GetPanLogic", std::make_shared<GetPanLogic>());
	GameLogicManager::AddLogic("GetPotLogic", std::make_shared<GetPotLogic>());
	GameLogicManager::AddLogic("PotLogic", std::make_shared<PotLogic>());
	GameLogicManager::AddLogic("UISliderLogic", std::make_shared<UISliderLogic>());
	GameLogicManager::AddLogic("HighlightLogic", std::make_shared<HighlightLogic>());
	GameLogicManager::AddLogic("ExitConfirmLogic", std::make_shared<ExitConfirmLogic>());
	// Main Menu Logic
	GameLogicManager::AddLogic("HowToPlayLogic", std::make_shared<HowToPlayLogic>());
	GameLogicManager::AddLogic("GameSettingsLogic", std::make_shared<GameSettingsLogic>());
	// Customer Logic
	GameLogicManager::AddLogic("CustomerLogic", std::make_shared<CustomerLogic>());
	GameLogicManager::AddLogic("CutSceneLogic", std::make_shared<CutSceneLogic>());


	GameLogicManager::AddLogic("GameLoopLogic", std::make_shared<GameLoopLogic>());
	GameLogicManager::AddLogic("MiniGameLogic", std::make_shared<MiniGameLogic>());
}

 /****************************************************************
 * @brief Updates the game state and handles scene transitions.
 * ****************************************************************/
void GameManager::Update()
{
	if (ShouldChangeScene)
	{
		DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(ActiveSceneName);
		ShouldChangeScene = false;
	}
}


/****************************************************************
* @brief Sets the active scene for the game, optionally triggering a scene transition.
* ****************************************************************/
void GameManager::SetActiveScene(std::string sceneName, bool transition)
{
	ActiveSceneName = sceneName;
	if (transition)
	{
		ShouldChangeScene = true;
	}
	else
	{
		DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(ActiveSceneName);
	}
}

// Set a global variable
void GameManager::SetGlobalVariable(const std::string& key, const std::string& value)
{
	GlobalVariables[key] = value;
}

// Get a global variable
std::string GameManager::GetGlobalVariable(const std::string& key)
{
	if (GlobalVariables.find(key) != GlobalVariables.end())
	{
		return GlobalVariables[key];
	}
	return ""; // Return empty string if key doesn't exist
}
