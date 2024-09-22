#pragma once

#include "Component.h"
#include "Vector2.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API RigidbodyComponent : public Component
{
public:
	Vec2 velocity; // Velocity of the entity
	Vec2 acceleration; // Acceleration of the entity
	float mass; // Mass of the entity
	bool isStatic; // is the entity static

	RigidbodyComponent() : velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), mass(1.0f), isStatic(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<RigidbodyComponent>(*this);
	}
};