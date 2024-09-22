#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"

class Prefab
{
public:
	std::string name;
	std::string texturePath;
	Vec2 position;
	Vec2 scale;
	std::vector<std::shared_ptr<Component>> components;

	Prefab(const std::string& name, const std::string& texturePath, Vec2 scale)
		: name(name), texturePath(texturePath), scale(scale) {}

	Entity* Instantiate(Vec2 newPosition);
};