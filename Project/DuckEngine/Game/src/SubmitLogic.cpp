#include "SubmitLogic.h"

void SubmitLogic::Start()
{
	submit_Station = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	submit_Transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(submit_Station->entityID);

	scoreText = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text");
	if (scoreText) {
		score_Text = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(scoreText->entityID);
	}
}

void SubmitLogic::removeObject(std::pair<int, ItemType> objData)
{
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objData.first);
	increaseScore();
}

void SubmitLogic::increaseScore(int score)
{
	totalScore += score;
	if (score_Text) {
		score_Text->text = "Score: " + std::to_string(totalScore);
	}
}