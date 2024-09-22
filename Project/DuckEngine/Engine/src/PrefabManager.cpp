#include "PrefabManager.h"

std::unordered_map<std::string, std::shared_ptr<Prefab>> PrefabManager::prefabs;

void PrefabManager::LoadPrefab(const std::string& name, const std::shared_ptr<Prefab>& prefab)
{
	prefabs[name] = prefab;
}

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
