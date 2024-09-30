#include "PrefabManager.h"
#include "DuckEngine.h"
#include "ComponentFactory.h"

struct PrefabManager::Impl
{
	std::unordered_map<std::string, std::shared_ptr<Prefab>> prefabs;

	~Impl()
	{
		prefabs.clear();
	}
};

PrefabManager::Impl* PrefabManager::impl = nullptr;

PrefabManager::PrefabManager()
{
	impl = new Impl();
}

void PrefabManager::AddPrefab(const char* name, const std::shared_ptr<Prefab>& prefab)
{
	impl->prefabs[std::string(name)] = prefab;
}

std::shared_ptr<Prefab> PrefabManager::GetPrefab(const char* name)
{
	auto it = impl->prefabs.find(std::string(name));

	if (it != impl->prefabs.end())
	{
		return it->second;
	}

	std::cerr << "Error: Prefab " << name << " not found!" << std::endl;
	return nullptr;
}

Entity* PrefabManager::InstantiatePrefab(const char* name, Vec2 newPosition)
{
	std::shared_ptr<Prefab> prefab = GetPrefab(name);

	if (prefab)
	{
		return prefab->Instantiate(newPosition);
	}

	std::cerr << "Error: Prefab " << name << " not found!" << std::endl;
	return nullptr;
}

void PrefabManager::LoadPrefabsFromFile(const char* filePath)
{
	json prefabData = Serialization::LoadJsonFile(filePath);

	if (prefabData.contains("prefabs"))
	{
		auto prefabsJson = prefabData["prefabs"];

		for (auto& [prefabName, prefabInfo] : prefabsJson.items())
		{
			std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>(prefabName.c_str());

			if (prefabInfo.contains("components"))
			{
				prefab->componentsData = prefabInfo["components"];
			}

			AddPrefab(prefabName.c_str(), prefab);
		}
	}
}

void PrefabManager::Exit() 
{
	delete impl;
}