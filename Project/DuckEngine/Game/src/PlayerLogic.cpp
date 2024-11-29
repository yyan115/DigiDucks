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

float actionCooldown = 0.5f;
float actionCounter = 0.5f;

/****************************************************************
* @brief Start function for the Player Logic
* ****************************************************************/
void PlayerLogic::Start()
{
	circleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(component->GetEntityID());
	boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(component->GetEntityID());
	if (boxCollider)
	{
		boxCollider->isKinematic = false;
		boxCollider->SetCollisionCallback([this](int otherEntityID)
			{
				interactObject = DuckEngine::DUCKENGINE_EntityManager.GetEntity(otherEntityID);
				// Pickup Object
				if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_R) && actionCounter <= 0)
				{
					InteractPressed();
					actionCounter = actionCooldown;
				}
				// Use Object
				else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_T))
				{
					InteractHold();
				}
			});
	}
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
	dir = FRONT;
	isHolding = false;

	actionCounter = actionCooldown;
}


/****************************************************************
* @brief Update function for the Player Logic
* ****************************************************************/
void PlayerLogic::Update()
{
	
	if (actionCounter >= 0)
	{
		actionCounter -= DuckEngine::DeltaTime();
	}
	if (animator)
	{
		if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_D)
			|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_A)
			|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_S)
			|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_W)
			|| DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_T))
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

/****************************************************************
* @brief FixedUpdate function for the Player Logic
* ****************************************************************/
void PlayerLogic::FixedUpdate()
{
	auto playersound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(component->GetEntityID());
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, 1.5f);
		}
		if (animator)
		{
			playersound->PlayHold();
			animator->PlayAnimation("BACK_WALK");
			dir = BACK;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, -1.5f);
		}
		if (animator)
		{
			playersound->PlayHold();
			animator->PlayAnimation("FRONT_WALK");
			dir = FRONT;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(-1.5f, 0.f);
		}
		if (animator)
		{
			playersound->PlayHold();
			animator->PlayAnimation("LEFT_WALK");
			dir = LEFT;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(1.5f, 0.f);
		}
		if (animator)
		{
			playersound->PlayHold();
			animator->PlayAnimation("RIGHT_WALK");
			dir = RIGHT;
		}
	}


	// Cheats
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_B))
	{
		if (!isHolding) {
			ItemType cheatType = ItemType::SALAD_PLATE;
			Entity* newObject = makeObject(cheatType);
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->setObject(std::make_pair(newObject->entityID, cheatType));
			isHolding = true;
		}
	}
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_N))
	{
		if (!isHolding) {
			ItemType cheatType = ItemType::CHEESE_BURGER_PLATE;
			Entity* newObject = makeObject(cheatType);
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->setObject(std::make_pair(newObject->entityID, cheatType));
			isHolding = true;
		}
	}

}


