#include "Prefab.h"
#include "DuckEngine.h"


void Prefab::AddComponent(const std::shared_ptr<Component>& component)
{

	components.push_back(component);
}

Entity* Prefab::Instantiate(Vec2 newPosition)
{
	// Create a new entity using the EntityFactory
	Entity* entity = EntityFactory::CreateEntity(texturePath, newPosition, scale);

	// Loop through each component and clone it
	for (const std::shared_ptr<Component>& component : components)
	{
		// Clone the component
		std::shared_ptr<Component> clonedComponent = component->Clone();

		// If it's a SpriteRendererComponent, make sure the texture is set properly
		if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(clonedComponent))
		{
			// Ensure the texture is loaded if not already
			if (spriteRenderer->texture)
			{
				spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(texturePath)[0];
				std::cout << "add" << std::endl;
			}
		}

		// Add the component to the entity
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent(entity->entityID, clonedComponent);
	}

	return entity;
}