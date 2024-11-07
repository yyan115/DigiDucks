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
    auto it = scenes.find(name);

    if (it == scenes.end())
    {
        std::cerr << "Scene '" << name << "' not found!" << std::endl;
        return;
    }

    if (activeScene)
    {
        activeScene->Unload();
    }

    activeScene = scenes[name];
    activeSceneName = name;

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

void SceneManager::PostUpdate()
{
    if (activeScene)
    {
        activeScene->PostUpdate();
    }
}

/**************************************************************************
@brief Renders the currently active scene. Placeholder function.
**************************************************************************/
void SceneManager::Render() 
{
    
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