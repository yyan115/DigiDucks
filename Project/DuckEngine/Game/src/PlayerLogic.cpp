#include "PlayerLogic.h"
#include <vector>

std::string currHolding;

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
				//isInteracting = true;
				InteractPressed();
			}
			else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_R)) 
			{
				//isInteracting = true;
				InteractHold();
			}
		});
}


void PlayerLogic::Update()
{
	if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_R))
	{
		//isInteracting = false;
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
	std::cout << "Interacting with: ";
	
	if(stockLogic)
	{
		switch (stockLogic->getType())
		{
		case IngredientType::BUN:
			std::cout << "Bun" << std::endl;
			break;
		case IngredientType::CHEESE:
			std::cout << "Cheese" << std::endl;
			break;
		case IngredientType::LETTUCE:
			std::cout << "Lettuce" << std::endl;
			break;
		case IngredientType::MUSHROOM:
			std::cout << "Mushroom" << std::endl;
			break;
		case IngredientType::SHRIMP:
			std::cout << "Shrimp" << std::endl;
			break;
		case IngredientType::STEAK:
			std::cout << "Steak" << std::endl;
			break;
		case IngredientType::TOMATO:
			std::cout << "Tomato" << std::endl;
			break;

		};
	}
	std::cout << '\n';
}

void PlayerLogic::InteractHold()
{

}