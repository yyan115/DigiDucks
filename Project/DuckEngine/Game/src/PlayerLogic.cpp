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
			if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_R))
			{
				isInteracting = true;
				InteractPressed();
			}
			else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_R)) 
			{
				InteractHold();
			}
		});
}


void PlayerLogic::Update()
{
	if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_R))
	{
		isInteracting = false;
	}
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
	auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(interactObject->entityID);
	if (stockLogic)
	{
		if (isHolding)
		{
			// If Same type of ingredient, or Bin, Put ingredient away.
			if (currHolding == stockLogic->getType()) {
				stockLogic->returnStock();
				currHolding = IngredientType::EMPTY;
			}
			else if (stockLogic->getType() == IngredientType::EMPTY)
			{
				currHolding = IngredientType::EMPTY;
			}
		}
		else
		{
			currHolding = stockLogic->getType();
			stockLogic->useStock();
		}
	}
}

void PlayerLogic::InteractHold()
{

}