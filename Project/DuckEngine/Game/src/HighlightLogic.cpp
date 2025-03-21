/******************************************************************************/
/*!
\file       HighlightLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernest@digipen.edu
\date       Mar 21 2025
\brief      Declaration of Highlight Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "HighlightLogic.h"

void HighlightLogic::Start()
{
	entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (entity)
	{
		sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entity->entityID);
		if(sprite)
		{
			ogSortingOrder = sprite->sortingOrder;
		}
		transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);

		if (entity->childEntities.size() > 0)
		{
			highlightEntity = entity->childEntities[0].get();
			if (highlightEntity)
			{
				highlightSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(highlightEntity->entityID);
				if (highlightSprite)
				{
					// Highlight sprite is always behind the highlightED sprite
					if(sprite)
					{
						highlightSprite->sortingOrder = sprite->sortingOrder + additionalSortingOrder -1;
					}
					highlightSprite->isVisible = false;
				}
				highlightTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(highlightEntity->entityID);
				if (highlightTransform && transform)
				{
					highlightTransform->SetPosition(transform->GetPosition());
				}
			}
		}
	}
}

void HighlightLogic::Update()
{

}

void HighlightLogic::FixedUpdate()
{
	if (isHighlighted)
	{
		if(highlightSprite)
			highlightSprite->isVisible = true;
		if(sprite)
			sprite->sortingOrder = ogSortingOrder + additionalSortingOrder;
	}
	else if (sprite->sortingOrder != ogSortingOrder)
	{
		if(highlightSprite)
			highlightSprite->isVisible = false;
		if (sprite)
			sprite->sortingOrder = ogSortingOrder;
	}
	isHighlighted = false;
}