#include "EntityFactory.h"
#include "DuckEngine.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Bounding.h"


Entity* EntityFactory::CreateEntity(const std::string& texturePath, Vec2 position, Vec2 scale)
{
    Entity* entity = &(DuckEngine::DUCKENGINE_EntityManager.CreateEntity());

    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->EntityID);
    RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(entity->EntityID);
    SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->EntityID, true);

    transform->position = position;
    transform->scale = scale;
    rigidbody->velocity = Vec2(0.0f, 0.0f);  // No initial movement
    spriteRenderer->texture = *AssetManager::LoadTexture(texturePath);
    BoundingCircle* circle = new BoundingCircle(transform->position, 2.f);

    return entity;

}

//Entity* EntityFactory::CreateEntityWithCustomComponents(const std::vector<Component*>& customComponents)
//{
//    Entity* entity;
//    return entity;
//}

