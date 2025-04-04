/******************************************************************************/
/*!
\file       PlayerLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Definition of all Player Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PlayerLogic.h"
#include "CombineLogic.h"
#include "SubmitLogic.h"
#include "RestockLogic.h"
#include "CustomerLogic.h"
#include "HighlightLogic.h"
#include "CustomerTableLogic.h"
#include "GameLoopLogic.h"

float actionCooldown = 0.5f;
float actionCounter = 0.5f;


/****************************************************************
* @brief Start function for the Player Logic
* ****************************************************************/
void PlayerLogic::Start()
{
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());
	circleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(component->GetEntityID());
	boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
	holding = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID()).get();
	movement = GameLogicManager::GetLogicForEntity<MovementLogic>(component->GetEntityID()).get();

	Entity* orderTabEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tabs").get();
	if (orderTabEntity)
		orderTabLogic = GameLogicManager::GetLogicForEntity<OrderTabLogic>(orderTabEntity->entityID).get();

	auto restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(restockMenu->entityID).get();
	}

	// SFX Managers
	Entity* SFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXManager").get();
	if (SFX)
	{
		SFXsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(SFX->entityID);
	}
	Entity* PlateSFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("PlateSFXManager").get();
	if (PlateSFX)
	{
		PlateSFXsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(PlateSFX->entityID);
	}
	Entity* BowlSFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BowlSFXManager").get();
	if (BowlSFX)
	{
		BowlSFXsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(BowlSFX->entityID);
	}
	Entity* PanSFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("PanSFXManager").get();
	if (PanSFX)
	{
		PanSFXsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(PanSFX->entityID);
	}
	Entity* PotSFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("PotSFXManager").get();
	if (PotSFX)
	{
		PotSFXsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(PotSFX->entityID);
	}

	dir = FRONT;
	isHolding = false;
	actionCounter = actionCooldown;

	if (boxCollider)
	{
		boxCollider->isKinematic = false;
		boxCollider->SetCollisionCallback([this](int otherEntityID)
			{
				interactObject = DuckEngine::DUCKENGINE_EntityManager.GetEntity(otherEntityID).get();
				if (interactObject)
				{
					auto highlightLogic = GameLogicManager::GetLogicForEntity<HighlightLogic>(interactObject->entityID);
					if (highlightLogic)
					{
						highlightLogic->isHighlighted = true;
					}
				}

				// Check gamepad connection
				const int gamepadIndex = DuckEngine_Input::GAMEPAD_1;
				bool gamepadConnected = DuckEngine_Input::IsGamepadConnected(gamepadIndex);

				// Pickup Object (J key or left face button - Square on PlayStation, X on Xbox)
				if ((DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_J) ||
					(gamepadConnected && DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_X)))
					&& actionCounter <= 0)
				{
					InteractPressed();
					actionCounter = actionCooldown;
				}
				// Use Object (K key or bottom face button - X on PlayStation, A on Xbox)
				else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_K) ||
					(gamepadConnected && DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_A)))
				{
					InteractHold();
				}
			});
	}
}


