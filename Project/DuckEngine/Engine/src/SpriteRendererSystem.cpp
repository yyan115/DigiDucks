#include "SpriteRendererSystem.h"
#include "GraphicsManager.h"

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

			whatToDraw.useTexture = true;
			whatToDraw.texture = &spriteRenderer->texture;

			GraphicsManager::AddToDrawQueue(whatToDraw);
		}
	}
}