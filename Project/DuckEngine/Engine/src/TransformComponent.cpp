#include "TransformComponent.h"
#include "DuckEngine.h"

void TransformComponent::UpdateGlobalPosition()
{
	if (!owner)
	{
		owner = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetEntityID());
	}

	if (owner && owner->parent)
	{
		auto* parentTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->parent->entityID);
		if (parentTransform)
		{
			globalPosition = parentTransform->globalPosition + localPosition;
			scale.x = parentTransform->scale.x * scale.x;
			scale.y = parentTransform->scale.y * scale.y;
		}
	}
	else
	{
		globalPosition = localPosition;
	}
}

void TransformComponent::SetLocalPosition(const Vec2& newPos)
{
	localPosition = newPos;
	UpdateGlobalPosition();
}

void TransformComponent::SetPosition(const Vec2& newPos)
{
	if (position != newPos)
	{
		previousPosition = newPos;
		position = newPos;
	}
}

Vec2& TransformComponent::GetPosition()
{
	return position;
}