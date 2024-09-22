#include "EntityFactoryTwo.h"
#include "AssetManager.h"
#include "Bounding.h"

Entity* EntityFactoryTwo::CreatePlayer(const std::string& texturePath, const Vec2& position, const Vec2& scale, float moveSpeed)
{
    // Create the Player entity
    Entity* player = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

    // Add necessary components to the Player
    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(player->EntityID);
    RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(player->EntityID);
    SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(player->EntityID, true);

    // Set player properties
    transform->position = position;
    transform->scale = scale;
    rigidbody->velocity = Vec2(0.0f, 0.0f);  // No initial movement
    spriteRenderer->texture = *AssetManager::LoadTexture(texturePath);

    BoundingCircle* circle = new BoundingCircle(transform->position, 2.f);

    rigidbody->acceleration = moveSpeed;  

    return player;
}