/******************************************************************************/
/*
\file:      LevelManager.cpp
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:	    yankai.tan@digipen.edu


\brief:     Contains the definitions that are used to load the level

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#include "LevelManager.h"
#include "Serialization.h"
#include "DuckEngine.h"
#include "Prefab.h"
#include "LevelManager.h"
#include "PrefabManager.h"
#include "ComponentFactory.h"
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <filesystem>


void LevelManager::LoadLevel(const std::string& levelFile)
{
    std::filesystem::path absolutePath = std::filesystem::absolute(levelFile);
    std::cout << "Loading level from: " << absolutePath.string() << std::endl;
    // Load the level data from the JSON file
    json levelData = Serialization::LoadJsonFile(levelFile.c_str());

    // Iterate over the game objects
    if (levelData.contains("gameObjects"))
    {
        auto gameObjects = levelData["gameObjects"];
        for (auto& [gameObjectName, gameObjectData] : gameObjects.items())
        {
            // Check if the game object is a prefab or a custom entity
            std::string prefabName = gameObjectData.value("prefab", "");

            if (!prefabName.empty())
            {
                // If the game object uses a prefab, retrieve the prefab from the manager
                std::shared_ptr<Prefab> prefab = std::shared_ptr<Prefab>(PrefabManager::GetPrefab(prefabName.c_str()));

                if (prefab)
                {
                    // Create the entity
                    Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
                    entity->name = gameObjectName;

                    // Apply components from the prefab
                    ComponentFactory::AddComponentsToEntity(entity, prefab->componentsData);

                    // set position
                    if (gameObjectData.contains("position"))
                    {
                        Vec2 position = Serialization::GetVec2(gameObjectData, "position", Vec2(0.0f, 0.0f));
                        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);
                        if (transform)
                        {
                            transform->position.x = position.x;
                            transform->position.y = position.y;
                        }
                    }

                }
                else
                {
                    std::cerr << "Error: Could not find prefab: " << prefabName << std::endl;
                }
            }

            else if (gameObjectData.contains("components"))
            {
                // If the game object doesn't use a prefab, create the entity manually
                Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
                entity->name = gameObjectName;

                // Add components directly to the entity using ComponentLoader
                ComponentFactory::AddComponentsToEntity(entity, gameObjectData["components"]);

            }
        }
    }
}

/****************************************************************
* @brief Load level using data from json file
*
* @param levelFile - json file containing data for the level
*
***************************************************************/
std::string OpenFileDialog() {
    wchar_t fileName[260] = L"";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"JSON Files\0*.json\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"json";

    if (GetOpenFileName(&ofn)) {
        // Convert wide char to narrow char string
        char narrowFileName[260];
        size_t convertedChars = 0;
        wcstombs_s(&convertedChars, narrowFileName, sizeof(narrowFileName), fileName, _TRUNCATE);
        return std::string(narrowFileName);
    }
    return "";
}

/****************************************************************
* @brief Open Level Dialog
*
***************************************************************/
void LevelManager::OpenLevelDialog()
{
    std::string levelFile = OpenFileDialog();

    // Load the selected level file
    LoadLevel(levelFile);

}

void LevelManager::SaveEntityChanges(int entityID)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (!entity) return;

    // Load the current scene data.
    json sceneData = Serialization::LoadJsonFile("../Resources/Scenes/SpriteMovementScene.json");

    // Find or create the game object entry.
    std::string entityName = entity->name.empty() ? "Entity_" + std::to_string(entityID) : entity->name;
    json& gameObjectData = sceneData["gameObjects"][entityName];

    if (gameObjectData.contains("components"))
    {
        // Case 1: Entity has components (like obstacleObject4).
        ComponentFactory::SaveComponentsToJson(entityID, gameObjectData["components"]);
    }
    else
    {
        // Case 2: Prefab-based entity (like Player or Obstacle).
        if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID)) {
            gameObjectData["position"]["x"] = transform->position.x;
            gameObjectData["position"]["y"] = transform->position.y;
        }
    }

    // Save the updated scene data back to the file.
    Serialization::SaveJsonFile("../Resources/Scenes/SpriteMovementScene.json", sceneData);
    std::cout << "Entity changes saved for: " << entityName << std::endl;
}




void LevelManager::OverwritePrefab(int entityID)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (!entity) return;

    // Load the prefab data
    std::string prefabPath = "../Resources/Prefab.json";
    json prefabData = Serialization::LoadJsonFile(prefabPath);

    std::string prefabName = entity->name; // Assuming prefab name matches entity name

    if (prefabData["prefabs"].contains(prefabName)) {
        // Save the entity's components into the prefab JSON
        ComponentFactory::SaveComponentsToJson(entityID, prefabData["prefabs"][prefabName]["components"]);

        // Save the modified prefab JSON back to the file
        Serialization::SaveJsonFile(prefabPath, prefabData);

        std::cout << "Prefab '" << prefabName << "' has been updated with the new component values." << std::endl;
    }
    else {
        std::cerr << "Error: Prefab not found for entity " << prefabName << std::endl;
    }
}

