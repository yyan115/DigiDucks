#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class RestockLogic : public GameLogic
{
private:
	Entity* restockStation;
	TransformComponent* restockTransform;

public:

	RestockLogic() :
		GameLogic(nullptr), restockStation(nullptr), restockTransform(nullptr) {}

	RestockLogic(GameLogicComponent* component) :
		GameLogic(nullptr), restockStation(nullptr), restockTransform(nullptr) {}

	void Start() override;
	void Update() override;


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RestockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void restockAll();

};