#pragma once
#include "GameLogic.h"

class LoadingScreenLogic : public GameLogic
{
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<LoadingScreenLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};