/******************************************************************************/
/*!
\file       HighlightLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernest@digipen.edu
\date       Mar 21 2025
\brief      Declaration of Highlight Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

// When player's box collider collides with the object, the object will be highlighted
// This logic is added to any object that should be highlighted
class HighlightLogic : public GameLogic
{
private:
	// Entity and sprite of the object
	Entity* entity = nullptr;
	SpriteRendererComponent* sprite = nullptr;
	TransformComponent* transform = nullptr;
	int ogSortingOrder = 0;

	// Highlight object
	Entity* highlightEntity = nullptr;
	SpriteRendererComponent* highlightSprite = nullptr;
	TransformComponent* highlightTransform = nullptr;
	int ogHighlightSortingOrder = 0;

	const int additionalSortingOrder = 3;
	
public:
	bool isHighlighted = false;

	HighlightLogic() : GameLogic(nullptr) {}

	HighlightLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<HighlightLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;

	void Update() override;

	void FixedUpdate() override;
};