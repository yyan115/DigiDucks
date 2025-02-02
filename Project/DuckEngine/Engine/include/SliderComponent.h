/******************************************************************************/
/*!
\file		SliderComponent.h
\author 	Ernest Ho, h.yonghengernest, 2301223 (100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Feb 1 2025
\brief  	This file holds the component of Slider for the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Component.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class SliderComponent :public Component {
public:
	bool isEnable;			// Enable the slider
	bool isIncrease;		// Increase/Decrease the slider
	bool isHorizontal;		// Direction of the slider
	bool isVertical;		// Direction of the slider
	float minValue;			// Minimum value of the slider
	float maxValue;			// Maximum value of the slider
	float currentValue;		// Current value of the slider
	float step;				// Step value of the slider

	// Constructor initializes the SliderComponent with default values
	DUCKENGINE_API SliderComponent(bool enable = false, bool increase = true, bool sides = true, bool heights = false, float min = 0.0f, float max = 1.0f, float current = 0.0f, float step = 0.1f) :
		isEnable(enable), isIncrease(increase), isHorizontal(sides), isVertical(heights), minValue(min), maxValue(max), currentValue(current), step(step) { }

	std::shared_ptr<Component> Clone() const override 
	{
		return std::make_shared<SliderComponent>(*this);
	}
};