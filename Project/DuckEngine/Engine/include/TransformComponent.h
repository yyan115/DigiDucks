#pragma once
#include "Component.h"
#include "Vector2.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API TransformComponent : public Component
{
  public:
	Vec2 position;
	float angle;
	Vec2 scale;

	TransformComponent() : scale(1.0f, 1.0f), angle(0.0f), position(0.0f, 0.0f) {}
	TransformComponent(float scaleX, float scaleY, float angle, float posX, float posY) : scale(scaleX, scaleY), angle(angle), position(posX, posY) {};
	TransformComponent(Vec2& scale, float angle, Vec2& position) : scale(scale), angle(angle), position(position) {};
	TransformComponent(Vec2&& scale, float angle, Vec2&& position) : scale(scale), angle(angle), position(position) {};


};