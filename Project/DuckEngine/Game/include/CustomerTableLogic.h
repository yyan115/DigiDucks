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