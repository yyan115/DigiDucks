#pragma once
#include "Component.h"

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
    float x, y;
    TransformComponent(float xPos, float yPos) : x(xPos), y(yPos){};
};