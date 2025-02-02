#include "SliderLogic.h"

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

}

/****************************************************************
* @brief FixedUpdate function for the Slider Logic
* ****************************************************************/
void SliderLogic::FixedUpdate()
{

}
