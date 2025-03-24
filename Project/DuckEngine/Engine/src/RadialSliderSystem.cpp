#include "RadialSliderSystem.h"
#include "GraphicsManager.h"
#include "DuckEngine_Input.h"
#include "WindowManager.h"
#include "ImageLoader.h"
#include <cmath>

void RadialSliderSystem::Start()
{
	// Initialization if needed
}

void RadialSliderSystem::Update()
{
	// Process all RadialSliderComponents
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<RadialSliderComponent>())
	{
		RadialSliderComponent* slider = static_cast<RadialSliderComponent*>(component.get());
		if (!slider || !slider->isEnabled)
			continue;

		// Get the TransformComponent to know the position and scale
		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!transform)
			continue;

		// Save old value to detect changes
		float oldValue = slider->currentValue;

		// Handle user interaction with this slider
		//HandleSliderInteraction(entityId, slider, transform);
	}
}

void RadialSliderSystem::Render() {
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<RadialSliderComponent>()) {
		RadialSliderComponent* slider = static_cast<RadialSliderComponent*>(component.get());

		if (!slider->isVisible || !slider->isEnabled)
			continue;

		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!transform)
			continue;

		// Use slider->radius instead of a fixed value
		float currentAngle = ConvertValueToAngle(slider->currentValue,
			slider->minValue,
			slider->maxValue,
			slider->minAngle,
			slider->maxAngle);

		DrawRadialSlider(transform->GetPosition(),
			slider->radius,  // Use the component's radius
			slider->thickness, // Use the component's thickness
			slider->minAngle,
			slider->maxAngle,
			currentAngle,
			slider->backgroundColor,  // Use component's background color
			slider->fillColor,        // Use component's fill color
			slider);
	}
}

void RadialSliderSystem::HandleSliderInteraction(int entityId, RadialSliderComponent* slider, TransformComponent* transform)
{
	// Get center position and radius
	Vector2D center = transform->GetPosition();
	float radius = transform->scale.x * 0.5f; // Assuming scale.x is diameter

	// Get mouse position
	Vector2D mousePos;

	if (DuckEngine::isEditor)
	{
		mousePos = DuckEngine::editorMouseWorldPos;
	}
	else
	{
		mousePos = Vector2D(
			static_cast<float>(DuckEngine_Input::GetMouseX()),
			static_cast<float>(DuckEngine_Input::GetMouseY())
		);

		// Convert screen position to world position
		mousePos = GraphicsManager::ScreenToWorld(mousePos);
	}

	// Check if mouse is inside the radial slider area
	if (IsPointInRadialSlider(mousePos, center, radius))
	{
		// Check for mouse click
		if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
		{
			// Calculate the angle from center to mouse position
			float angle = CalculateAngleFromPoint(center, mousePos);

			// Make sure the angle is within the slider's range
			if (angle >= slider->minAngle && angle <= slider->maxAngle)
			{
				// Convert the angle to a value in the slider's range
				float newValue = ConvertAngleToValue(
					angle,
					slider->minAngle,
					slider->maxAngle,
					slider->minValue,
					slider->maxValue
				);

				// Apply step if needed
				if (slider->step > 0.0f)
				{
					newValue = round(newValue / slider->step) * slider->step;
				}

				// Clamp the value to the valid range
				slider->currentValue = ClampValue(newValue, slider->minValue, slider->maxValue);
			}
		}
	}
}

bool RadialSliderSystem::IsPointInRadialSlider(const Vector2D& point, const Vector2D& center, float radius) const
{
	// Calculate distance between point and center
	float distance = Vec2Dist(point, center);

	// Check if the point is within the circle
	return distance <= radius;
}

float RadialSliderSystem::CalculateAngleFromPoint(const Vector2D& center, const Vector2D& point) const
{
	// Calculate vector from center to point
	Vector2D direction = point - center;

	// Calculate angle in radians
	float angleRad = atan2(direction.y, direction.x);

	// Convert to degrees and normalize
	float angleDeg = angleRad * 180.0f / M_PI;

	// Normalize to 0-360 range
	return NormalizeAngle(angleDeg);
}

