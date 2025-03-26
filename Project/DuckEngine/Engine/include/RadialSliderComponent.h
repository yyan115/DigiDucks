#pragma once

#include "Component.h"
#include "Color.h"

class RadialSliderComponent : public Component
{
public:
	RadialSliderComponent()
		: isEnabled(false),
		isVisible(true),
		sortingOrder(0),
		minValue(0.0f),
		maxValue(1.0f),
		currentValue(0.0f),
		step(0.1f),
		minAngle(0.0f),
		maxAngle(360.0f),
		radius(1.0f),
		thickness(1.0f),  // Default thickness for the slider
		backgroundColor(100, 100, 100, 255),  // Default gray
		fillColor(50, 150, 255, 255)  // Default blue
	{
	}

	virtual ~RadialSliderComponent() override = default;

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<RadialSliderComponent>(*this);
	}

	bool isEnabled;
	bool isVisible;
	int sortingOrder;

	float minValue;
	float maxValue;
	float currentValue;
	float step;

	float minAngle;
	float maxAngle;

	float radius;    // Size of the radial slider
	float thickness; // Thickness of the slider arc

	// Custom colors
	Color backgroundColor;  // Background arc color
	Color fillColor;        // Filled arc color
};