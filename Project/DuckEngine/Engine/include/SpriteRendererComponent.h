/******************************************************************************/
/*!
\file       SpriteRendererComponent.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the SpriteRendererComponent class, which represents the
			component responsible for rendering sprites in the game engine.
			It includes properties such as texture, layer, and color.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

/************************************************************************
@brief The SpriteRendererComponent class is responsible for holding sprite
	   rendering information. It includes attributes such as texture, layer,
	   and color, and it inherits from the Component base class.
*************************************************************************/
class SpriteRendererComponent : public Component
{
public:
	bool sprite;
	int sortingOrder;
	Texture texture;
	std::string texturePath;
	bool useColor;
	Color color;
	bool isVisible;

	/************************************************************************
	@brief Constructor for the SpriteRendererComponent class. Initializes
		   the component with a sprite flag, layer, optional color usage,
		   and color value.
	@param hasSprite Boolean flag indicating if the entity has a sprite.
	@param spriteLayer The rendering layer for the sprite (default is 0).
	@param useColor Boolean flag indicating if the sprite uses a solid color
					instead of a texture (default is false).
	@param color The color to apply if useColor is true (default is an empty color).
	*************************************************************************/
	DUCKENGINE_API SpriteRendererComponent(bool hasSprite = false, int spriteLayer = 0, bool useColor = false, Color color = Color(), bool visible = true) : sprite(hasSprite), sortingOrder(spriteLayer), texture(), useColor(useColor), color(color), isVisible(visible) {}

	/************************************************************************
	@brief Clones the current SpriteRendererComponent instance. This method
		   is used to create a deep copy of the component.
	@return A shared pointer to the newly cloned SpriteRendererComponent.
	*************************************************************************/
	DUCKENGINE_API std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<SpriteRendererComponent>(*this);
	}

	/************************************************************************
	@brief Retrieves the file path of the current texture.
	@return A string containing the texture path.
	*************************************************************************/
	DUCKENGINE_API std::string GetFilePath() const
	{
		return texturePath;
	}

};