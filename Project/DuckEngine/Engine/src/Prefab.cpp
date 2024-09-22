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
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent(entity->EntityID, clonedComponent);
		
	}

	return entity;
}
