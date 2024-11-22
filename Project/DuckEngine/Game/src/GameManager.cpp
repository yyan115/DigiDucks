#include "GameManager.h"

DuckEngine GameManager::DuckEngine;

std::string GameManager::ActiveSceneName = "SpriteMovementScene";

void GameManager::InitScenes()
{
    DuckEngine.DUCKENGINE_SceneManager.AddScene("MaxLoadScene", std::make_shared<MaxLoadScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("SpriteMovementScene", std::make_shared<SpriteMovementScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("GameScene", std::make_shared<GameScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("MainMenu", std::make_shared<MainMenu>());
}

void GameManager::SetActiveScene(std::string sceneName)
{
    ActiveSceneName = sceneName;
    DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(sceneName);
}
