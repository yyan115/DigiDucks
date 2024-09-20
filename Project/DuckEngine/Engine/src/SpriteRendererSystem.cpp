#include "SpriteRendererSystem.h"
#include "GraphicsManager.h"
#include "Color.h"

void SpriteRendererSystem::Update()
{
	for (const auto& [entityId, spriteRenderer] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SpriteRendererComponent>())
	{
		SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);
		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (spriteRenderer->sprite && transform)
		{
			//std::cout << "SpriteRenderer: " << transform->x << " " << transform->y << " \n";
			DrawOptions whatToDraw;
			whatToDraw.translation = transform->position;
			whatToDraw.scale = transform->scale;
			whatToDraw.rotation = transform->angle;

			if (spriteRenderer->texture) {
				//std::cout << "use texture\n";
				whatToDraw.useTexture = true;
				whatToDraw.texture = &spriteRenderer->texture;
			}

			// PLS ADD THIS TO SOMEWHERE, PROBABLY SPRITE OR TRANSFORM, THEN CHECK IF TRUE/FALSE
			whatToDraw.relativeToCamera = true;

			// USE THIS TO USE AND SET COLORS
			// FKING WEIRD PLS FIX, SOMETIMES GET RANDOM BUG WHERE OVER 1K ERRORS, AND WHEN I CALL USECOLOR IN GAME THIS DOESNT WORK IDK WHY
			if (spriteRenderer->useColor) {
				//std::cout << "use color\n";
				whatToDraw.useColor = true;
				whatToDraw.color = spriteRenderer->color;
			}

			GraphicsManager::AddToDrawQueue(whatToDraw);
		}
	}
}