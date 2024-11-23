/******************************************************************************
\file       SceneManager.cpp
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Implementation of the SceneManager class, which manages the loading,
            switching, updating, and unloading of scenes in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include <iostream>
#include "DuckEngine.h"
#include "SceneManager.h"

/**************************************************************************
@brief Initializes the SceneManager. Placeholder for setup logic.
**************************************************************************/
void SceneManager::Initialize()
{
}

/**************************************************************************
@brief Adds a scene to the manager's scene list.
@param name The name of the scene to add.
@param scene A shared pointer to the scene object.
**************************************************************************/
void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene) 
{
    scenes[name] = scene;
    scene->sceneName = name;
}

/**************************************************************************
@brief Sets the specified scene as active, unloading the previous scene and
       loading the new one.
@param name The name of the scene to activate.
**************************************************************************/
void SceneManager::SetActiveScene(const std::string& name)
{
    // Check if the scene being set is already the active scene
    if (activeSceneName == name)
    {
        // Reset the current active scene
        std::cout << "Scene '" << name << "' is already active. Resetting the scene." << std::endl;

        if (activeScene)
        {
            activeScene->Unload(); // Unload all entities and resources
        }

        LevelManager::LoadLevel(name); // Reload the level data
        if (activeScene)
        {
            activeScene->Load();  // Reload the active scene
            activeScene->Start(); // Restart the scene
        }

        return;
    }

    // Handle switching to a new scene
    auto it = scenes.find(name);
    if (it == scenes.end())
    {
        std::cerr << "Scene '" << name << "' not found!" << std::endl;
        return;
    }

    if (activeScene)
    {
        activeScene->Unload(); // Unload the previous scene
    }

    activeScene = scenes[name];
    activeSceneName = name;

    // Load level data and set up the new scene
    LevelManager::LoadLevel(name);
    activeScene->Load();
    activeScene->Start();
}



/**************************************************************************
@brief Updates the currently active scene by calling its Update function.
**************************************************************************/
void SceneManager::Update() 
{
    if (activeScene) 
    {
        activeScene->Update();
    }
}

/**************************************************************************
@brief Renders the currently active scene. Placeholder function.
**************************************************************************/
void SceneManager::Render() 
{
    if (activeScene)
    {
        activeScene->PostUpdate();
    }
}

/**************************************************************************
@brief Shuts down the SceneManager by unloading the active scene if one exists.
**************************************************************************/
void SceneManager::Shutdown() 
{
    if (activeScene) {
        activeScene->Unload();
    }
}