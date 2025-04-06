/******************************************************************************/
/*!
\file       RadialSliderSystem.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Implements the RadialSliderSystem class, which manages the
			behavior, interaction, and rendering of radial slider UI
			components. This system supports dynamic visuals based on
			angle and value mapping, and integrates with the DuckEngine's
			rendering pipeline.

			Key functionalities include:
			- Converting between angles and values
			- Clamping and normalizing input
			- Rendering the arc-based background and fill of sliders
			- Handling slider visibility and sorting logic
			- Supporting dynamic updates via the DuckEngine component system

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "RadialSliderSystem.h"
#include "GraphicsManager.h"
#include "DuckEngine_Input.h"
#include "WindowManager.h"
#include "ImageLoader.h"
#include <cmath>

void RadialSliderSystem::Start()
{

}

void RadialSliderSystem::Update()
{

}

void RadialSliderSystem::Render() 
{
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<RadialSliderComponent>()) {
		RadialSliderComponent* slider = static_cast<RadialSliderComponent*>(component.get());

		if (!slider->isVisible || !slider->isEnabled)
			continue;

		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!transform)
			continue;

		DrawRadialSlider(transform->GetPosition(),
			slider->radius,  // Use the component's radius
			slider->thickness, // Use the component's thickness
			slider->minAngle,
			slider->maxAngle,
			slider->backgroundColor,  // Use component's background color
			slider->fillColor,        // Use component's fill color
			slider);
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
	float angleDeg = static_cast<float>(angleRad * 180.0f / M_PI);

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