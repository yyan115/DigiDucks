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
	//glm::vec2 position;
 //   float rotation;
	//glm::vec2 scale;

	//TransformComponent(glm::vec2 pos, float rot, glm::vec2 scl)
 //           : position(pos), rotation(rot), scale(scl) {}

	Vec2 scale;
	float angle;
	Vec2 position;

    //TransformComponent(float xPos, float yPos) : x(xPos), y(yPos){};

	TransformComponent(float scaleX, float scaleY, float angle, float posX, float posY) : scale(scaleX, scaleY), angle(angle), position(posX, posY) {};
	TransformComponent(Vec2& scale, float angle, Vec2& position) : scale(scale), angle(angle), position(position) {};
	TransformComponent(Vec2&& scale, float angle, Vec2&& position) : scale(scale), angle(angle), position(position) {};


};