float RadialSliderSystem::NormalizeAngle(float angle) const
{
	// Normalize angle to 0-360 range
	while (angle < 0.0f)
		angle += 360.0f;

	while (angle >= 360.0f)
		angle -= 360.0f;

	return angle;
}

float RadialSliderSystem::ConvertAngleToValue(float angle, float minAngle, float maxAngle, float minValue, float maxValue) const
{
	// Ensure angles are in normalized space
	angle = NormalizeAngle(angle);
	minAngle = NormalizeAngle(minAngle);
	maxAngle = NormalizeAngle(maxAngle);

	// Handle wraparound case
	if (maxAngle < minAngle)
	{
		if (angle < minAngle && angle > maxAngle)
			return minValue;

		if (angle >= minAngle)
			angle = angle - minAngle;
		else
			angle = (360.0f - minAngle) + angle;

		maxAngle = (maxAngle < minAngle) ? (360.0f - minAngle) + maxAngle : maxAngle - minAngle;
		minAngle = 0.0f;
	}

	if (fabs(maxAngle - minAngle) < 0.0001f) {
		return minValue; // Return min value if there's no range
	}

	// Calculate normalized position in angle range
	float normalizedPos = (angle - minAngle) / (maxAngle - minAngle);

	// Map to value range
	return minValue + normalizedPos * (maxValue - minValue);
}

float RadialSliderSystem::ConvertValueToAngle(float value, float minValue, float maxValue, float minAngle, float maxAngle) const
{
	// Make sure we don't divide by zero
	if (maxValue == minValue) return minAngle;

	// Simple linear mapping from value range to angle range
	float normalizedValue = (value - minValue) / (maxValue - minValue);
	return minAngle + normalizedValue * (maxAngle - minAngle);
}

float RadialSliderSystem::ClampValue(float value, float min, float max) const
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

void RadialSliderSystem::DrawRadialSlider
(
	const Vector2D& center,
	float radius,
	float thickness,
	float startAngle,
	float endAngle,
	float currentAngle,
	const Color& backgroundColor,
	const Color& fillColor,
	RadialSliderComponent* slider
)
{
	int sliderOrder = slider->sortingOrder;
	bool useCamera = true; // set false if it's UI-based

	startAngle += 90;
	endAngle += 90;

	for (float angle = startAngle; angle <= endAngle; angle += 1.0f)
	{
		float angleRad = angle * static_cast<float>(M_PI) / 180.0f;
		Vector2D point
		{
			center.x + radius * std::cos(angleRad),
			center.y + radius * std::sin(angleRad)
		};

		SliderRenderCommand cmd;
		cmd.position = point;
		cmd.radius = thickness; 
		cmd.color = backgroundColor;
		cmd.relativeToCamera = useCamera;

		GraphicsManager::AddToDrawQueue({ sliderOrder, 0, RenderCommandType::Slider, cmd });
	}

	if (slider->currentValue > slider->minValue)
	{
	
		float valueRange = slider->maxValue - slider->minValue;
		float percentFilled = (slider->currentValue - slider->minValue) / valueRange;
		float angleRange = endAngle - startAngle;

		float fillStartAngle = endAngle;
		float fillEndAngle = endAngle - (percentFilled * angleRange);

		fillEndAngle = std::max(fillEndAngle, startAngle);

		for (float angle = fillStartAngle; angle >= fillEndAngle; angle -= 1.0f)
		{
			float angleRad = angle * static_cast<float>(M_PI) / 180.0f;
			Vector2D point
			{
				center.x + radius * std::cos(angleRad),
				center.y + radius * std::sin(angleRad)
			};

			SliderRenderCommand cmd;
			cmd.position = point;
			cmd.radius = thickness;
			cmd.color = fillColor;
			cmd.relativeToCamera = useCamera;

			GraphicsManager::AddToDrawQueue({ sliderOrder + 1, 0, RenderCommandType::Slider, cmd });
		}
	}
}