#pragma once

#include "DuckEngine.h"
#include "CustomerLogic.h"
#include "RestockLogic.h"
#include "CustomerLogic.h"
#include "PauseMenuLogic.h"
#include "GamesettingsLogic.h"
#include "MiniGameLogic.h"

class GameLoopLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	bool IsGameStarted();

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<GameLoopLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
	std::vector<CustomerLogic*> customers{};
	int currentCustomerIndex = 0;


private:
	// Helper functions
	void UpdateOrderTexture();

};
