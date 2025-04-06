/******************************************************************************/
/*!
\file       RadialSliderComponent.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Declares the RadialSliderComponent class, a UI component that
			represents a circular slider with adjustable angles, radius,
			thickness, and value range. This component supports custom fill
			and background colors, and integrates with the DuckEngine
			component system.

			The radial slider is configurable in appearance and behavior,
			making it suitable for gameplay elements like volume knobs,
			timers, or health rings.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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