/****************************************************************
* @brief Update function for the Player Logic
* ****************************************************************/
void PlayerLogic::Update()
{
	if (transform->GetPosition().y <= -2.0f)
	{
		spriteRenderer->sortingOrder = 8;
	}
	else
	{
		spriteRenderer->sortingOrder = 2;
	}

	if (restockLogic)
	{
		if (restockLogic->isRestock)
		{
			movement->isMoving = false;
		}
		else
		{
			movement->isMoving = true;
		}
	}

	if (actionCounter >= 0)
	{
		actionCounter -= DuckEngine::DeltaTime();
	}

	const int gamepadIndex = DuckEngine_Input::GAMEPAD_1;
	bool gamepadConnected = DuckEngine_Input::IsGamepadConnected(gamepadIndex);

	// Track joystick movement for animation transitions
	static bool wasMovingWithJoystick = false;
	bool isMovingWithJoystick = false;

	// Check if currently moving with joystick
	if (gamepadConnected) {
		float leftStickX = DuckEngine_Input::GetGamepadAxisValue(gamepadIndex, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
		float leftStickY = DuckEngine_Input::GetGamepadAxisValue(gamepadIndex, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);
		bool dpadActive = DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP) ||
			DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN) ||
			DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT) ||
			DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);

		const float deadzone = 0.2f;
		isMovingWithJoystick = (std::abs(leftStickX) > deadzone || std::abs(leftStickY) > deadzone || dpadActive);
	}

	if (animator)
	{
		if (movement->isMoving)
		{
			// Detect joystick returning to neutral position (stick released)
			bool joystickReleased = wasMovingWithJoystick && !isMovingWithJoystick;

			// Check both keyboard and gamepad inputs for animation updates
			if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_D)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_A)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_S)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_W)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_K)
				|| (gamepadConnected && DuckEngine_Input::IsGamepadButtonReleased(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_A))
				|| joystickReleased)
			{
				if (dir == FRONT)
					animator->PlayAnimation("FRONT_IDLE");
				else if (dir == BACK)
					animator->PlayAnimation("BACK_IDLE");
				else if (dir == LEFT)
					animator->PlayAnimation("LEFT_IDLE");
				else if (dir == RIGHT)
					animator->PlayAnimation("RIGHT_IDLE");
			}
		}
	}

	// Update previous joystick state for next frame
	wasMovingWithJoystick = isMovingWithJoystick;

	// Cheats
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_V))
	{
		if (!isHolding) {
			ItemType cheatType = ItemType::SALAD_PLATE;
			Entity* newObject = makeObject(cheatType);
			holding->setObject(std::make_pair(newObject->entityID, cheatType));
			isHolding = true;
		}
	}
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_B))
	{
		if (!isHolding) {
			ItemType cheatType = ItemType::CHEESE_BURGER_PLATE;
			Entity* newObject = makeObject(cheatType);
			holding->setObject(std::make_pair(newObject->entityID, cheatType));
			isHolding = true;
		}
	}
}

/****************************************************************
* @brief FixedUpdate function for the Player Logic
* ****************************************************************/
void PlayerLogic::FixedUpdate()
{
	auto playersound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(component->GetEntityID());

	// If player is not moving, do nothing
	if (!movement->isMoving)
	{
		playersound->Stop();
		animator->Pause();
		return;
	}

	// Check for gamepad input
	const int gamepadIndex = DuckEngine_Input::GAMEPAD_1;
	bool gamepadConnected = DuckEngine_Input::IsGamepadConnected(gamepadIndex);

	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;

	// Check keyboard input
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W)) {
		movingUp = true;
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S)) {
		movingDown = true;
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A)) {
		movingLeft = true;
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D)) {
		movingRight = true;
	}

	// Check gamepad input if connected
	if (gamepadConnected) {
		// Analog stick input (prioritize this if it's being used)
		float leftStickX = DuckEngine_Input::GetGamepadAxisValue(gamepadIndex, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
		float leftStickY = DuckEngine_Input::GetGamepadAxisValue(gamepadIndex, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);

		const float deadzone = 0.2f;
		if (std::abs(leftStickX) > deadzone || std::abs(leftStickY) > deadzone) {
			// Reset keyboard-based directions as we'll use analog instead
			movingUp = false;
			movingDown = false;
			movingLeft = false;
			movingRight = false;

			// Determine direction from analog stick
			if (leftStickY < -deadzone) {
				movingUp = true;
			}
			if (leftStickY > deadzone) {
				movingDown = true;
			}
			if (leftStickX < -deadzone) {
				movingLeft = true;
			}
			if (leftStickX > deadzone) {
				movingRight = true;
			}
		}

		// D-pad input (if analog stick isn't being used)
		if (!(movingUp || movingDown || movingLeft || movingRight)) {
			if (DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP)) {
				movingUp = true;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN)) {
				movingDown = true;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT)) {
				movingLeft = true;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(gamepadIndex, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT)) {
				movingRight = true;
			}
		}
	}

	// Apply movement based on collected input
	if (movingUp)
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, 1.f);
		}
		if (animator)
		{
			playersound->Play(-1);
			animator->PlayAnimation("BACK_WALK");
			dir = BACK;
		}
	}
	if (movingDown)
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, -1.f);
		}
		if (animator)
		{
			playersound->Play(-1);
			animator->PlayAnimation("FRONT_WALK");
			dir = FRONT;
		}
	}
	if (movingLeft)
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(-1.f, 0.f);
		}
		if (animator)
		{
			playersound->Play(-1);
			animator->PlayAnimation("LEFT_WALK");
			dir = LEFT;
		}
	}
	if (movingRight)
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(1.f, 0.f);
		}
		if (animator)
		{
			playersound->Play(-1);
			animator->PlayAnimation("RIGHT_WALK");
			dir = RIGHT;
		}
	}
}

