/******************************************************************************/
/*!
\file       ComponentFactory.cpp
\author     Jovan Chua 2301244
\par        c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Implementation of the ComponentFactory class to add components to entities based on JSON data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "ComponentFactory.h"


// Implementation of the function to add components to an entity based on the JSON data
/************************************************************************
@brief Adds various components to an entity based on the provided JSON data.
@param entity, a pointer to the entity to which the components will be added.
@param componentsData, a JSON array containing the component data.
@return nothing
*************************************************************************/
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
                color.r = static_cast<float>(componentData["properties"]["color"].value("r", 255));
                color.g = static_cast<float>(componentData["properties"]["color"].value("g", 255));
                color.b = static_cast<float>(componentData["properties"]["color"].value("b", 255));
                color.a = static_cast<float>(componentData["properties"]["color"].value("a", 255));
            }

            // Load the texture
            Texture texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(texturePath.c_str())[0];

            // Create and add the SpriteRendererComponent
            auto spriteRenderer = std::make_shared<SpriteRendererComponent>(sprite, layer, useColor, color);
            spriteRenderer->texture = texture;
            spriteRenderer->texturePath = texturePath;
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->entityID, *spriteRenderer);
        
            AnimatorComponent* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entity->entityID);
            if (animator && animator->currentAnimation) {
                // If the AnimatorComponent is present and it has a current animation, use the first frame
                texture = animator->currentAnimation->Frames[0];
            }
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
            std::vector<std::shared_ptr<Texture>> textures;
            if (componentData["properties"].contains("animations"))
            {
                for (const auto& animData : componentData["properties"]["animations"])
                {
                    std::string animName = animData["name"];
                    std::string textureResource = animData["texture"];
                    float frameDuration = animData["frameDuration"];

                    textures = DuckEngine::DUCKENGINE_AssetManager.LoadTexture(textureResource.c_str());
                    
                    animator->AddAnimation(animName, textures, textureResource, frameDuration);
                }
            }
            // Add the AnimatorComponent to the entity
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<AnimatorComponent>(entity->entityID, *animator);
        }
        // Sound Component
        else if (componentType == "SoundComponent") 
        {
            std::string soundID = componentData["properties"].value("soundID", "");
            bool loop = componentData["properties"].value("loop", false);
            bool playOnStart = componentData["properties"].value("playOnStart", false);
            float volume = componentData["properties"].value("volume", 1.0f);

            auto soundComponent = std::make_shared<SoundComponent>(soundID, loop, playOnStart, volume);
            DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SoundComponent>(entity->entityID, *soundComponent);
        }

    }
}

void ComponentFactory::SaveComponentsToJson(int entityID, json& componentsArray)
{
    componentsArray.clear();

    // Save TransformComponent.
    if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID)) 
    {
        json transformData;
        transformData["type"] = "TransformComponent";
        transformData["properties"]["position"] =
        {
            {"x", transform->position.x},
            {"y", transform->position.y}
        };
        transformData["properties"]["scale"] =
        {
            {"x", transform->scale.x},
            {"y", transform->scale.y}
        };
        transformData["properties"]["relativeToCamera"] = transform->relativeToCamera;
        componentsArray.push_back(transformData);
    }

    // Save SpriteRendererComponent.
    if (auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID)) 
    {
        json spriteData;
        spriteData["type"] = "SpriteRendererComponent";
        spriteData["properties"]["sprite"] = spriteRenderer->sprite;
        spriteData["properties"]["layer"] = spriteRenderer->layer;
        spriteData["properties"]["texture"] = spriteRenderer->GetFilePath();
        spriteData["properties"]["useColor"] = spriteRenderer->useColor;
        spriteData["properties"]["color"] =
        {
            {"r", spriteRenderer->color.r},
            {"g", spriteRenderer->color.g},
            {"b", spriteRenderer->color.b},
            {"a", spriteRenderer->color.a}
        };
        componentsArray.push_back(spriteData);
    }

    // Save BoundingBox.
    if (auto* boundingBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityID)) 
    {
        json boundingBoxData;
        boundingBoxData["type"] = "BoundingBox";
        boundingBoxData["properties"]["center"] =
        {
            {"x", boundingBox->getCenter().x},
            {"y", boundingBox->getCenter().y}
        };
        boundingBoxData["properties"]["size"] =
        {
            {"x", boundingBox->getSize().x},
            {"y", boundingBox->getSize().y}
        };
        componentsArray.push_back(boundingBoxData);
    }

    // Save BoundingCircle.
    if (auto* boundingCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityID)) 
    {
        json boundingCircleData;
        boundingCircleData["type"] = "BoundingCircle";
        boundingCircleData["properties"]["center"] =
        {
            {"x", boundingCircle->getCenter().x},
            {"y", boundingCircle->getCenter().y}
        };
        boundingCircleData["properties"]["radius"] = boundingCircle->getRadius();
        componentsArray.push_back(boundingCircleData);
    }

    // Save RigidbodyComponent.
    if (auto* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityID)) 
    {
        json rigidbodyData;
        rigidbodyData["type"] = "RigidbodyComponent";
        rigidbodyData["properties"]["isStatic"] = rigidbody->isStatic;
        componentsArray.push_back(rigidbodyData);
    }

    // Save SoundComponent.
    if (auto* sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(entityID))
    {
        json SoundData;
        SoundData["type"] = "SoundComponent";
        SoundData["properties"]["soundID"] = sound->soundID;
        SoundData["properties"]["loop"] = sound->loop;
        SoundData["properties"]["playOnStart"] = sound->playOnStart;
        SoundData["properties"]["volume"] = sound->volume;
        componentsArray.push_back(SoundData);
    }

    if (auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entityID))
    {
        json animatorData;
        animatorData["type"] = "AnimatorComponent";
        animatorData["properties"]["animations"] = json::array();

        for (const auto& [animName, animation] : animator->animations)
        {
            json animationData;
            animationData["name"] = animName;
            animationData["frameDuration"] = animation.frameDuration;

            animationData["texture"] = animation.animationFilePath;

            animatorData["properties"]["animations"].push_back(animationData);
        }

        componentsArray.push_back(animatorData);
    }
}


