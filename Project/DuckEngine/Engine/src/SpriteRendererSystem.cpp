#include "SpriteRendererSystem.h"
#include "GraphicsManager.h"
#include "Color.h"
#include "algorithm"

struct RenderData
{
	TransformComponent* transform;
	SpriteRendererComponent* spriteRenderer;
	int layer;
};

void SpriteRendererSystem::Start()
{

}

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
		}
	}
}