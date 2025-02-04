#include "RobotLogic.h"

void RobotLogic::Start()
{
	robot = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	robotAni = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());

	restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		std::cout << "Restock Menu Object Found" << std::endl;
		restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(restockMenu->entityID);
	}
}

void RobotLogic::Update()
{
	if (robotAni)
	{
		robotAni->PlayAnimation("Idle");
	}
}

void RobotLogic::FixedUpdate()
{

}

void RobotLogic::RestockMenu(bool state)
{
	if (restockMenu && restockLogic)
	{
		std::cout << "Restock Menu" << std::endl;
		restockLogic->RestockMenu(state);
	}
}