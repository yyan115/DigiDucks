#include "TransformComponent.h"
#include "DuckEngine.h"

#include "TransformComponent.h"
#include "DuckEngine.h"

void TransformComponent::SetPosition(const Vec2& newPos)
{
	if (worldPosition != newPos)
	{
		// Store the old position to calculate delta
		Vec2 oldPosition = worldPosition;
		worldPosition = newPos;
		previousPosition = newPos; // Update previousPosition after calculating delta

		Vec2 delta = newPos - oldPosition;

		Entity* parentEntity = nullptr;
		for (const auto& potentialParent : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
		{
			auto it = std::find_if(potentialParent->childEntities.begin(), potentialParent->childEntities.end(),
				[this](const std::shared_ptr<Entity>& child) { return child->entityID == GetEntityID(); });

			if (it != potentialParent->childEntities.end())
			{
				parentEntity = potentialParent.get();
				break;
			}
		}

		if (parentEntity)
		{
			auto parentTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(parentEntity->entityID);
			if (parentTransform)
			{
				localPosition = worldPosition - parentTransform->worldPosition;
			}
		}
		else
		{
			localPosition = worldPosition;
		}

		UpdateChildPositions(delta);
	}
}

Vec2& TransformComponent::GetPosition()
{
	return worldPosition;
}

Vec2& TransformComponent::GetLocalPosition()
{
	return localPosition;
}

void TransformComponent::UpdateChildPositions(const Vec2& delta)
{
	auto entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetEntityID());
	if (!entity)
	{
		std::cerr << "Warning: Entity not found for entity ID " << GetEntityID() << std::endl;
		return;
	}

	if (!entity->childEntities.empty())
	{
		for (const auto& childEntity : entity->childEntities)
		{
			if (childEntity)
			{
				auto childTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(childEntity->entityID);
				if (childTransform)
				{
					childTransform->worldPosition += delta;

					childTransform->localPosition = childTransform->worldPosition - worldPosition;

					childTransform->UpdateChildPositions(delta);
				}
			}
		}
	}
}
