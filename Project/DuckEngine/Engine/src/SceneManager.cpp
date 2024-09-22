#include <iostream>

#include "SceneManager.h"

void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
}

void SceneManager::SetActiveScene(const std::string& name) {
    // If the requested scene is already active, don't reload it
    if (activeScene && scenes[name] == activeScene) {
        return;  // Scene is already active, do nothing
    }

    // Unload the current scene if it's different
    if (activeScene) 
    {
        activeScene->Unload();
    }

    // Set the new scene as active
    activeScene = scenes[name];
    activeScene->Load();
    activeScene->Start();
}


void SceneManager::Update() {
    if (activeScene) {
        activeScene->Update();
    }
}

void SceneManager::Render() {
    
}

void SceneManager::Shutdown() {
    if (activeScene) {
        activeScene->Unload();
    }
}