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
	bool isEnable;
	float minValue;
	float maxValue;
	float currentValue;
	float step;

	DUCKENGINE_API SliderComponent(bool enable = false, float min = 0.0f, float max = 1.0f, float current = 0.0f, float step = 0.1f) : isEnable(enable), minValue(min), maxValue(max), currentValue(current), step(step) {}

	std::shared_ptr<Component> Clone() const override 
	{
		return std::make_shared<SliderComponent>(*this);
	}

};