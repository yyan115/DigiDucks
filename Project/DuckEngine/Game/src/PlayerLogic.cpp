#include "PlayerLogic.h"
#include <vector>


void PlayerLogic::Start()
{
	circleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(component->GetEntityID());
	boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(component->GetEntityID());
	boxCollider->isKinematic = false;
	boxCollider->SetCollisionCallback([this](int otherEntityID)
		{
			interactObject = DuckEngine::DUCKENGINE_EntityManager.GetEntity(otherEntityID);

			// Pickup Object
			if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_R))
			{
				InteractPressed();
			}
			// Use Object
			else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_T)) 
			{
				InteractHold();
			}
		});
	isHolding = false;
}


void PlayerLogic::Update()
{
}

void PlayerLogic::FixedUpdate()
{
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, 1.f);
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(0.f, -1.f);
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(-1.f, 0.f);
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		if (boxCollider)
		{
			boxCollider->setOffSet(1.f, 0.f);
		}
	}
}


void PlayerLogic::InteractPressed()
{	
	// If player isnt holding anything
	if (!isHolding)
	{
		// Check if Stock Object
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
		if (stockLogic)
		{
			// If empty stock or Bin, do nothing
			if (stockLogic->getType() == IngredientType::BIN || stockLogic->getType() == IngredientType::EMPTY) return;

			// If stock is not empty, create object and set holding
			if (stockLogic->isEmpty()) return;
			stockLogic->useStock();

			Entity* newObject = makeObject(stockLogic->getType());
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->setObject(std::make_pair(newObject->entityID, stockLogic->getType()));
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
				isHolding = true;
				return;
			}
		}
	}
	else if (isHolding) // If player is already holding something
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
		if (stockLogic)
		{
			// If Object is Not BIN, do nothing
			if(stockLogic->getType() != IngredientType::BIN) return;
			// If Object is BIN, Destroy Object
			auto holdingLogic = GameLogicManager::GetLogicForEntity<HoldingLogic>(component->GetEntityID());
			holdingLogic->deleteObject();
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
				return;
			}
		}
	}
	
}

void PlayerLogic::InteractHold()
{
	// Hold down is for Cutting/Cooking
	// Player Must Not be Holding Anything
	if (!isHolding)
	{
		auto chopBoardLogic = GameLogicManager::GetLogicForEntity<ChopBoardLogic>(interactObject->entityID);
		if (chopBoardLogic)
		{

			return;
		}

		auto panLogic = GameLogicManager::GetLogicForEntity<PanLogic>(interactObject->entityID);
		if (panLogic)
		{

			return;
		}

	}
}

Entity* PlayerLogic::makeObject(IngredientType type) 
{
	Entity* newObject = nullptr;
	newObject = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity(circleCollider->getCenter() + offSet, Vec2{ 1.5f, 1.5f });
	SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(newObject->entityID, true);
	spriteRenderer->sortingOrder = 2;

	switch (type)
	{
	case IngredientType::BUN:
		spriteRenderer->texture = AssetManager::GetTextureByName("bun");
		break;
	case IngredientType::CHEESE:
		spriteRenderer->texture = AssetManager::GetTextureByName("cheese");
		break;
	case IngredientType::LETTUCE:
		spriteRenderer->texture = AssetManager::GetTextureByName("lettuce");
		break;
	case IngredientType::MUSHROOM:
		spriteRenderer->texture = AssetManager::GetTextureByName("mushroom");
		break;
	case IngredientType::SHRIMP:
		spriteRenderer->texture = AssetManager::GetTextureByName("shrimp");
		break;
	case IngredientType::STEAK:
		spriteRenderer->texture = AssetManager::GetTextureByName("steak");
		break;
	case IngredientType::TOMATO:
		spriteRenderer->texture = AssetManager::GetTextureByName("tomato");
		break;
	};

	return newObject;
}