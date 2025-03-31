/******************************************************************************/
/*!
\file       SubmitLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Definition of the Submit Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "SubmitLogic.h"
#include "GameLoopLogic.h"

Entity* orderTab = nullptr;
bool newOrderGenerated = false;

/****************************************************************
* @brief Start function for the Submit Station Logic.
* ****************************************************************/
void SubmitLogic::Start()
{
	submit_Station = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	submit_Transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(submit_Station->entityID);

	scoreText = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text").get();
	if (scoreText) {
		score_Text = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(scoreText->entityID);
	}
}

/****************************************************************
* @brief Shutdown function for the Submit Station Logic.
*
* @param objData - The object data to remove.
* ****************************************************************/
void SubmitLogic::removeObject(std::pair<int, ItemType> objData)
{
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objData.first);
	//increaseScore();

	// LOGIC TO INCREASE SCORE BY 10
	Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station").get();
	if (submit)
	{
		auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(submit->entityID);

		submitLogic->increaseScore(10);
	}

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	if (gameLoopEntity) 
	{
		GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();
		if (gameLoopLogic && gameLoopLogic->currentActiveOrders > 0) 
		{
			gameLoopLogic->currentActiveOrders--;
		}
	}
}

/****************************************************************
* @brief Increase the score of the player.
*
* @param score - The score to increase by.
* ****************************************************************/
void SubmitLogic::increaseScore(int score)
{
	totalScore += score;
	if (score_Text) {
		score_Text->text = std::to_string(totalScore);
	}
	newOrderGenerated = true;
}


/****************************************************************
* @brief Decrease the score of the player.
*
* @param score - The score to decrease by.
* ****************************************************************/
void SubmitLogic::decreaseScore(int score)
{
	totalScore -= score;
	if (score_Text) {
		score_Text->text = std::to_string(totalScore);
	}
}


/****************************************************************
* @brief Check if a new order has been generated.
*
* @return bool - True if a new order has been generated.
* ****************************************************************/
bool SubmitLogic::CheckNewOrder() {
	if (newOrderGenerated) {
		newOrderGenerated = false;
		return true;
	}
	return false;
}