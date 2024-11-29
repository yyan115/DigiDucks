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


	void Start() override;
	
	void Update() override {}

	void FixedUpdate() override {}

	void removeObject(std::pair<int, ItemType> objData);

	void increaseScore(int score = 10);

	int getScore() { return totalScore; }
};