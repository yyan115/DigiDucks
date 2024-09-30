#pragma once

#include <string>
#include <vector>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"
#include "Serialization.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API Prefab
{
public:
	std::string name;
	std::string texturePath;
	Vec2 position;
	Vec2 scale;
	std::vector<std::shared_ptr<Component>> components;
	nlohmann::json componentsData;

	Prefab(const std::string& name, const std::string& texturePath = "", Vec2 scale = {1.0f, 1.0f})
		: name(name), texturePath(texturePath), scale(scale) {}

	void AddComponent(const std::shared_ptr<Component>& component);
	Entity* Instantiate(Vec2 newPosition);
};