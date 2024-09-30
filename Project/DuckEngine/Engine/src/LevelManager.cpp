#include "LevelManager.h"
#include "Serialization.h"
#include "DuckEngine.h"
#include "Prefab.h"
#include "LevelManager.h"
#include "PrefabManager.h"
#include "ComponentFactory.h"


void LevelManager::LoadLevel(const std::string& levelFile)
{
    // Load the level data from the JSON file
    json levelData = Serialization::LoadJsonFile(levelFile);

    // Iterate over the game objects
    if (levelData.contains("gameObjects"))
    {
        auto gameObjects = levelData["gameObjects"];
        for (auto& [gameObjectName, gameObjectData] : gameObjects.items())
        {
            // Extract the custom name of the entity
            std::string entityName = gameObjectData.value("name", "");

            // Check if the game object is a prefab or a custom entity
            std::string prefabName = gameObjectData.value("prefab", "");

            if (!prefabName.empty())
            {
                // If the game object uses a prefab, retrieve the prefab from the manager
                std::shared_ptr<Prefab> prefab = PrefabManager::GetPrefab(prefabName);

                if (prefab)
                {
                    // Create the entity
                    Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
                    entity->name = entityName;

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
                entity->name = entityName;

                // Add components directly to the entity using ComponentLoader
                ComponentFactory::AddComponentsToEntity(entity, gameObjectData["components"]);

                std::cout << "Created entity '" << entityName << "' with custom components." << std::endl;
            }
        }
    }
}
