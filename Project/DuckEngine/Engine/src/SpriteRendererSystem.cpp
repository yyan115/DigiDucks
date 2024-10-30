/******************************************************************************/
/*!
\file       SpriteRendererSystem.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the SpriteRendererSystem class, which manages the
			rendering of sprites in the game engine. This system organizes
			sprites into layers and adds them to the graphics draw queue.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "SpriteRendererSystem.h"
#include "GraphicsManager.h"
#include "Color.h"
#include "algorithm"

namespace {
	void DrawDebug(TransformComponent* transform, SpriteRendererComponent* spriteRenderer) {
		// Calculate the rectangle bounds
		Vector2D halfSize = transform->scale * 0.5f; // Using scale to determine half dimensions
		Vector2D min = transform->position - halfSize; // Bottom-left corner
		Vector2D max = transform->position + halfSize; // Top-right corner

		// Optionally apply rotation to corners if needed
		// This step can vary based on how you want to handle rotation
		// For a simple case, you can just draw a rectangle without rotation
		DuckEngine::DrawRectangle(min, max, transform->angle, spriteRenderer->color); // Adjust the color as needed
	}
}

struct RenderData
{
	TransformComponent* transform;
	SpriteRendererComponent* spriteRenderer;
	int layer;
};

void SpriteRendererSystem::Start()
{

}

/************************************************************************
@brief Updates the SpriteRendererSystem by gathering all entities with
	   SpriteRendererComponent and TransformComponent, sorting them by layer,
	   and adding them to the graphics draw queue for rendering.
*************************************************************************/
void SpriteRendererSystem::Update()
{
	std::vector<RenderData> renderQueue;

	// add to render queue
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SpriteRendererComponent>())
	{
		SpriteRendererComponent* spriteRenderer = static_cast<SpriteRendererComponent*>(component.get());
		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);

		if (spriteRenderer && transform)
		{
			RenderData data;
			data.transform = transform;
			data.spriteRenderer = spriteRenderer;
			data.layer = spriteRenderer->layer;
			renderQueue.push_back(data);
		}
	}

	// sort according to layer
	std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderData& a, const RenderData& b) {
		return a.layer < b.layer;
		});

	for (const RenderData& data : renderQueue)
	{
		SpriteRendererComponent* spriteRenderer = data.spriteRenderer;
		TransformComponent* transform = data.transform;
		if (spriteRenderer->sprite && transform)
		{
			//std::cout << "SpriteRenderer: " << transform->x << " " << transform->y << " \n";
			DrawOptions drawOptions;
			drawOptions.translation = transform->position;
			drawOptions.scale = transform->scale;
			drawOptions.rotation = transform->angle;

			if (spriteRenderer->texture) 
			{
				drawOptions.useTexture = true;
				drawOptions.texture = &spriteRenderer->texture;
			}
			else if (spriteRenderer->useColor)
			{
				//std::cout << "use color\n";
				drawOptions.useColor = true;
				drawOptions.color = spriteRenderer->color;
			}
			else {
				drawOptions.useColor = true;
				drawOptions.color = {255.f, 0.f, 255.f, 255.f};
			}

			drawOptions.relativeToCamera = transform->relativeToCamera;

			GraphicsManager::AddToDrawQueue(drawOptions);
			DrawDebug(transform, spriteRenderer);
		}
		else if (spriteRenderer && transform) {
			//std::cout << "SpriteRenderer: " << transform->x << " " << transform->y << " \n";
			DrawOptions drawOptions;
			drawOptions.translation = transform->position;
			drawOptions.scale = transform->scale;
			drawOptions.rotation = transform->angle;

			drawOptions.useColor = true;
			drawOptions.color = { 255.f, 0.f, 255.f, 255.f };

			drawOptions.relativeToCamera = transform->relativeToCamera;

			GraphicsManager::AddToDrawQueue(drawOptions);
			DrawDebug(transform, spriteRenderer);
		}
	}
}