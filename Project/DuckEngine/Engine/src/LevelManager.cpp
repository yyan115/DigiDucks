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

std::string ConstructSceneFilePath(std::string sceneName)
{
    std::string finalPath = "../Resources/Scenes/";
    finalPath += sceneName;
    finalPath += ".json";

    return finalPath;
}

void LevelManager::LoadLevel(const std::string& levelFile)
{
    //std::filesystem::path absolutePath = std::filesystem::absolute(levelFile);
    //std::cout << "Loading level from: " << absolutePath.string() << std::endl;
    // Load the level data from the JSON file
    std::string finalPath = ConstructSceneFilePath(levelFile);
    json levelData = Serialization::LoadJsonFile(finalPath.c_str());
    std::cout << finalPath << std::endl;

    if (levelData.contains("gameObjects"))
    {
        auto gameObjects = levelData["gameObjects"];
        for (auto& [gameObjectName, gameObjectData] : gameObjects.items())
        {
            std::string prefabName = gameObjectData.value("prefab", "");

            if (!prefabName.empty())
            {
                std::shared_ptr<Prefab> prefab = std::shared_ptr<Prefab>(PrefabManager::GetPrefab(prefabName.c_str()));

                if (prefab)
                {
                    Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
                    entity->name = gameObjectName;
                    entity->prefabName = prefabName;

                    ComponentFactory::AddComponentsToEntity(entity, prefab->componentsData);

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
                Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
                entity->name = gameObjectName;

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
std::string LevelManager::OpenFileDialog(const std::string& fileType) {
    wchar_t fileName[260] = L"";
    wchar_t defExt[10];

    // Set up the OPENFILENAME structure
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    // Select filter and default extension based on fileType
    if (fileType == "scene") {
        ofn.lpstrFilter = L"JSON Files\0*.json\0All Files\0*.*\0";
        wcscpy_s(defExt, L"json");
    }
    else if (fileType == "texture") {
        ofn.lpstrFilter = L"Image Files (.png;*.jpg;*.jpeg)\0*.png;*.jpg;*.jpeg\0All Files\0*.*\0";
        wcscpy_s(defExt, L"png");
    }
    else if (fileType == "audio") {
        ofn.lpstrFilter = L"Audio Files (*.ogg;*.mp3;*.wav)\0*.ogg;*.mp3;*.wav\0All Files\0*.*\0";
        wcscpy_s(defExt, L"mp3");
    }
    else {
        // Default to all files if fileType is unknown
        ofn.lpstrFilter = L"All Files\0*.*\0";
    }
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = defExt;                  // Use the passed-in default extension

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
    std::string levelFile = OpenFileDialog("scene");

    // Load the selected level file
    LoadLevel(levelFile);

}

std::string GetPrefabName(int entityID)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (entity && !entity->prefabName.empty())
    {
        std::cout << entity->prefabName << std::endl;
        return entity->prefabName;
    }
    return "";
}


void LevelManager::SaveSceneChanges(const std::string& sceneName)
{
    std::string finalPath = ConstructSceneFilePath(sceneName);
    json sceneData = Serialization::LoadJsonFile(finalPath);

    sceneData["gameObjects"].clear();

    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    for (auto& entity : entities)
    {
        std::string entityName = entity.name.empty() ? "GameObject " + std::to_string(entity.entityID) : entity.name;

        if (!entity.prefabName.empty())
        {
            json& gameObjectData = sceneData["gameObjects"][entityName];
            gameObjectData["prefab"] = entity.prefabName;

            if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity.entityID))
            {
                gameObjectData["position"]["x"] = transform->position.x;
                gameObjectData["position"]["y"] = transform->position.y;
            }
        }
        else
        {
            json& gameObjectData = sceneData["gameObjects"][entityName];
            ComponentFactory::SaveComponentsToJson(entity.entityID, gameObjectData["components"]);
        }
    }

    Serialization::SaveJsonFile(finalPath, sceneData);
    std::cout << "Scene changes saved: " << sceneName << std::endl;
}



void LevelManager::SaveEntityChanges(int entityID, std::string& sceneName)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (!entity) return;

    std::string finalPath = ConstructSceneFilePath(sceneName);
    json sceneData = Serialization::LoadJsonFile(finalPath);

    std::string entityName = entity->name.empty() ? "Entity_" + std::to_string(entityID) : entity->name;
    json& gameObjectData = sceneData["gameObjects"][entityName];

    if (gameObjectData.contains("components"))
    {
        ComponentFactory::SaveComponentsToJson(entityID, gameObjectData["components"]);
    }
    else
    {
        if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID)) {
            gameObjectData["position"]["x"] = transform->position.x;
            gameObjectData["position"]["y"] = transform->position.y;
        }
    }

    Serialization::SaveJsonFile("../Resources/Scenes/SpriteMovementScene.json", sceneData);
    std::cout << "Entity changes saved for: " << entityName << std::endl;
}




void LevelManager::OverwritePrefab(int entityID)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (!entity) return;

    std::string prefabPath = "../Resources/Prefab.json";
    json prefabData = Serialization::LoadJsonFile(prefabPath);

    std::string prefabName = entity->name;

    if (prefabData["prefabs"].contains(prefabName)) 
    {
        ComponentFactory::SaveComponentsToJson(entityID, prefabData["prefabs"][prefabName]["components"]);

        Serialization::SaveJsonFile(prefabPath, prefabData);

        std::cout << "Prefab '" << prefabName << "' has been updated with the new component values." << std::endl;
    }
    else {
        std::cerr << "Error: Prefab not found for entity " << prefabName << std::endl;
    }
}

