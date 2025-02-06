#include "SliderLogic.h"
#include <iostream>


/****************************************************************
* @brief Start function for the Slider Logic
* ****************************************************************/
void SliderLogic::Start()
{
	sliderEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (sliderEntity)
	{
		sliderBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());
		if (sliderBgSpt)
		{
		//	sliderBgSpt->isVisible = false;
		}
		slider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(component->GetEntityID());
		if (slider)
		{
			// Get the Slider's Child Transform (Green Part)
			if (sliderEntity->childEntities.size() > 0)
			{
				sliderTrfm = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(sliderEntity->childEntities[0]->entityID);

				if (sliderTrfm)
				{
					originalPos = sliderTrfm->GetPosition();
					originalScale = sliderTrfm->scale;
				}
			}
		}
		
	}
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

			if(slider->currentValue >= slider->maxValue)
				slider->currentValue = slider->maxValue;
		}
		// Decrease
		else
		{
			// Horizontal
			DecreaseHorizontal();

			// Vertical
			DecreaseVertical();				

			slider->currentValue -= slider->step;

			if (slider->currentValue <= slider->minValue)
				slider->currentValue = slider->minValue;
		}		
	}
	slider->isEnable = false;
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
			sliderTrfm->SetPosition(Vec2(sliderTrfm->GetPosition().x + slider->step / 2.0f, sliderTrfm->GetPosition().y));
			sliderTrfm->scale.x += slider->step;
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
			if (sliderTrfm->scale.x > 0) {
				sliderTrfm->SetPosition(Vec2(sliderTrfm->GetPosition().x - slider->step / 2.0f, sliderTrfm->GetPosition().y));
				sliderTrfm->scale.x -= slider->step;
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
			sliderTrfm->SetPosition(Vec2(sliderTrfm->GetPosition().x, sliderTrfm->GetPosition().y + slider->step / 2.0f));
			sliderTrfm->scale.y += slider->step;
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
			if (sliderTrfm->scale.y > 0) {
				sliderTrfm->SetPosition(Vec2(sliderTrfm->GetPosition().x, sliderTrfm->GetPosition().y - slider->step / 2.0f));
				sliderTrfm->scale.y -= slider->step;
			}
		}
	}
	else {
		slider->isEnable = false;
	}
}

/****************************************************************
* @brief Reset Slider function for the Slider Logic
* ****************************************************************/
void SliderLogic::ResetSlider()
{
	if (sliderTrfm)
	{
		slider->currentValue = slider->minValue;
		sliderTrfm->SetPosition(originalPos);
		sliderTrfm->scale = originalScale;
	}
}