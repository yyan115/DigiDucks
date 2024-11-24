#include "PlayerLogic.h"

std::string currHolding;

void PlayerLogic::Start()
{
	// Initialize the components from the GameLogicComponent's owner entity
	circleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(component->GetEntityID());
	boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(component->GetEntityID());
	boxCollider->SetCollisionCallback([this](int otherEntityID)
		{
			interactObject = DuckEngine::DUCKENGINE_EntityManager.GetEntity(otherEntityID);
			if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_R))
			{
				isInteracting = true;
				if (isHolding) 
				{
					if (interactObject->name == currHolding || interactObject->name == "Bin_1" || interactObject->name == "Bin_2")
					{
						InteractPressed();
						isHolding = !isHolding;
					}
					else {
						std::cout << "Player is already holding " << currHolding << std::endl;
					}
				}
				else {
					InteractPressed();
					isHolding = !isHolding;
				}
			}
			else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_R)) 
			{
				isInteracting = true;
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


void PlayerLogic::InteractPressed() const
{
	if (isHolding)
	{
		std::cout << "Player is no longer holding " << currHolding << std::endl;;
	}
	else
	{
		currHolding = interactObject->name;
		std::cout << "Player is holding object " << currHolding << std::endl;
	}
}

void PlayerLogic::InteractHold() const
{

}