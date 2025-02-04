#include "RobotLogic.h"

void RobotLogic::Start()
{
	robot = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	robotAni = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());

	restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		std::cout << "Restock Menu ID: " << restockMenu->entityID << std::endl;
		restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(restockMenu->entityID);
		if (restockLogic == nullptr)
		{
			std::cout << "Restock Logic Not Found" << std::endl;
		}
		else
		{
			std::cout << "Restock Logic Found and Entity Name is " << restockMenu->name << std::endl;
		}
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
	std::cout << "Restock Menu 1 " << std::endl;
	if (restockMenu && restockLogic)
	{
		std::cout << "Restock Menu 2 " << std::endl;
		restockLogic->RestockMenu(state);
	}
}