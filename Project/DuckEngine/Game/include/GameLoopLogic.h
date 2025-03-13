#pragma once

#include "DuckEngine.h"
#include "CustomerLogic.h"
#include "CustomerLogic.h"
#include "PauseMenuLogic.h"
#include "GamesettingsLogic.h"
#include "CutSceneLogic.h"

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
	int customerCount = 0;
	bool isCustomerWaitingForOrder = false;
	float timeSinceLastCustomer = 0.0f;
	float customerSpawnInterval = 10.0f;
	float customerSpawnCooldown = 3.0f;
	bool isSpawningCustomer = false; 
	int maxActiveOrders = 3;
	int currentActiveOrders = 0;
	int customersFinished = 0;


	std::vector<std::pair<Entity*, bool>> seatingLocations{};
	bool IsSeatOccupied(Entity* seat);
	bool OccupySeat(Entity* seat);
	void FreeSeat(Entity* seat);

private:
	Entity* CutScene = nullptr;
	std::shared_ptr<CutSceneLogic> CutSceneManager = nullptr;
};
