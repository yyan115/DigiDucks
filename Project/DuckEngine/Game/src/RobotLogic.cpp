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