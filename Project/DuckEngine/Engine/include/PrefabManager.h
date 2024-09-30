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

class DUCKENGINE_API PrefabManager
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Prefab>> prefabs;

public:

	// Add a single prefab to the manager
	static void AddPrefab(const std::string& name, const std::shared_ptr<Prefab>& prefab);

	// retrieve prefab by name
	static std::shared_ptr<Prefab> GetPrefab(const std::string& name);

	// instantiate prefab by name
	static Entity* InstantiatePrefab(const std::string& name, Vec2 newPosition);

	static void LoadPrefabsFromFile(const std::string& filePath);


};