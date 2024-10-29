#include "ButtonSystem.h"
#include "ButtonComponent.h"
#include "DuckEngine_Input.h"

// Utility function to check if a point is within button bounds
bool IsPointInside(const Vector2D& point, const Vector2D& min, const Vector2D& max)
{
	return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

void ButtonSystem::Start()
{

}

void ButtonSystem::Update()
{
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
	{
		ButtonComponent* button = static_cast<ButtonComponent*>(component.get());

		// Skip if disabled
		if (!button->isEnabled)
		{
			continue;
		}

		// Check if button is pressed
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {

			if (IsPointInside({ static_cast<float>(DuckEngine_Input::GetMouseX()), static_cast<float>(DuckEngine_Input::GetMouseY()) }, button->min, button->max)) {

				if (button->onClick) {

					button->onClick();
				}
			}
		}
		
		// on hover MIA for now
	}
}