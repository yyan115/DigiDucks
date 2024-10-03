/******************************************************************************/
/*!
\file       PrefabManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the PrefabManager class, providing functionality to
			manage prefabs by adding, retrieving, and instantiating them,
			as well as loading prefabs from a file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PrefabManager.h"
#include "DuckEngine.h"
#include "ComponentFactory.h"

std::unordered_map<std::string, std::shared_ptr<Prefab>> PrefabManager::prefabs;

/************************************************************************
@brief Adds a prefab to the PrefabManager's collection.
@param name The name of the prefab to be added.
@param prefab A shared pointer to the prefab object.
*************************************************************************/
void PrefabManager::AddPrefab(const std::string& name, const std::shared_ptr<Prefab>& prefab)
{
	prefabs[name] = prefab;
}

/************************************************************************
@brief Retrieves a prefab by name from the PrefabManager's collection.
@param name The name of the prefab to retrieve.
@return A shared pointer to the requested prefab, or nullptr if not found.
*************************************************************************/
std::shared_ptr<Prefab> PrefabManager::GetPrefab(const std::string& name)
{
	auto it = prefabs.find(name);

	if (it != prefabs.end())
	{
		return it->second;
	}

	std::cerr << "Error: Prefab " << name << " not found!" << std::endl;
	return nullptr;
}

/************************************************************************
@brief Instantiates a prefab by name at a given position.
@param name The name of the prefab to instantiate.
@param newPosition The position where the new entity will be created.
@return A pointer to the newly created entity, or nullptr if the prefab is not found.
*************************************************************************/
Entity* PrefabManager::InstantiatePrefab(const std::string& name, Vec2 newPosition)
{
	std::shared_ptr<Prefab> prefab = GetPrefab(name);

	if (prefab)
	{
		return prefab->Instantiate(newPosition);
	}

	std::cerr << "Error: Prefab " << name << " not found!" << std::endl;
	return nullptr;
}

/************************************************************************
@brief Loads prefabs from a JSON file and adds them to the PrefabManager's collection.
@param filePath The path to the JSON file containing prefab definitions.
*************************************************************************/
void PrefabManager::LoadPrefabsFromFile(const std::string& filePath)
{
	json prefabData = Serialization::LoadJsonFile(filePath);

	if (prefabData.contains("prefabs"))
	{
		auto prefabsJson = prefabData["prefabs"];

		for (auto& [prefabName, prefabInfo] : prefabsJson.items())
		{
			std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>(prefabName);

			if (prefabInfo.contains("components"))
			{
				prefab->componentsData = prefabInfo["components"];
			}

			AddPrefab(prefabName, prefab);
		}
	}
}
