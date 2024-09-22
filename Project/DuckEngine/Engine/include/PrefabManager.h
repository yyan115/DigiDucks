#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"

struct Prefab
{
	std::string name;
	std::string texturePath;
	Vec2 position;
	Vec2 scale;
	std::vector<Component*> components;

	Entity* Instantiate(Vec2 newPosition)
	{




	}


};