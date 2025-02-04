/******************************************************************************/
/*!
\file       PrefabManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the PrefabManager class, responsible for managing the
			storage and instantiation of prefabs in the game engine. It allows
			adding, retrieving, and instantiating prefabs by name, as well as
			loading prefabs from a file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Prefab.h"
#include "Vector2.h"   
#include "Entity.h"  


#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The PrefabManager class is responsible for managing a collection of
	   prefabs in the game engine. It provides methods for adding prefabs,
	   retrieving them by name, instantiating them, and loading them from
	   a file.
*************************************************************************/
class PrefabManager
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Prefab>> prefabs;

public:

	/************************************************************************
	@brief Adds a prefab to the manager's collection of prefabs.
	@param name The name of the prefab to add.
	@param prefab A shared pointer to the prefab object.
	*************************************************************************/
	static DUCKENGINE_API void AddPrefab(const std::string& name, const std::shared_ptr<Prefab>& prefab);

	/************************************************************************
	@brief Retrieves a prefab by its name.
	@param name The name of the prefab to retrieve.
	@return A shared pointer to the requested prefab, or nullptr if not found.
	*************************************************************************/
	static DUCKENGINE_API std::shared_ptr<Prefab> GetPrefab(const std::string& name);

	/************************************************************************
	@brief Instantiates a prefab by its name at a specified position.
	@param name The name of the prefab to instantiate.
	@param newPosition The position where the new entity will be created.
	@return A pointer to the newly created entity.
	*************************************************************************/
	static DUCKENGINE_API Entity* InstantiatePrefab(const std::string& name, Vec2 newPosition);

	/************************************************************************
	@brief Loads prefabs from a file into the prefab manager. Each prefab
		   defined in the file will be added to the manager's collection.
	@param filePath The path to the file containing prefab data.
	*************************************************************************/
	static DUCKENGINE_API void LoadPrefabsFromDirectory(const std::string& directoryPath);

	static DUCKENGINE_API void SavePrefab(const std::string& name);

	static const DUCKENGINE_API std::unordered_map<std::string, std::shared_ptr<Prefab>>& GetAllPrefabs();

	static int DUCKENGINE_API GenerateTemporaryEntityFromPrefab(const std::string& prefabName);

	static void DUCKENGINE_API SyncPrefabInstances(const std::shared_ptr<Prefab>& prefab);

	static DUCKENGINE_API std::shared_ptr<Prefab> GetPrefabFromEntity(const Entity* entity);
	
	static bool DUCKENGINE_API RemovePrefab(const std::string& name);
};