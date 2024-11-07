/******************************************************************************
\file       SceneManager.h
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Declaration of the SceneManager class, which manages the loading,
            switching, updating, and unloading of scenes in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include "Scene.h"
#include <unordered_map>
#include <memory>


class SceneManager {
    public:
        /**************************************************************************
        @brief Initializes the SceneManager with available scenes.
        **************************************************************************/
        void Initialize();

        /**************************************************************************
        @brief Adds a scene to the scene manager.
        @param name The name of the scene to add.
        @param scene A shared pointer to the scene object to add.
        **************************************************************************/
        DUCKENGINE_API void AddScene(const std::string& name, std::shared_ptr<Scene> scene);

        /**************************************************************************
        @brief Sets the specified scene as the active scene.
        @param name The name of the scene to activate.
        **************************************************************************/
        DUCKENGINE_API void SetActiveScene(const std::string& name);

        /**************************************************************************
        @brief Updates the currently active scene.
        **************************************************************************/
        void Update();

        void PostUpdate();

        /**************************************************************************
        @brief Renders the currently active scene.
        **************************************************************************/
        void Render();

        /**************************************************************************
        @brief Unloads all active scenes and prepares the manager for shutdown.
        **************************************************************************/
        void Shutdown();

        /**************************************************************************
        @brief Gets the name of the currently active scene.
        @return The name of the active scene as a string.
        **************************************************************************/
        std::string GetActiveSceneName() { return activeSceneName; }

        /**************************************************************************
        @brief Gets a pointer to the currently active scene.
        @return A pointer to the active scene object.
        **************************************************************************/
        Scene* GetActiveScene() { return activeScene.get(); }

    private:
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
        std::shared_ptr<Scene> activeScene;
        std::string activeSceneName;
    };