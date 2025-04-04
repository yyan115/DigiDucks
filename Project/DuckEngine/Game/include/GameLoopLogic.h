/******************************************************************************/
/*!
\file       GameLoopLogic.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the GameLoopLogic class, which defines the core game
			loop, managing customer interactions, seating logic, and
			gameplay states. This class integrates with DuckEngine’s
			component system and various logic managers, including
			PauseMenuLogic, GameSettingsLogic, and CutSceneLogic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include "DuckEngine.h"
#include "CustomerLogic.h"
#include "CustomerLogic.h"
#include "PauseMenuLogic.h"
#include "GamesettingsLogic.h"
#include "CutSceneLogic.h"
#include <tuple>

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

	std::vector<std::tuple<Entity*, bool, std::vector<Entity*>>> seatingLocations{};

	// entity is seat entity, customer is customer logic
	std::vector<std::pair<Entity*, CustomerLogic*>> customersAtSeats{};

	bool IsSeatOccupied(Entity* seat);
	bool OccupySeat(Entity* seat);
	void FreeSeat(Entity* seat);

	void SetCustomer(Entity* seat, CustomerLogic* customer) {
		for (auto &pair : customersAtSeats) {
			if (pair.first == seat) {
				pair.second = customer;
			}
		}
	}

	void RemoveCustomer(Entity* seat, CustomerLogic* customer) {
		for (auto &pair : customersAtSeats) {
			if (pair.first == seat) {
				pair.second = nullptr;
			}
		}
	}

	// Timer
	Entity* timer = nullptr;
	TextComponent* timerText = nullptr;
	float timeLeft = 0.f;

private:
	Entity* CutScene = nullptr;
	std::shared_ptr<CutSceneLogic> CutSceneManager = nullptr;
};
