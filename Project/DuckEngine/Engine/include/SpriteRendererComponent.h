#pragma once
#include "Component.h"

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
	SpriteRendererComponent(bool hasSprite, int spriteLayer = 0) : sprite(hasSprite), layer(spriteLayer) {}
};