std::shared_ptr<Component> ComponentFactory::CreateComponentFromJson(const nlohmann::json& componentJson)
{
    std::string type = componentJson["type"];

    if (type == "TransformComponent")
    {
        Vec2 position = Serialization::GetVec2(componentJson["properties"], "position", Vec2(0.0f, 0.0f));
        Vec2 scale = Serialization::GetVec2(componentJson["properties"], "scale", Vec2(1.0f, 1.0f));
        bool relativeToCamera = componentJson["properties"].value("relativeToCamera", true);

        auto transformComponent = std::make_shared<TransformComponent>(position, scale);
        transformComponent->relativeToCamera = relativeToCamera;
        return transformComponent;
    }

    else if (type == "RigidbodyComponent")
    {
        auto rbComponent = std::make_shared<RigidbodyComponent>();
        rbComponent->isStatic = componentJson["properties"].value("isStatic", false);
        return rbComponent;
    }

    else if (type == "SpriteRendererComponent")
    {
        bool sprite = componentJson["properties"]["sprite"];
        int layer = componentJson["properties"]["layer"];
        std::string texturePath = componentJson["properties"]["texture"];
        bool useColor = componentJson["properties"]["useColor"];

        Color color;
        if (componentJson["properties"].contains("color"))
        {
            color.r = componentJson["properties"]["color"]["r"];
            color.g = componentJson["properties"]["color"]["g"];
            color.b = componentJson["properties"]["color"]["b"];
            color.a = componentJson["properties"]["color"]["a"];
        }

        auto spriteRenderer = std::make_shared<SpriteRendererComponent>(sprite, layer, useColor, color);
        spriteRenderer->texturePath = texturePath;
        return spriteRenderer;
    }

    else if (type == "BoundingBox")
    {
        Vec2 center = Serialization::GetVec2(componentJson["properties"], "center", Vec2(0.0f, 0.0f));
        Vec2 size = Serialization::GetVec2(componentJson["properties"], "size", Vec2(1.0f, 1.0f));

        auto boundingBox = std::make_shared<BoundingBox>(center, size);
        return boundingBox;
    }

    else if (type == "BoundingCircle")
    {
        Vec2 center = Serialization::GetVec2(componentJson["properties"], "center", Vec2(0.0f, 0.0f));
        float radius = componentJson["properties"].value("radius", 1.0f);

        auto boundingCircle = std::make_shared<BoundingCircle>(center, radius);
        return boundingCircle;
    }

    else if (type == "AnimatorComponent")
    {
        auto animator = std::make_shared<AnimatorComponent>();
        std::vector<std::shared_ptr<Texture>> textures;
        if (componentJson["properties"].contains("animations"))
        {
            for (const auto& animData : componentJson["properties"]["animations"])
            {
                std::string animName = animData["name"];
                std::string textureResource = animData["texture"];
                float frameDuration = animData["frameDuration"];

                textures = DuckEngine::DUCKENGINE_AssetManager.LoadTexture(textureResource.c_str(), 19, 24);

                animator->AddAnimation(animName, textures, textureResource, frameDuration);
            }
        }
        return animator;
    }



    else if (type == "SoundComponent")
    {
        auto soundComponent = std::make_shared<SoundComponent>("", false, false, 1.0f);
        soundComponent->soundID = componentJson["properties"]["soundID"];
        soundComponent->loop = componentJson["properties"].value("loop", false);
        soundComponent->playOnStart = componentJson["properties"].value("playOnStart", false);
        soundComponent->volume = componentJson["properties"].value("volume", 1.0f);
        return soundComponent;
    }

    std::cerr << "Error: Unknown component type: " << type << std::endl;
    return nullptr;
}
