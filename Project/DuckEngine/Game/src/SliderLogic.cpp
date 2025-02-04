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
		// Increase
		if (slider->isIncrease)
		{
			// Horizontal
			IncreaseHorizontal();

			// Vertical
			IncreaseVertical();

			slider->currentValue += slider->step;
		}
		// Decrease
		else
		{
			// Horizontal
			DecreaseHorizontal();

			// Vertical
			DecreaseVertical();				

			slider->currentValue -= slider->step;			
		}
	}
}

/****************************************************************
* @brief FixedUpdate function for the Slider Logic
* ****************************************************************/
void SliderLogic::FixedUpdate()
{

}


/****************************************************************
* @brief Increase Horizontal function for the Slider Logic
* ****************************************************************/
void SliderLogic::IncreaseHorizontal()
{
	if (slider->currentValue + slider->step < slider->maxValue)
	{
		if (slider->isHorizontal)
		{
			transform->SetPosition(Vec2(transform->GetPosition().x + slider->step / 2.0f, transform->GetPosition().y));
			transform->scale.x += slider->step;
		}
	}
	else 
	{
		slider->isEnable = false;
	}
}

/****************************************************************
* @brief Decrease Horizontal function for the Slider Logic
* ****************************************************************/
void SliderLogic::DecreaseHorizontal()
{
	if (slider->currentValue - slider->step > slider->minValue)
	{
		if (slider->isHorizontal)
		{
			if (transform->scale.x > 0) {
				transform->SetPosition(Vec2(transform->GetPosition().x - slider->step / 2.0f, transform->GetPosition().y));
				transform->scale.x -= slider->step;
			}
		}
	}
	else {
		slider->isEnable = false;
	}
}

/****************************************************************
* @brief Increase Vertical function for the Slider Logic
* ****************************************************************/
void SliderLogic::IncreaseVertical()
{

	if (slider->currentValue + slider->step < slider->maxValue)
	{
		if (slider->isVertical)
		{
			transform->SetPosition(Vec2(transform->GetPosition().x, transform->GetPosition().y + slider->step / 2.0f));
			transform->scale.y += slider->step;
		}
	}
	else 
	{
		slider->isEnable = false;
	}
}

/****************************************************************
* @brief Decrease Vertical function for the Slider Logic
* ****************************************************************/
void SliderLogic::DecreaseVertical()
{
	if (slider->currentValue - slider->step > slider->minValue)
	{
		if (slider->isVertical)
		{
			if (transform->scale.y > 0) {
				transform->SetPosition(Vec2(transform->GetPosition().x, transform->GetPosition().y - slider->step / 2.0f));
				transform->scale.y -= slider->step;
			}
		}
	}
	else {
		slider->isEnable = false;
	}
}