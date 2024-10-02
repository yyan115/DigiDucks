#pragma once
#include "Component.h"
#include "string"
#include "Texture.h"
#include "Color.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class SpriteRendererComponent : public Component
{
public:
	bool sprite;
	int layer;
	Texture texture;
	bool useColor;
	Color color;
	DUCKENGINE_API SpriteRendererComponent(bool hasSprite, int spriteLayer = 0, bool useColor = false, Color color = Color()) : sprite(hasSprite), layer(spriteLayer), texture(), useColor(useColor), color(color) {}

	DUCKENGINE_API std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<SpriteRendererComponent>(*this);
	}


};