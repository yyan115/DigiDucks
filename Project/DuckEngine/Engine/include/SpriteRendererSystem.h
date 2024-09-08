#pragma once

#include "System.h"
#include "DuckEngine.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API SpriteRendererSystem : public System
{
public:
	void Update() override
	{
		for (const auto& [entityId, spriteRenderer] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SpriteRendererComponent>())
		{
			SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);
			TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
			if (spriteRenderer->sprite && transform)
			{
				std::cout << "SpriteRenderer: " << transform->x << " " << transform->y << " \n";
			}
		}
	}
};