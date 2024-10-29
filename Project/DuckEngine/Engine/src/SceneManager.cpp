#include <iostream>

#include "SceneManager.h"

void SceneManager::Initialize()
{
}

void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene) 
{
    scenes[name] = scene;
}

void SceneManager::ActivateSceneWithoutReload(const std::string& name) 
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
}


void SceneManager::SetActiveScene(const std::string& name) 
{
    auto it = scenes.find(name);
    if (it == scenes.end()) 
    {
        std::cerr << "Scene '" << name << "' not found!" << std::endl;
        return;
    }

    if (activeScene == scenes[name]) 
    {
        std::cout << "Scene '" << name << "' is already active. No action taken." << std::endl;
        return;
    }

    if (activeScene) 
    {
        activeScene->Unload();
    }

    activeScene = scenes[name];

    activeScene->Load();
    activeScene->Start();
}

void SceneManager::Update() 
{
    if (activeScene) 
    {
        activeScene->Update();
    }
}

void SceneManager::Render() 
{
    
}

void SceneManager::Shutdown() 
{
    if (activeScene) {
        activeScene->Unload();
    }
}