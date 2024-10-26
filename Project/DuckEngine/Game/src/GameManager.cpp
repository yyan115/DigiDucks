#include "GameManager.h"

DuckEngine GameManager::DuckEngine;

void GameManager::InitScenes()
{
    DuckEngine.DUCKENGINE_SceneManager.AddScene("MaxLoadScene", std::make_shared<MaxLoadScene>());
    DuckEngine.DUCKENGINE_SceneManager.AddScene("SpriteMovementScene", std::make_shared<SpriteMovementScene>());

}

void GameManager::SetActiveScene(std::string sceneName)
{
    DuckEngine.DUCKENGINE_SceneManager.SetActiveScene(sceneName);
}