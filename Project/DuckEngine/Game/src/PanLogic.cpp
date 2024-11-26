#include "PanLogic.h"

void PanLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = IngredientType::EMPTY;
	cookTime = 3.f;
	isOccupied = false;

}

void PanLogic::Update()
{
}

void PanLogic::FixedUpdate()
{
}

void PanLogic::setObject(std::pair<int, IngredientType> objData)
{
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	type = objData.second;
	isOccupied = true;
}

std::pair<int, IngredientType> PanLogic::moveObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	isOccupied = false;

	IngredientType temp = type;
	type = IngredientType::EMPTY;

	return std::make_pair(objectID, temp);
}

void PanLogic::cookObject()
{
	cookTime -= DuckEngine::DeltaTime() / 3.f;

}