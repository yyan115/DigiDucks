
#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"


class SliderLogic : public GameLogic
{
private:
	SliderComponent* slider;
	TransformComponent* transform;

public:

	SliderLogic()
		: GameLogic(nullptr), slider(nullptr), transform(nullptr) {
	}
	SliderLogic(GameLogicComponent* component)
		: GameLogic(component), slider(nullptr), transform(nullptr) {
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<SliderLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the Slider Logic
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Slider Logic
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the Slider Logic
	* ****************************************************************/
	void FixedUpdate() override;

};