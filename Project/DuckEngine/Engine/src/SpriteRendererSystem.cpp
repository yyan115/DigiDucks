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
			DrawOptions drawOptions;
			drawOptions.translation = transform->position;
			drawOptions.scale = transform->scale;
			drawOptions.rotation = transform->angle;

			if (spriteRenderer->texture) {
				//std::cout << "use texture\n";
				drawOptions.useTexture = true;
				drawOptions.texture = &spriteRenderer->texture;
			}

			// PLS ADD THIS TO SOMEWHERE, PROBABLY SPRITE OR TRANSFORM, THEN CHECK IF TRUE/FALSE
			drawOptions.relativeToCamera = true;

			// USE THIS TO USE AND SET COLORS
			// FKING WEIRD PLS FIX, SOMETIMES GET RANDOM BUG WHERE OVER 1K ERRORS, AND WHEN I CALL USECOLOR IN GAME THIS DOESNT WORK IDK WHY
			if (spriteRenderer->useColor) {
				//std::cout << "use color\n";
				drawOptions.useColor = true;
				drawOptions.color = spriteRenderer->color;
			}

			GraphicsManager::AddToDrawQueue(drawOptions);
		}
	}
}