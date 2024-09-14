#pragma warning(disable : 4251)
#pragma once
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "CameraComponent.h"
#include "ComponentPool.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API ComponentManager
{
  private:
    std::unordered_map<std::type_index, std::shared_ptr<ComponentPoolBase>> componentPools;

    enum ComponentType
    {
        TRANSFORM_COMPONENT = 1 << 0,
        SPRITE_RENDERER_COMPONENT = 1 << 1,
        CAMERA_COMPONENT = 1 << 2,
    };

    template <typename T>
    ComponentPool<T>* GetOrCreatePool()
    {
        const std::type_index& typeIndex = typeid(T);
        if (componentPools.find(typeIndex) == componentPools.end()) 
        {
            componentPools[typeIndex] = std::make_unique<ComponentPool<T>>();
        }
        return static_cast<ComponentPool<T>*>(componentPools[typeIndex].get());
    }


  public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    template <typename T, typename... Args>
    T* AddComponent(int entityID, Args&&... args)
    {
        ComponentPool<T>& pool = *GetOrCreatePool<T>();
        T component(std::forward<Args>(args)...);
        std::shared_ptr<T> compPtr = pool.AddComponent(entityID, component);
        return compPtr.get();
    }

    template <typename T>
    T* GetComponent(int entityID)
    {
        ComponentPool<T>& pool = *GetOrCreatePool<T>();
        std::shared_ptr<T> compPtr = pool.GetComponent(entityID);
        return compPtr ? compPtr.get() : nullptr;
    }

    template <typename T>
    std::vector<T*> GetComponents()
    {
        ComponentPool<T>* pool = GetOrCreatePool<T>();
        std::vector<T*> components;

        for (auto& component : pool->components)
        {
            components.push_back(component.get());
        }
        return components;
    }

    // Remove a component of any type from an entity
    template <typename T>
    void RemoveComponent(int entityID) 
    {
        ComponentPool<T>& pool = *GetOrCreatePool<T>();
        pool.RemoveComponent(entityID);
    }

    template <typename T>
    bool HasComponent(int entityID)
    {
        ComponentPool<T>* pool = GetOrCreatePool<T>();
        return pool->GetComponent(entityID) != nullptr;
    }

    template <typename T>
    int GetEntityIDByComponent(const T* component)
    {
        ComponentPool<T>* pool = GetOrCreatePool<T>();

        for (int entityID = 0; entityID < pool->components.size(); ++entityID)
        {
            if (pool->components[entityID].get() == component)
            {
                return entityID;
            }
        }
        return -1;
    }



    void RemoveAllComponents(int entityID)
    {
        int totalRemoved = 0;

        for (const auto& [type, poolPtr] : componentPools)
        {
            auto* pool = static_cast<ComponentPoolBase*>(poolPtr.get());
            pool->RemoveComponent(entityID);
            totalRemoved++;
        }

        std::cout << "Removed " << totalRemoved << " components for entity " << entityID << std::endl;
    }



};
