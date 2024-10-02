#include "Prefab.h"
#include "DuckEngine.h"


void Prefab::AddComponent(const std::shared_ptr<Component>& component)
{
	components.push_back(component);
}

Entity* Prefab::Instantiate(Vec2 newPosition)
{
	Entity* entity = EntityFactory::CreateEntity(texturePath, newPosition, scale);

	for (const std::shared_ptr<Component>& component : components)
	{
		std::shared_ptr<Component> clonedComponent = component->Clone();

		if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(clonedComponent))
		{
			if (spriteRenderer->texture)
			{
				spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(texturePath.c_str())[0];
				std::cout << "add" << std::endl;
			}
		}

		DuckEngine::DUCKENGINE_ComponentManager.AddComponent(entity->entityID, clonedComponent);
	}

	return entity;
}