/******************************************************************************/
/*!
\file       SubmitLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Declaration of the Submit Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"

class SubmitLogic : public GameLogic
{
private:
	Entity* submit_Station;
	TransformComponent* submit_Transform;
	Entity* scoreText;
	TextComponent* score_Text;
	int totalScore = 0;

public:
	SubmitLogic() :
		GameLogic(nullptr), submit_Station(nullptr), submit_Transform(nullptr), scoreText(nullptr), score_Text(nullptr), totalScore(0) {}

	SubmitLogic(GameLogicComponent* component) :
		GameLogic(nullptr), submit_Station(nullptr), submit_Transform(nullptr), scoreText(nullptr), score_Text(nullptr), totalScore(0)
	{
		UNREFERENCED_PARAMETER(component);
	}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<SubmitLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the Submit Station Logic.
	* ****************************************************************/
	void Start() override;
	
	/****************************************************************
	* @brief Update function for the Submit Station Logic.
	* ****************************************************************/
	void Update() override {}

	/****************************************************************
	* @brief FixedUpdate function for the Submit Station Logic.
	* ****************************************************************/
	void FixedUpdate() override {}

	/****************************************************************
	* @brief Shutdown function for the Submit Station Logic.
	* 
	* @param objData - The object data to remove.
	* ****************************************************************/
	void removeObject(std::pair<int, ItemType> objData);

	/****************************************************************
	* @brief Increase the score of the player.
	* 
	* @param score - The score to increase by.
	* ****************************************************************/
	void increaseScore(int score = 10);

	/****************************************************************
	* @brief Get the score of the player.
	* 
	* @return int - The score of the player.
	* ****************************************************************/
	int getScore() const { return totalScore; }

	/****************************************************************
	* @brief Check if a new order has been generated.
	*
	* @return bool - True if a new order has been generated.
	* ****************************************************************/
	bool CheckNewOrder();
};