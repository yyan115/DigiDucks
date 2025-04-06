/******************************************************************************/
/*!
\file       CustomerTableLogic.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       April 6 2025
\brief      Declares the CustomerTableLogic class, which manages logic related
			to a customer seated at a table. This logic component is attached
			to table entities and handles state references such as the linked
			customer or the seat assignment.

			Intended to be integrated with broader customer AI and game loop
			systems, allowing for expandable interaction logic such as serving,
			waiting, or leaving.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "DuckEngine.h"
#include "CustomerLogic.h"
#include "GameLogicComponent.h"

class CustomerTableLogic : public GameLogic {
public:
	//CustomerLogic* customer = nullptr;

	//GameLoopLogic* gameLoopLogic = nullptr;
	
	Entity* seatEntity = nullptr;

	CustomerTableLogic() {}

	void Start() override {
		//Entity* gameLogicManager = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
		//gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLogicManager->entityID).get();


	}

	void Update() override {}

	void FixedUpdate() override {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CustomerTableLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};