/****************************************************************
* @brief Helper function to get the SFX type of object
* ****************************************************************/
SoundComponent* PlayerLogic::GetSFXForType(int type) {
	if (type <= 14) return SFXsound;
	if (type >= 15 && type <= 18) return PanSFXsound;
	if (type >= 19 && type <= 22) return PotSFXsound;
	if (type >= 23 && type <= 37) return PlateSFXsound;
	if (type >= 38 && type <= 41) return BowlSFXsound;
	return SFXsound;
}

/****************************************************************
* @brief Function to handle when player press a key
* ****************************************************************/
void PlayerLogic::InteractPressed()
{
	if (DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SoundComponent>(interactObject->entityID)) {
		sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(interactObject->entityID);
	}
	ItemType type = ItemType::EMPTY;

	// If player isnt holding anything
	if (!isHolding)
	{
		// Check if Stock Object
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
		if (stockLogic)
		{
			if (sound) {
				sound->Stop();
				sound->Play(-1);
			}

			// If empty stock or Bin, do nothing
			if (stockLogic->getType() == ItemType::BIN || stockLogic->getType() == ItemType::EMPTY) return;

			// If stock is not empty, create object and set holding
			if (stockLogic->isEmpty()) return;
			stockLogic->useStock();

			Entity* newObject = makeObject(stockLogic->getType());
			holding->setObject(std::make_pair(newObject->entityID, stockLogic->getType()));

			isHolding = true;
			return;
		}

		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(interactObject->entityID);
		if (tableLogic)
		{
			// If Table is occupied, take object from table
			if (tableLogic->isOccupied)
			{
				holding->setObject(tableLogic->moveObject());
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					if (static_cast<int>(type) <= 14) soundToPlay->Play(1);
					else soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			return;
		}

		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{
			// If Board is occupied, take object from board
			if (chopBoardLogic->isOccupied)
			{
				holding->setObject(chopBoardLogic->moveObject());
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			return;
		}

		auto stoveLogic = GameLogicManager::GetLogicForEntity<StoveLogic>(interactObject->entityID);
		if (stoveLogic)
		{
			if ((stoveLogic->isPot || stoveLogic->isPan) && !stoveLogic->isOccupied)
			{
				holding->setObject(stoveLogic->moveObject());
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			else if (stoveLogic->isPan && stoveLogic->isOccupied)
			{
				ItemType temp = stoveLogic->movePatty();
				Entity* newObject = makeObject(temp);
				holding->setObject(std::make_pair(newObject->entityID, temp));
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			else if (stoveLogic->isPot && stoveLogic->isOccupied)
			{
				// If Pot is still cooking, return
				if (!stoveLogic->isCooked) return;
				holding->setObject(stoveLogic->moveObject());
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			return;
		}

		auto robotLogic = GameLogicManager::GetLogicForEntity<RobotLogic>(interactObject->entityID);
		if (robotLogic)
		{
			// If Robot restock station is restocking, return
			if (restockLogic->isDelay) return;

			if (sound) sound->Play(0);
			setRestockMenu(true);
			return;
		}

	}

	// If player is already holding something
	else if (isHolding)
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
		if (stockLogic)
		{
			// If Stock is Not BIN, do nothing
			if (stockLogic->getType() != ItemType::BIN) return;

			// If Holding is Filled Pot, Empty Pot
			if (isTypePot(holding->getType()))
			{
				auto potLogic = GameLogicManager::GetLogicForEntity<PotLogic>(holding->getObjectID());
				if (potLogic)
				{
					holding->setType(potLogic->EmptyPot());
					return;
				}
			}

			// If Holding Item is Pan or Pot, do nothing
			if (isEquipment(holding->getType())) return;

			// If Stock is BIN, Destroy Object
			holding->deleteObject();
			if (sound) sound->Play(-1);
			isHolding = false;
			return;
		}

		auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(interactObject->entityID);

		auto customerTable = GameLogicManager::GetLogicForEntity<CustomerTableLogic>(interactObject->entityID);

		if (customerTable) {
			std::cout << "CUSTOMERR TABLE DETEECED\n";

			Entity* gameLogicManager = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
			GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLogicManager->entityID).get();

			CustomerLogic* correctCustomer = nullptr;
			std::pair<Entity*, CustomerLogic*> pairCustomer;

			// very convoluted way to check and submit order.
			// table in scene contains CustomerTableLogic - this holds the seat entity ID and gets seat when seatingLocations get init
			// 1. First check for the correct seat in seatingLocations - match current customer table's seat ID with seatingLocations's seat ID
			// 2. Now use customersAtSeats to get access to customerLogic, by matching the seat IDs from both seatingLocations and customersAtSeats
			// Now you are 100% sure you got access to customer that is currently waiting.
			for (auto& pairSeat : gameLoopLogic->seatingLocations)
			{
				// match seat and table
				if (std::get<0>(pairSeat) == customerTable.get()->seatEntity && std::get<0>(pairSeat) != nullptr) {
					//

					std::cout << "1 - FOUND tuple seat and customer table seat\n";

					for (auto& pair : gameLoopLogic->customersAtSeats)
					{
						if (pair.first == std::get<0>(pairSeat) && pair.first != nullptr) {
							//
							correctCustomer = pair.second;
							pairCustomer = pair;

							std::cout << "Found correct customer\n";
						}
					}
				}
			}

			ItemType heldType = holding->getType();

			auto& allOrderTabs = orderTabLogic->GetOrderTabs();
			bool orderSubmitted = false;

			std::cout << "Looping through tabs now.\n";

			for (auto& tab : allOrderTabs)
			{
				// find customer in tab
				if (tab.tabCustomer == correctCustomer && correctCustomer != nullptr) {

					std::cout << "Correct customer matched with tab.\n";

					if (tab.tabCustomer == nullptr) { std::cout << "tab is nullptr???\n"; }
					if (orderSubmitted) { std::cout << "order is true???\n"; }

					// copy pasted from ernest code
					if (tab.tabCustomer && !orderSubmitted)
					{
						ItemType requestedItem = tab.tabOrder;
						bool isReadyToCollect = tab.tabCustomer->WalkState->GetIsWaitingToCollectOrder();

						std::cout << "Checking if can submit\n";

						if (heldType == requestedItem && isReadyToCollect)
						{
							tab.tabCustomer->OrderCompleted();

							submitLogic->removeObject(holding->moveObject());

							//tab.tabCustomer->WalkState.get()->currentTargetIndex = 0;
							//tab.tabCustomer->WalkState.get()->currentQueueTarget = tab.tabCustomer->WalkState.get()->seatPoints[tab.tabCustomer->WalkState.get()->currentTargetIndex];
							tab.tabCustomer->WalkState.get()->orderCollectedNowLeave = true;

							orderTabLogic->RemoveOrder(tab.tabCustomer);

							if (sound)
							{
								sound->Play();
							}

							isHolding = false;
							orderSubmitted = true;

							gameLoopLogic->RemoveCustomer(pairCustomer.first);

							std::cout << "Order submitted.\n";
							return;
						}
					}
				}
			}

		}
		

		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(interactObject->entityID);
		if (tableLogic)
		{
			// If Table is occupied, take object from table
			if (!tableLogic->isOccupied)
			{
				tableLogic->setObject(holding->moveObject());
				type = tableLogic->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					if (static_cast<int>(type) <= 14) soundToPlay->Play();
					else soundToPlay->Play(-1);	
				}
				isHolding = false;
			}
			else if (tableLogic->isOccupied)
			{
				if (holding->getType() == ItemType::BOWL)
				{
					if (tableLogic->isPotFilled())
					{
						holding->deleteObject();
						ItemType temp = tableLogic->moveSoup();
						Entity* newObject = makeObject(temp);
						holding->setObject(std::make_pair(newObject->entityID, temp));
						type = holding->getType();
						SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
						if (soundToPlay) {
							soundToPlay->Stop();
							soundToPlay->Play(-1);
						}
						isHolding = true;
					}
					return;
				}

				else if (canCombine(holding->getType(), tableLogic->getType()))
				{
					std::pair<int, ItemType> combined = combineObjects(holding->moveObject(), tableLogic->moveObject());
					tableLogic->setObject(combined);
					type = tableLogic->getType();
					SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
					if (soundToPlay) {
						soundToPlay->Stop();
						soundToPlay->Play(-1);
					}
					isHolding = false;
				}
			}
			return;
		}

		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{
			if (!chopBoardLogic->isOccupied)
			{
				// If Object is Ingredient, put on board
				if (chopBoardLogic->checkIngredient(holding->getType()))
				{
					chopBoardLogic->setObject(holding->moveObject());
					type = chopBoardLogic->getType();
					SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
					if (soundToPlay) {
						soundToPlay->Stop();
						soundToPlay->Play(-1);
					}
					isHolding = false;
				}
			}
			return;
		}

		auto stoveLogic = GameLogicManager::GetLogicForEntity<StoveLogic>(interactObject->entityID);
		if (stoveLogic)
		{
			if (!stoveLogic->isPot && !stoveLogic->isPan)
			{
				if (!isEquipment(holding->getType())) return;
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
					soundToPlay->Stop();
					soundToPlay->Play(-1);
				}
				stoveLogic->setCookingType(holding->moveObject());
				isHolding = false;
			}
			else if (stoveLogic->isPan)
			{
				if (!stoveLogic->isOccupied)
				{
					if (holding->getType() != ItemType::R_PATTY && holding->getType() != ItemType::C_PATTY) return;
					type = holding->getType();
					SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
					if (soundToPlay) {
						soundToPlay->Stop();
						soundToPlay->Play(-1);
					}
					// Delete Holding object and assign 
					stoveLogic->setObject(holding->moveObject());
					isHolding = false;
				}
			}
			else if (stoveLogic->isPot)
			{
				if (!stoveLogic->isOccupied)
				{
					if (!isIngredient(holding->getType())) return;
					type = holding->getType();
					SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
					if (soundToPlay) {
						soundToPlay->Stop();
						soundToPlay->Play(-1);
					}
					stoveLogic->setObject(holding->moveObject());
					isHolding = false;
				}
				else if (stoveLogic->isOccupied)
				{
					if (holding->getType() == ItemType::BOWL)
					{
						if (!stoveLogic->isPotFilled()) return;
						holding->deleteObject();
						ItemType temp = stoveLogic->moveSoup();
						Entity* newObject = makeObject(temp);
						holding->setObject(std::make_pair(newObject->entityID, temp));
						type = holding->getType();
						SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
						if (soundToPlay) {
							soundToPlay->Stop();
							soundToPlay->Play(-1);
						}
						isHolding = true;
					}
				}
			}

			return;
		}
		
	}

}

/****************************************************************
* @brief Function to handle when player hold a key
* ****************************************************************/
void PlayerLogic::InteractHold()
{
	if (DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SoundComponent>(interactObject->entityID)) {
		sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(interactObject->entityID);
	}
	// Hold down is for Cutting/Cooking
	// Player Must Not be Holding Anything
	if (!isHolding)
	{
		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{
			// Something on the board
			if (chopBoardLogic->isOccupied && dir == FRONT)
			{
				// If object is already chopped, do nothing
				if (chopBoardLogic->isChopped) return;

				chopBoardLogic->chopObject();
				if (animator)
				{
					if (sound) sound->Play();
					animator->PlayAnimation("CHOP");
				}
			}

			return;
		}
	}
}

/****************************************************************
* @brief Function that makes a new object based on the type
*
* @param type - the type of object to be made
*
* @return - Pointer to the object
* ****************************************************************/
Entity* PlayerLogic::makeObject(ItemType type)
{
	Entity* newObject = nullptr;
	newObject = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity(circleCollider->getCenter() + offSet, Vec2{ 1.5f, 1.5f });
	SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(newObject->entityID, true);
	spriteRenderer->sortingOrder = 19;

	spriteRenderer->texture = AssetManager::GetTextureByName(whatType(type));

	return newObject;
}


/****************************************************************
* @brief Function to set the restock menu
* ****************************************************************/
void PlayerLogic::setRestockMenu(bool state)
{
	restockLogic->RestockMenu(state);
}