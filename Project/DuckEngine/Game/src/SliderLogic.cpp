#include "SliderLogic.h"
#include <iostream>


/****************************************************************
* @brief Start function for the Slider Logic
* ****************************************************************/
void SliderLogic::Start()
{
	slider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(component->GetEntityID());
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
}

/****************************************************************
* @brief Update function for the Slider Logic
* ****************************************************************/
void SliderLogic::Update()
{
	if (slider->isEnable)
	{
		std::cout << "Slider is Enable" << std::endl;
		// Increase
		if (slider->isIncrease)
		{
			if (slider->currentValue+slider->step < slider->maxValue)
			{
				// Horizontal
				if (slider->isHorizontal)
				{
					transform->SetPosition(Vec2(transform->GetPosition().x + slider->step, transform->GetPosition().y));
					transform->scale.x += 2 * slider->step;
				}

				// Vertical
				if (slider->isVertical)
				{
					transform->SetPosition(Vec2(transform->GetPosition().x, transform->GetPosition().y + slider->step));
					transform->scale.y += 2 * slider->step;
				}

				slider->currentValue += slider->step;
			}
			else {
				slider->isEnable = false;
			}
		}
		// Decrease
		else
		{
			if (slider->currentValue-slider->step > slider->minValue)
			{
				// Horizontal
				if (slider->isHorizontal)
				{
					if (transform->scale.x > 0) {
						transform->SetPosition(Vec2(transform->GetPosition().x - slider->step, transform->GetPosition().y));
						transform->scale.x -= 2 * slider->step;
					}
				}

				// Vertical
				if (slider->isVertical)
				{
					if (transform->scale.y > 0) {
						transform->SetPosition(Vec2(transform->GetPosition().x, transform->GetPosition().y - slider->step));
						transform->scale.y -= 2 * slider->step;
					}
				}

				slider->currentValue -= slider->step;
			}
			else {
				slider->isEnable = false;
			}

		}
	}
}

/****************************************************************
* @brief FixedUpdate function for the Slider Logic
* ****************************************************************/
void SliderLogic::FixedUpdate()
{

}
