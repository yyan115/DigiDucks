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