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
        // Initialize the scene manager with available scenes
        void Initialize();

        DUCKENGINE_API void AddScene(const std::string& name, std::shared_ptr<Scene> scene);

        void ActivateSceneWithoutReload(const std::string& name);

        // Switch scenes
        DUCKENGINE_API void SetActiveScene(const std::string& name);

        // Update current active scene
        void Update();

        // Render current active scene
        void Render();

        void Shutdown();

        std::string GetActiveSceneName() { return activeSceneName; }

    private:
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
        std::shared_ptr<Scene> activeScene;
        std::string activeSceneName;
    };