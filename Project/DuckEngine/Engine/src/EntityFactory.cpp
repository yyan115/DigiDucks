#include "EntityFactory.h"
#include "DuckEngine.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Bounding.h"

Entity* EntityFactory::CreateEntity()
{
    Entity* entity = &(DuckEngine::DUCKENGINE_EntityManager.CreateEntity());
    return entity;
}

Entity* EntityFactory::CreateEntity(Vec2 position, Vec2 scale)
{
    Entity* entity = &(DuckEngine::DUCKENGINE_EntityManager.CreateEntity());
    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID);
    transform->position = position;
    transform->scale = scale;
    return entity;
}

Entity* EntityFactory::CreateEntity(const std::string& texturePath, Vec2 position, Vec2 scale)
{
    Entity* entity = &(DuckEngine::DUCKENGINE_EntityManager.CreateEntity());

    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID);
    SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->entityID, true);

    transform->position = position;
    transform->scale = scale;

    if (!texturePath.empty())
    {
        spriteRenderer->texture = *AssetManager::LoadTexture(texturePath)[0];
    }
    else
    {
        spriteRenderer->useColor = true;
        spriteRenderer->color = { 255.0f, 0.0f, 255.0f, 255.0f };
    }

    return entity;

}

//Entity* EntityFactory::CreateEntityWithCustomComponents(const std::vector<Component*>& customComponents)
//{
//    Entity* entity;
//    return entity;
//}

