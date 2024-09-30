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

public:
    PrefabManager();

    // Add a single prefab to the manager
    static void AddPrefab(const char* name, const std::shared_ptr<Prefab>& prefab);

    // Retrieve prefab by name
    static std::shared_ptr<Prefab> GetPrefab(const char* name);

    // Instantiate prefab by name
    static Entity* InstantiatePrefab(const char* name, Vec2 newPosition);

    // Load prefabs from a file
    static void LoadPrefabsFromFile(const char* filePath);

    static void Exit();

private:
    struct Impl;  // Forward declare Impl
    static Impl* impl;  // Pointer to Impl
};