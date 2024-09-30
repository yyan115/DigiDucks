#include "ComponentFactory.h"

// Implementation of the function to add components to an entity based on the JSON data
void ComponentFactory::AddComponentsToEntity(Entity* entity, const nlohmann::json& componentsData)
{
    for (const auto& componentData : componentsData)
    {
        std::string componentType = componentData["type"];

        // TransformComponent
        if (componentType == "TransformComponent")
        {
            Vec2 position = Serialization::GetVec2(componentData["properties"], "position", Vec2(0.0f, 0.0f));
            Vec2 scale = Serialization::GetVec2(componentData["properties"], "scale", Vec2(1.0f, 1.0f));
            bool relativeToCamera = componentData["properties"].value("relativeToCamera", true);

            // Create and add the TransformComponent
            auto transform = std::make_shared<TransformComponent>(position, scale);
            transform->relativeToCamera = relativeToCamera;
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID, *transform);
        }
        // SpriteRendererComponent
        else if (componentType == "SpriteRendererComponent")
        {
            bool sprite = componentData["properties"].value("sprite", true);
            int layer = componentData["properties"].value("layer", 0);
            std::string texturePath = componentData["properties"].value("texture", "");
            bool useColor = componentData["properties"].value("useColor", false);

            // Extract color with default values
            Color color = { 255, 255, 255, 255 };
            if (componentData["properties"].contains("color")) {
                color.r = componentData["properties"]["color"].value("r", 255);
                color.g = componentData["properties"]["color"].value("g", 255);
                color.b = componentData["properties"]["color"].value("b", 255);
                color.a = componentData["properties"]["color"].value("a", 255);
            }

            // Load the texture
            Texture texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(texturePath)[0];

            // Create and add the SpriteRendererComponent
            auto spriteRenderer = std::make_shared<SpriteRendererComponent>(sprite, layer, useColor, color);
            spriteRenderer->texture = texture;
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->entityID, *spriteRenderer);
        }
        // BoundingBox
        else if (componentType == "BoundingBox")
        {
            Vec2 center = Serialization::GetVec2(componentData["properties"], "center", Vec2(0.0f, 0.0f));
            Vec2 size = Serialization::GetVec2(componentData["properties"], "size", Vec2(1.0f, 1.0f));

            // Create and add the BoundingBox
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(entity->entityID, center, size);
        }
        // BoundingCircle
        else if (componentType == "BoundingCircle")
        {
            float radius = componentData["properties"]["radius"];
            Vec2 center = Serialization::GetVec2(componentData["properties"], "center", Vec2(0.0f, 0.0f));

            // Create and add the BoundingCircle
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(entity->entityID, center, radius);
        }
        // RigidbodyComponent
        else if (componentType == "RigidbodyComponent")
        {
            bool isStaticComponent = componentData["properties"].value("isStatic", false);

            // Create and add the RigidbodyComponent
            auto rbComponent = std::make_shared<RigidbodyComponent>();
            rbComponent->isStatic = isStaticComponent;
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(entity->entityID, *rbComponent);
        }
        // AnimatorComponent
        else if (componentType == "AnimatorComponent")
        {
            auto animator = std::make_shared<AnimatorComponent>();

            if (componentData["properties"].contains("animations"))
            {
                for (const auto& animData : componentData["properties"]["animations"])
                {
                    std::string animName = animData["name"];
                    std::string textureResource = animData["texture"];
                    float frameDuration = animData["frameDuration"];

                    // Load the texture and add the animation to the AnimatorComponent
                    std::vector<std::shared_ptr<Texture>> textures = DuckEngine::DUCKENGINE_AssetManager.LoadTexture(textureResource);
                    animator->AddAnimation(animName, textures, frameDuration);
                }
            }

            // Add the AnimatorComponent to the entity
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<AnimatorComponent>(entity->entityID, *animator);
        }
    }
}
