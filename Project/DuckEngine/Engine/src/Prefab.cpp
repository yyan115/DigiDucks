#include "Prefab.h"
#include "DuckEngine.h"
#include <cstring>  // For strcpy and strlen

struct Prefab::Impl
{
    char* name;
    char* texturePath;
    Vec2 position;
    Vec2 scale;
    std::vector<std::shared_ptr<Component>> components;
    nlohmann::json componentsData;

    Impl()
        : name(nullptr), texturePath(nullptr), position(Vec2(0.0f, 0.0f)), scale(Vec2(1.0f, 1.0f)), componentsData({})
    {}

    ~Impl()
    {
        delete[] name;
        delete[] texturePath;
    }
};


Prefab::Prefab()
    : impl(new Impl())
{

}
Prefab::Prefab(const char* name, const char* texturePath, Vec2 scale)
    : impl(new Impl())
{
    size_t nameLen = std::strlen(name) + 1;
    impl->name = new char[nameLen];
    strcpy_s(impl->name, nameLen, name);  

    size_t textureLen = std::strlen(texturePath) + 1;
    impl->texturePath = new char[textureLen];
    strcpy_s(impl->texturePath, textureLen, texturePath); 

    impl->scale = scale;
}


Prefab::~Prefab()
{
    delete impl;
}

void Prefab::AddComponent(const std::shared_ptr<Component>& component)
{
    impl->components.push_back(component);
}

Entity* Prefab::Instantiate(Vec2 newPosition)
{
    // Create a new entity using the EntityFactory
    Entity* entity = EntityFactory::CreateEntity(impl->texturePath, newPosition, impl->scale);

    // Loop through each component and clone it
    for (const std::shared_ptr<Component>& component : impl->components)
    {
        // Clone the component
        std::shared_ptr<Component> clonedComponent = component->Clone();

        // If it's a SpriteRendererComponent, make sure the texture is set properly
        if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(clonedComponent))
        {
            // Ensure the texture is loaded if not already
            if (spriteRenderer->texture)
            {
                spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(impl->texturePath)[0];
            }
        }

        // Add the component to the entity
        DuckEngine::DUCKENGINE_ComponentManager.AddComponent(entity->entityID, clonedComponent);
    }

    return entity;
}

nlohmann::json& Prefab::GetComponentsData()
{
    return impl->componentsData;
}

void Prefab::SetComponentsData(nlohmann::json& components)
{
    impl->componentsData = components;
}
