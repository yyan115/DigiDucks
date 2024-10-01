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


void LevelManager::LoadLevel(const std::string& levelFile)
{
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

void LevelManager::OpenLevelDialog()
{
    std::string levelFile = OpenFileDialog();

    // Load the selected level file
    LoadLevel(levelFile);

}