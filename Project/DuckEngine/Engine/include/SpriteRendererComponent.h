#pragma once
#include "Component.h"
#include "string"
#include "Sprite.h"


// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API SpriteRendererComponent : public Component
{
public:
	bool sprite;
	int layer;
	Sprite texture;
	SpriteRendererComponent(bool hasSprite, int spriteLayer = 0) : sprite(hasSprite), layer(spriteLayer), texture() {}
};