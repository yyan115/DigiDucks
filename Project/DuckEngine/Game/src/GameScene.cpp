#include "GameScene.h"
#include "DuckEngine.h"
#include <random>
#include <chrono>
#include <map>
#include "ImageLoader.h"
#include "ResourcePath.h"

/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
void GameScene::Load() {
    DuckEngine::EnableLogging(false);

    DuckEngine::SetCameraHeight(20);

    DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CRATE.c_str());

    LevelManager::LoadLevelGame("../Resources/Scenes/GameScene.json");

}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void GameScene::Start() {

}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
void GameScene::Update() {

}

/// <summary>
/// Performs any operations after the main update logic, such as cleanup or post-processing.
/// This function is called every frame, after the Update() method.
/// </summary>
void GameScene::PostUpdate() {

}

/// <summary>
/// Exits the scene, performing any cleanup necessary before the scene is unloaded.
/// </summary>
void GameScene::Exit() {

}

/// <summary>
/// Unloads the scene and frees any resources that were loaded during the Load() phase.
/// </summary>
void GameScene::Unload() {

}