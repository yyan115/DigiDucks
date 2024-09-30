#pragma warning(disable : 4251)
#pragma once
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "CameraComponent.h"
#include "RigidbodyComponent.h"
#include "Bounding.h"
#include "Serialization.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API ComponentManager
{
  private:
    std::unordered_map<std::type_index,
                       std::unordered_map<int, std::shared_ptr<Component>>>
        componentStorage;

  public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    // Add a component of any type to an entity
    template <typename T, typename... Args>
    T* AddComponent(int entityID, Args &&...args)
    {
        auto& typeMap = componentStorage[typeid(T)];
        std::shared_ptr<Component> component = std::make_shared<T>(std::forward<Args>(args)...);
        auto result = typeMap.emplace(entityID, component);

        std::cout << "Added component of type " << typeid(T).name()
            << " to entity " << entityID
            << " (Insertion " << (result.second ? "successful" : "failed") << ")"
            << std::endl;

        return std::static_pointer_cast<T>(result.first->second).get();
    }

    void AddComponent(int entityID, const std::shared_ptr<Component>& component)
    {
        auto& typeMap = componentStorage[typeid(*component)];
        typeMap[entityID] = component;
    }

    // Get a component of any type associated with an entity
    template <typename T> T *GetComponent(int entityID)
    {
        auto typeIt = componentStorage.find(typeid(T));
        if (typeIt != componentStorage.end())
        {
            auto componentIt = typeIt->second.find(entityID);
            if (componentIt != typeIt->second.end())
            {
                return std::static_pointer_cast<T>(componentIt->second).get();
            }
        }
        return nullptr;
    }

    // Get all components of a certain type (returns a map of entityID -> Component)
    template <typename T>
    std::unordered_map<int, std::shared_ptr<Component>>& GetComponents()
    {
        // Find the map for the specific component type
        return componentStorage[typeid(T)];
    }


    // Remove a component of any type from an entity
    template <typename T> void RemoveComponent(int entityID)
    {
        auto typeIt = componentStorage.find(typeid(T));
        if (typeIt != componentStorage.end())
        {
            typeIt->second.erase(entityID);
        }
    }

    // Check if an entity has a component of a certain type
    template <typename T> bool HasComponent(int entityID)
    {
        auto typeIt = componentStorage.find(typeid(T));
        if (typeIt != componentStorage.end())
        {
            return typeIt->second.find(entityID) != typeIt->second.end();
        }
        return false;
    }

    void RemoveAllComponents(int entityID)
    {
        if (entityID < 0) entityID = 0;
        int totalRemoved = 0;
        int totalRemaining = 0;

        for (auto& [type, componentMap] : componentStorage)
        {
            auto it = componentMap.find(entityID);
            if (it != componentMap.end())
            {
                componentMap.erase(it);
                totalRemoved++;
            }
        }

        for (const auto& [type, componentMap] : componentStorage)
        {
            if (componentMap.find(entityID) != componentMap.end())
            {
                totalRemaining++;
            }
        }

        std::cout << "Removed " << totalRemoved << " components for entity " << entityID << std::endl;
        std::cout << "Remaining components for entity " << entityID << ": " << totalRemaining << std::endl;
    }

};
