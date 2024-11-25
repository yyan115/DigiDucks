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
#include <filesystem>

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

	return nullptr;
}

const std::unordered_map<std::string, std::shared_ptr<Prefab>>& PrefabManager::GetAllPrefabs() {
	return prefabs;
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

	return nullptr;
}

/************************************************************************
@brief Loads prefabs from a JSON file and adds them to the PrefabManager's collection.
@param filePath The path to the JSON file containing prefab definitions.
*************************************************************************/
void PrefabManager::LoadPrefabsFromDirectory(const std::string& directoryPath)
{
	std::filesystem::path absolutePath = std::filesystem::absolute(directoryPath);
	std::cout << "Loading prefabs from directory: " << absolutePath << std::endl;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			std::string prefabName = entry.path().stem().string();
			std::string filePath = entry.path().string();

			//std::cout << "Found prefab file: " << filePath << std::endl;

			nlohmann::json prefabData = Serialization::LoadJsonFile(filePath.c_str());
			if (prefabData.is_null())
			{
				std::cerr << "Error: Failed to parse JSON file: " << filePath << std::endl;
				continue;
			}

			if (prefabData.contains("components"))
			{
				std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>(prefabName);
				prefab->componentsData = prefabData["components"];

				for (const auto& componentJson : prefab->componentsData)
				{
					std::shared_ptr<Component> component = ComponentFactory::CreateComponentFromJson(componentJson);
					if (component)
					{
						prefab->AddComponent(component);

						if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(component))
						{
							prefab->texturePath = spriteRenderer->texturePath;
						}
					}
				}

				AddPrefab(prefabName, prefab);
				std::cout << "Successfully loaded prefab: " << prefabName << std::endl;
			}
			else
			{
				std::cerr << "Error: Missing 'components' key in prefab JSON: " << prefabName << std::endl;
				continue;
			}
		}
	}

	std::cout << "Finished loading prefabs from directory: " << directoryPath << std::endl;

	std::cout << "Prefabs loaded into PrefabManager:" << std::endl;
	for (const auto& [name, prefab] : prefabs)
	{
		std::cout << "  - " << name << std::endl;
	}
}

void PrefabManager::SavePrefab(const std::string& name)
{
	auto prefab = GetPrefab(name);
	if (!prefab)
	{
		std::cerr << "Error: Prefab not found: " << name << std::endl;
		return;
	}

	nlohmann::json prefabData;
	prefabData["components"] = prefab->componentsData;

	std::string filePath = "Resources/Prefabs/" + name + ".json";
	try
	{
		Serialization::SaveJsonFile(filePath, prefabData);
		std::cout << "Prefab saved successfully: " << filePath << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error saving prefab: " << e.what() << std::endl;
	}
}
