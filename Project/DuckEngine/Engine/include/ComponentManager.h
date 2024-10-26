/******************************************************************************/
/*!
\file       ComponentManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the ComponentManager class responsible for managing
            components in the game engine, providing functionality for
            adding, retrieving, and removing components associated with
            entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "Bounding.h"
#include "Serialization.h"
#include "SoundComponent.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The ComponentManager class is responsible for managing the storage
       and lifecycle of components in the game engine. It supports adding,
       retrieving, and removing components associated with entities.
*************************************************************************/
class ComponentManager
{
  private:
      std::unordered_map<std::type_index,
                       std::unordered_map<int, std::shared_ptr<Component>>>
        componentStorage;

      bool enableLogging = false;

  public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    /************************************************************************
    @brief Adds a component of a specific type to an entity.
    @tparam T The component type to add.
    @param entityID The ID of the entity to associate with the component.
    @param args Arguments required to construct the component.
    @return A pointer to the added component.
    *************************************************************************/
    template <typename T, typename... Args>
    T* AddComponent(int entityID, Args &&...args)
    {
        auto& typeMap = componentStorage[typeid(T)];
        std::shared_ptr<Component> component = std::make_shared<T>(std::forward<Args>(args)...);
        auto result = typeMap.emplace(entityID, component);

        if (enableLogging) {
            std::cout << "Added component of type " << typeid(T).name()
                << " to entity " << entityID
                << " (Insertion " << (result.second ? "successful" : "failed") << ")"
                << std::endl;
        }

        return std::static_pointer_cast<T>(result.first->second).get();
    }

    /************************************************************************
    @brief Adds a pre-constructed component to an entity.
    @param entityID The ID of the entity.
    @param component A shared pointer to the component to be added.
    *************************************************************************/
    DUCKENGINE_API void AddComponent(int entityID, const std::shared_ptr<Component>& component)
    {
        auto& typeMap = componentStorage[typeid(*component)];
        typeMap[entityID] = component;
    }

    /************************************************************************
    @brief Retrieves a component of a specific type associated with an entity.
    @tparam T The type of the component to retrieve.
    @param entityID The ID of the entity.
    @return A pointer to the component, or nullptr if not found.
    *************************************************************************/
    template <typename T> 
    T *GetComponent(int entityID)
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

    /************************************************************************
    @brief Retrieves all components of a specific type.
    @tparam T The type of components to retrieve.
    @return A reference to a map of entityID to component pointers.
    *************************************************************************/
    template <typename T>
    std::unordered_map<int, std::shared_ptr<Component>>& GetComponents()
    {
        return componentStorage[typeid(T)];
    }


    /************************************************************************
    @brief Removes a component of a specific type from an entity.
    @tparam T The type of component to remove.
    @param entityID The ID of the entity.
    *************************************************************************/
    template <typename T> 
    void RemoveComponent(int entityID)
    {
        auto typeIt = componentStorage.find(typeid(T));
        if (typeIt != componentStorage.end())
        {
            typeIt->second.erase(entityID);
        }
    }

    /************************************************************************
    @brief Checks if an entity has a component of a specific type.
    @tparam T The type of the component to check for.
    @param entityID The ID of the entity.
    @return True if the component exists, otherwise false.
    *************************************************************************/
    template <typename T> 
    bool HasComponent(int entityID)
    {
        auto typeIt = componentStorage.find(typeid(T));
        if (typeIt != componentStorage.end())
        {
            return typeIt->second.find(entityID) != typeIt->second.end();
        }
        return false;
    }

    /************************************************************************
    @brief Removes all components associated with an entity.
    @param entityID The ID of the entity.
    *************************************************************************/
    void DUCKENGINE_API RemoveAllComponents(int entityID)
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

        if (enableLogging) {
            std::cout << "Removed " << totalRemoved << " components for entity " << entityID << std::endl;
            std::cout << "Remaining components for entity " << entityID << ": " << totalRemaining << std::endl;
        }
    }

    /************************************************************************
    @brief Enables or disables logging of component operations.
    @param enable A boolean indicating whether logging should be enabled.
    *************************************************************************/
    void EnableLogging(bool enable) {
        enableLogging = enable;
    }
};
