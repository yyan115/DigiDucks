#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "TableLogic.h"


// This Logic is Only for Entity with TableLogic
// This Logic is used to find the Pot and set it to TableLogic Item
class GetPotLogic : public GameLogic
{
private:

public:

	GetPotLogic() : GameLogic(nullptr) {}

	GetPotLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}


	/****************************************************************
	* @brief Start function for the Score Logic.
	* ****************************************************************/
	void Start() override
	{

		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(component->GetEntityID());

		if (tableLogic)
		{
			tableLogic->setPot();
		}
		else
		{
			std::cout << "Table Logic Not Found" << std::endl;
		}
	}

	/****************************************************************
	* @brief Update function for the Score Logic.
	* ****************************************************************/
	void Update() override {}

	/****************************************************************
	* @brief Fixed Update function for the Score Logic.
	* ****************************************************************/
	void FixedUpdate() override {}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<GetPotLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};