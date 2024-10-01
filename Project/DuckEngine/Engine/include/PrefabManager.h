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

class PrefabManager
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Prefab>> prefabs;

public:

	// Add a single prefab to the manager
	static DUCKENGINE_API void AddPrefab(const std::string& name, const std::shared_ptr<Prefab>& prefab);

	// retrieve prefab by name
	static DUCKENGINE_API std::shared_ptr<Prefab> GetPrefab(const std::string& name);

	// instantiate prefab by name
	static DUCKENGINE_API Entity* InstantiatePrefab(const std::string& name, Vec2 newPosition);

	static DUCKENGINE_API void LoadPrefabsFromFile(const std::string& filePath);


};