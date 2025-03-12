/******************************************************************************/
/*!
\file       GameLogicManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Declares the GameLogicManager class, which manages global and
            entity-specific game logic objects. Provides functionality to
            add, retrieve, and remove game logic, as well as utilities for
            handling logic across all entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "GameLogic.h"
#include "GameLogicComponent.h"
#include <iostream>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class GameLogicManager
{
private:
    static DUCKENGINE_API std::unordered_map<std::string, std::shared_ptr<GameLogic>> logicMap;
    static DUCKENGINE_API std::unordered_map<int, std::vector<std::shared_ptr<GameLogic>>> entityLogicMap;

public:
    /**************************************************************************
    * @brief Adds a logic object to the global logic map.
    * @param name The name of the logic to add.
    * @param logic A shared pointer to the GameLogic instance.
    **************************************************************************/
    static DUCKENGINE_API void AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic);

    /**************************************************************************
    * @brief Associates a logic object with a specific entity.
    * @param entityID The ID of the entity.
    * @param logic A shared pointer to the GameLogic instance.
    **************************************************************************/
    static DUCKENGINE_API void AddLogicToEntity(int entityID, std::shared_ptr<GameLogic> logic);

    /**************************************************************************
    * @brief Retrieves a logic object by its name.
    * @param name The name of the logic to retrieve.
    * @return A shared pointer to the GameLogic instance, or nullptr if not found.
    **************************************************************************/
    static DUCKENGINE_API std::shared_ptr<GameLogic> GetLogic(const std::string& name);

    /**************************************************************************
    * @brief Retrieves all logic objects associated with a specific entity.
    * @param entityID The ID of the entity.
    * @return A vector of shared pointers to the GameLogic instances.
    **************************************************************************/
    static DUCKENGINE_API std::vector<std::shared_ptr<GameLogic>> GetAllLogicsForEntity(int entityID);

    /**************************************************************************
    * @brief Retrieves a specific type of logic object associated with an entity.
    * @tparam T The type of logic to retrieve.
    * @param entityID The ID of the entity.
    * @return A shared pointer to the logic object, or nullptr if not found.
    **************************************************************************/
    template <typename T>
    static std::shared_ptr<T> GetLogicForEntity(int entityID)
    {
        //std::cout << "Looking For GameLogic with Component ID: " << entityID << std::endl;

        static_assert(std::is_base_of<GameLogic, T>::value, "T must inherit from GameLogic");

        if (entityLogicMap.find(entityID) != entityLogicMap.end())
        {
            for (const auto& logic : entityLogicMap[entityID])
            {
                auto typedLogic = std::dynamic_pointer_cast<T>(logic);
                if (typedLogic && typedLogic->GetComponentID() == entityID)
                {
                    //std::cout << "Found GameLogic with Component ID: " << typedLogic->GetComponentID() << std::endl;
                    return typedLogic;
                }
            }
        }

        //std::cout << "No matching GameLogic found for Component ID: " << entityID << std::endl;
        return nullptr;
    }

    /**************************************************************************
    * @brief Retrieves all entities that have a specific type of logic.
    * @tparam T The type of logic to search for.
    * @return A vector of entity IDs that have the specified logic type.
    **************************************************************************/
    template <typename T>
    static std::vector<int> GetAllEntitiesWithLogic()
    {
        static_assert(std::is_base_of<GameLogic, T>::value, "T must inherit from GameLogic");

        std::vector<int> entitiesWithLogic;

        for (const auto& [entityID, logics] : entityLogicMap)
        {
            for (const auto& logic : logics)
            {
                auto typedLogic = std::dynamic_pointer_cast<T>(logic);
                if (typedLogic)
                {
                    entitiesWithLogic.push_back(entityID);
                    break; // Found the logic for this entity, move to the next entity
                }
            }
        }

        return entitiesWithLogic;
    }

	template <typename T>
	static std::shared_ptr<T> GetSingleLogic()
	{
		static_assert(std::is_base_of<GameLogic, T>::value,
			"T must inherit from GameLogic.");

		std::shared_ptr<T> singleInstance = nullptr;

		for (auto& [name, logic] : logicMap)
		{
			std::shared_ptr<T> castedLogic = std::dynamic_pointer_cast<T>(logic);

			if (castedLogic)
			{
				if (singleInstance)
				{
					throw std::runtime_error(
						"More than one instance of the requested logic type found."
					);
				}

				singleInstance = castedLogic;
			}
		}

		return singleInstance;
	}


    /**************************************************************************
    * @brief Removes all logic objects associated with a specific entity.
    * @param entityID The ID of the entity.
    **************************************************************************/
    static DUCKENGINE_API void RemoveLogicsForEntity(int entityID);

    static std::vector<std::shared_ptr<GameLogic>>& GetLogicsForEntity(int entityID);

    static void StartAll();
    static void UpdateAll();
    static void Clear();


};
