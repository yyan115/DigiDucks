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

Entity* orderTab = nullptr;
bool newOrderGenerated = false;

/****************************************************************
* @brief Start function for the Submit Station Logic.
* ****************************************************************/
void SubmitLogic::Start()
{
	submit_Station = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	submit_Transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(submit_Station->entityID);

	scoreText = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text");
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
	increaseScore();
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
		score_Text->text = "Score: " + std::to_string(totalScore);
	}
	newOrderGenerated = true;
}

bool SubmitLogic::CheckNewOrder() {
	if (newOrderGenerated) {
		newOrderGenerated = false;
		return true;
	}
	return false;
}