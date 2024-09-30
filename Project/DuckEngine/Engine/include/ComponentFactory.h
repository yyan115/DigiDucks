#pragma once

#include <string>
#include "Serialization.h"
#include "DuckEngine.h" 

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API ComponentFactory
{
public:
    static void AddComponentsToEntity(Entity* entity, const nlohmann::json& componentsData);
};

