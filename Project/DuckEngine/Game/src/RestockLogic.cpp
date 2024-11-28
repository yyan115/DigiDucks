#include "RestockLogic.h"
#include "StockLogic.h"
#include "GameLogicManager.h"

void RestockLogic::Start()
{
	restockStation = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	restockTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
}

void RestockLogic::restockAll()
{
    std::vector<int> allEntities = GameLogicManager::GetAllEntitiesWithLogic<StockLogic>();

	for (int entityID : allEntities)
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(entityID);
		stockLogic->restock();
	}
}
