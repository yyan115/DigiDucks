/******************************************************************************/
/*!
\file       SliderLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Feb 02 2025
\brief      Declaration of Slider Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

	/****************************************************************
	* @brief Increase Horizontal function for the Slider Logic
	* ****************************************************************/
	void IncreaseHorizontal();

	/****************************************************************
	* @brief Decrease Horizontal function for the Slider Logic
	* ****************************************************************/
	void DecreaseHorizontal();

	/****************************************************************
	* @brief Increase Vertical function for the Slider Logic
	* ****************************************************************/
	void IncreaseVertical();

	/****************************************************************
	* @brief Decrease Vertical function for the Slider Logic
	* ****************************************************************/
	void DecreaseVertical();

};