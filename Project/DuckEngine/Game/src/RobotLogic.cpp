/******************************************************************************/
/*!
\file       RobotLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Feb 04 2025
\brief      Declaration of Robot Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "RobotLogic.h"

void RobotLogic::Start()
{
	robot = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	robotAni = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());

	if (robot)
	{
		if (robot->childEntities.size() > 0)
		{
			for (int i = 0; i < robot->childEntities.size(); i++)
			{
				sliderLogic = GameLogicManager::GetLogicForEntity<SliderLogic>(robot->childEntities[i]->entityID);
				if (sliderLogic)
					break;
			}
		}

		auto restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
		if (restockMenu)
		{
			restockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(restockMenu->entityID);
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
	if (restockLogic && sliderLogic)
	{
		if (restockLogic->isDelay)
		{
			if (sliderLogic)
				sliderLogic->EnableSlider(true);
		}
		else
		{
			if (sliderLogic)
				sliderLogic->ResetSlider();
		}
	}
}