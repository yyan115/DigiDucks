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

#include "DuckEngine.h"
#include "PrefabManager.h"
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

int PrefabManager::GenerateTemporaryEntityFromPrefab(const std::string& prefabName)
{
	std::shared_ptr<Prefab> prefab = GetPrefab(prefabName);
	if (!prefab)
	{
		std::cerr << "Error: Prefab not found: " << prefabName << std::endl;
		return -1;
	}

	auto entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
	if (!entity)
	{
		std::cerr << "Error: Failed to create temporary entity for prefab: " << prefabName << std::endl;
		return -1;
	}

	entity->name = prefabName + "_Temp";
	ComponentFactory::AddComponentsToEntity(entity.get(), prefab->componentsData);

	return entity->entityID;
}

void PrefabManager::SyncPrefabInstances(const std::shared_ptr<Prefab>& prefab)
{
	if (!prefab)
		return;

	for (auto& entity : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
	{
		if (entity.get()->prefabName == prefab->name)
		{
			auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entity.get()->entityID);
			if (animator)
			{
				animator->animations.clear();
				for (const auto& [name, animation] : prefab->componentsData["AnimatorComponent"]["animations"].items())
				{
					Animation anim(animation["frameDuration"]);
					anim.name = name;
					animator->animations[name] = anim;
				}
			}
		}
	}
}

std::shared_ptr<Prefab> PrefabManager::GetPrefabFromEntity(const Entity* entity)
{
	if (!entity)
	{
		std::cerr << "Error: Entity is null." << std::endl;
		return nullptr;
	}

	const std::string& prefabName = entity->prefabName;
	auto it = prefabs.find(prefabName);
	if (it != prefabs.end())
	{
		return it->second;
	}

	std::cerr << "Error: Prefab not found for entity with prefabName: " << prefabName << std::endl;
	return nullptr;
}

bool PrefabManager::RemovePrefab(const std::string& name)
{
	if (name.empty()) {
		std::cerr << "[ERROR] Attempted to delete a prefab with an empty name." << std::endl;
		return false;
	}

	auto it = prefabs.find(name);
	if (it == prefabs.end()) {
		std::cerr << "[ERROR] Prefab '" << name << "' not found in memory!" << std::endl;
		return false;
	}

	// Clear the prefab before erasing to prevent dangling pointers**
	it->second.reset(); // Remove shared pointer reference
	prefabs.erase(it);

	std::string filePath = "Resources/Prefabs/" + name + ".json";
	if (!std::filesystem::exists(filePath)) {
		std::cerr << "[ERROR] Prefab file not found on disk: " << filePath << std::endl;
		return false;
	}

	try {
		std::filesystem::remove(filePath);
		std::cout << "[INFO] Prefab deleted successfully: " << filePath << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "[ERROR] Exception while deleting prefab file: " << e.what() << std::endl;
		return false;
	}
}