#pragma once
#include "Component.h"
#include "string"


// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

typedef unsigned int GLuint;

class DUCKENGINE_API SpriteRendererComponent : public Component
{
public:
	bool sprite;
	int layer;
	GLuint texture;
	SpriteRendererComponent(bool hasSprite, int spriteLayer = 0) : sprite(hasSprite), layer(spriteLayer) {}
};