/****************************************************************
* @brief Function to handle when player press a key
* ****************************************************************/
void PlayerLogic::InteractPressed()
{
	if (DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SoundComponent>(interactObject->entityID)) {
		sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(interactObject->entityID);
	}

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
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->setObject(std::make_pair(newObject->entityID, stockLogic->getType()));
			if (sound) sound->Play();
			isHolding = true;
			return;
		}

		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(interactObject->entityID);
		if (tableLogic)
		{
			// If Table is occupied, take object from table
			if (tableLogic->isOccupied)
			{
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				
				holdingLogic->setObject(tableLogic->moveObject());
				if (sound) sound->Play();
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
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				holdingLogic->setObject(chopBoardLogic->moveObject());
				isHolding = true;
			}
			return;
		}

		auto panLogic = GameLogicManager::GetLogicForEntity<PanLogic>(interactObject->entityID);
		if (panLogic)
		{
			// If Pan is occupied, take object from pan
			if (panLogic->isOccupied)
			{
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				holdingLogic->setObject(panLogic->moveObject());
				isHolding = true;
			}
			return;
		}

		auto restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(interactObject->entityID);
		if (restockLogic)
		{
			if (sound) sound->Play();
			restockLogic->restockAll();
			return;
		}

	}
	else if (isHolding) // If player is already holding something
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
		if (stockLogic)
		{
			// If Object is Not BIN, do nothing
			if(stockLogic->getType() != ItemType::BIN) return;
			// If Object is BIN, Destroy Object
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->deleteObject();
			if (sound) sound->Play();
			isHolding = false;
			return;
		}

		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(interactObject->entityID);
		if (tableLogic)
		{
			// If Table is occupied, take object from table
			if (!tableLogic->isOccupied)
			{
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				tableLogic->setObject(holdingLogic->moveObject());
				isHolding = false;
			}
			else if (tableLogic->isOccupied)
			{
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				if (canCombine(holdingLogic->getType(), tableLogic->getType()))
				{
					std::pair<int, ItemType> combined = combineObjects(holdingLogic->moveObject(), tableLogic->moveObject());
					holdingLogic->setObject(combined);
					isHolding = true;
				}
			}
			return;
		}

		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{
			if (!chopBoardLogic->isOccupied) {
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				chopBoardLogic->setObject(holdingLogic->moveObject());
				isHolding = false;
			}
			return;
		}

		auto panLogic = GameLogicManager::GetLogicForEntity<PanLogic>(interactObject->entityID);
		if (panLogic)
		{
			if (!panLogic->isOccupied) {
				auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
				if (holdingLogic->getType() != ItemType::R_PATTY) return;

				panLogic->setObject(holdingLogic->moveObject());
				isHolding = false;
			}
			return;
		}

		auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(interactObject->entityID);
		if (submitLogic)
		{
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			if (holdingLogic->getType() == ItemType::SALAD_PLATE || holdingLogic->getType() == ItemType::CHEESE_BURGER_PLATE)
			{
				submitLogic->removeObject(holdingLogic->moveObject());
				isHolding = false;
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
			if (chopBoardLogic->isOccupied)
			{				
				chopBoardLogic->chopObject();
				if (animator)
				{
					if (sound) sound->PlayHold();
					animator->PlayAnimation("CHOP");					
				}
			}

			return;
		}

		auto panLogic = GameLogicManager::GetLogicForEntity<PanLogic>(interactObject->entityID);
		if (panLogic)
		{
			if (panLogic->isOccupied)
			{
				panLogic->cookObject();
				if (animator)
				{
					if (sound) sound->PlayHold();
					//animator->PlayAnimation("Cook");
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
	spriteRenderer->sortingOrder = 3;

	switch (type)
	{
	case ItemType::BUN:
		spriteRenderer->texture = AssetManager::GetTextureByName("bun");
		break;
	case ItemType::CHEESE:
		spriteRenderer->texture = AssetManager::GetTextureByName("cheese");
		break;
	case ItemType::LETTUCE:
		spriteRenderer->texture = AssetManager::GetTextureByName("lettuce");
		break;
	case ItemType::MUSHROOM:
		spriteRenderer->texture = AssetManager::GetTextureByName("mushroom");
		break;
	case ItemType::SHRIMP:
		spriteRenderer->texture = AssetManager::GetTextureByName("shrimp");
		break;
	case ItemType::STEAK:
		spriteRenderer->texture = AssetManager::GetTextureByName("steak");
		break;
	case ItemType::TOMATO:
		spriteRenderer->texture = AssetManager::GetTextureByName("tomato");
		break;
	case ItemType::GREY_PLATE:
		spriteRenderer->texture = AssetManager::GetTextureByName("grey_plate");
		break;
	case ItemType::WHITE_PLATE:
		spriteRenderer->texture = AssetManager::GetTextureByName("white_plate");
		break;

	/*Cheats*/
	case ItemType::SALAD_PLATE:
		spriteRenderer->texture = AssetManager::GetTextureByName("salad_plate");
		break;
	case ItemType::CHEESE_BURGER_PLATE:
		spriteRenderer->texture = AssetManager::GetTextureByName("cheese_burger_plate");
		break;
	};

	return newObject;
}