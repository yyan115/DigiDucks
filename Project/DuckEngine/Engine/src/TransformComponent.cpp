#include "TransformComponent.h"

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