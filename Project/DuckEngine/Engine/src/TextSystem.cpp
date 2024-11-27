/******************************************************************************/
/*!
\file       TextSystem.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Implements the TextSystem class, responsible for rendering text
			components and managing text properties in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "TextSystem.h"
#include "TextComponent.h"
#include "DuckEngine_Input.h"

void TextSystem::Start()
{

}

void TextSystem::Update()
{

}

void TextSystem::Render()
{
	//std::cout << DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextComponent>().size();
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextComponent>())
	{
		TextComponent* text = static_cast<TextComponent*>(component.get());

		// Skip if disabled
		if (!text->isEnabled)
		{
			continue;
		}

		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!transform) continue;

		// Retrieve position and scale from TransformComponent
		Vector2D position = transform->GetPosition();
		Vector2D scale = transform->scale;

		DuckEngine::RenderText(text->fontName, text->text, position, static_cast<float>(text->fontSize), text->color);
	}
}