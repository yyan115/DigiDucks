#include "TransformComponent.h"
#include "DuckEngine.h"

void TransformComponent::SetPosition(const Vec2& newPos)
{
	if (position != newPos)
	{
		Vec2 delta = newPos - position;

		previousPosition = newPos;
		position = newPos;

		UpdateChildPositions(delta);
	}
}

Vec2& TransformComponent::GetPosition()
{
	return position;
}

void TransformComponent::UpdateChildPositions(const Vec2& delta)
{
	auto parentEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetEntityID());
	if (!parentEntity)
	{
		std::cerr << "Warning: Parent entity not found for entity ID " << GetEntityID() << std::endl;
		return;
	}

	if (!parentEntity->childEntities.empty())
	{
		for (const auto& childEntity : parentEntity->childEntities)
		{
			if (childEntity)
			{
				auto childTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(childEntity->entityID);
				if (childTransform)
				{
					childTransform->SetPosition(childTransform->GetPosition() + delta);
				}
			}
		}
	}
}