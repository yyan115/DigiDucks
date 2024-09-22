#pragma once

#include <vector>
#include "Entity.h"
#include "Vector2.h"
#include "Component.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API EntityFactory
{
public:
	static Entity* CreateEntity(const std::string& texturePath, Vec2 position, Vec2 scale);
	//static Entity* CreateEntityWithCustomComponents(const std::vector<Component*>& customComponents);
};