#pragma once

#include "DuckEngine.h"

class EntityFactoryTwo
{
public:
	static Entity* CreatePlayer(const std::string& texturePath,
		const Vec2& position, const Vec2& scale, float moveSpeed);


};