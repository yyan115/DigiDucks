/******************************************************************************/
/*!
\file       PlayerLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
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

float actionCooldown = 0.5f;
float actionCounter = 0.5f;

/****************************************************************
* @brief Start function for the Player Logic
* ****************************************************************/
void PlayerLogic::Start()
{
	circleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(component->GetEntityID());
	boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
	holding = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
	movement = GameLogicManager::GetLogicForEntity<MovementLogic>(component->GetEntityID());

	Entity* orderTabEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab").get();
	if(orderTabEntity)
		orderTabLogic = GameLogicManager::GetLogicForEntity<OrderTabLogic>(orderTabEntity->entityID).get();

	auto restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(restockMenu->entityID);
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
				// Pickup Object
				if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_J) && actionCounter <= 0)
				{
					InteractPressed();
					actionCounter = actionCooldown;
				}
				// Use Object
				else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_K))
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
	if (animator)
	{
		if (movement->isMoving)
		{
			if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_D)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_A)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_S)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_W)
				|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_K))
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

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
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
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
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
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
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
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
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
			// If empty stock or Bin, do nothing
			if (stockLogic->getType() == ItemType::BIN || stockLogic->getType() == ItemType::EMPTY) return;

			// If stock is not empty, create object and set holding
			if (stockLogic->isEmpty()) return;
			stockLogic->useStock();

			Entity* newObject = makeObject(stockLogic->getType());
			holding->setObject(std::make_pair(newObject->entityID, stockLogic->getType()));
			if (sound) sound->Play(-1);
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
					soundToPlay->Play(-1);
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
					soundToPlay->Play(-1);
				}
				isHolding = true;
			}
			return;
		}

		auto robotLogic = GameLogicManager::GetLogicForEntity<RobotLogic>(interactObject->entityID);
		if (robotLogic)
		{
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
			// If Object is Not BIN, do nothing
			if(stockLogic->getType() != ItemType::BIN) return;
			// If Object is BIN, Destroy Object
			holding->deleteObject();
			if (sound) sound->Play(-1);
			isHolding = false;
			return;
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
					soundToPlay->Play(-1);
				}
				isHolding = false;
			}
			else if (tableLogic->isOccupied)
			{
				if (canCombine(holding->getType(), tableLogic->getType()))
				{
					std::pair<int, ItemType> combined = combineObjects(holding->moveObject(), tableLogic->moveObject());
					holding->setObject(combined);
					type = holding->getType();
					SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
					if (soundToPlay) {
						soundToPlay->Play(-1);
					}
					isHolding = true;
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
				if (holding->getType() != ItemType::PAN && holding->getType() != ItemType::POT) return;
				type = holding->getType();
				SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
				if (soundToPlay) {
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
						soundToPlay->Play(-1);
					}
					stoveLogic->setObject(holding->moveObject());
					isHolding = false;
				}
				else if(stoveLogic->isOccupied)
				{
					if (holding->getType() == ItemType::BOWL)
					{
						holding->deleteObject();
						ItemType temp = stoveLogic->moveSoup();
						Entity* newObject = makeObject(temp);
						holding->setObject(std::make_pair(newObject->entityID, temp));
						type = holding->getType();
						SoundComponent* soundToPlay = GetSFXForType(static_cast<int>(type));
						if (soundToPlay) {
							soundToPlay->Play(-1);
						}
						isHolding = true;
					}
				}
			}

			return;
		}

		auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(interactObject->entityID);
		if (submitLogic)
		{
			if (DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName() == "Level0")
			{
				if (holding->getType() == ItemType::LETTUCE_PLATE)
				{
					submitLogic->removeObject(holding->moveObject());
					if (sound) sound->Play();
					isHolding = false;
				}
			}
			else
			{
				if (orderTabLogic) {
					if ((holding->getType() == orderTabLogic->GetCurrentOrder()) && orderTabLogic->GetCurrentCustomer()->WalkState->GetIsWaitingToCollectOrder())
					{
						orderTabLogic->GetCurrentCustomer()->OrderCompleted();
						submitLogic->removeObject(holding->moveObject());
						if (sound) sound->Play();
						isHolding = false;
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
	
	// Hold down is for Cutting/Cooking
	// Player Must Not be Holding Anything
	if (!isHolding)
	{
		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{
			// Something on the board
			if (chopBoardLogic->isOccupied)
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


		// Patty Auto Cooks
		//auto panLogic = GameLogicManager::GetLogicForEntity<PanLogic>(interactObject->entityID);
		//if (panLogic)
		//{
		//	if (panLogic->isOccupied)
		//	{
		//		panLogic->cookObject();
		//		if (animator)
		//		{
		//			if (sound) sound->Play();
		//			//animator->PlayAnimation("Cook");

		//			panLogic->EmitSparks();
		//			//	// Then inside your stove/cooking system:
		//			//if (foodIsCooking && someRandomChance())
		//			//{
		//			//	g_particleManager.Emit(ParticleType::CookingSparks, stovePos, { 0,0 });
		//			//	DuckEngine::Emit("CookingSparks", , {0.2f, 0.2f});
		//			//}
		//		}
		//	}

		//	return;
		//}

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
	spriteRenderer->sortingOrder = 7;

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