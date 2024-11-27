#include "PanLogic.h"

void PanLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());

	makeEmptyPan();
}

void PanLogic::Update()
{
	if (isCooked)
	{
		switch (type)
		{
		case IngredientType::R_PATTY:
			objectSprite->texture = AssetManager::GetTextureByName("fryingpan_cooked");
			type = IngredientType::C_PATTY;
			break;
		default: 
			break;
		};
	}

}

void PanLogic::FixedUpdate()
{
}

void PanLogic::setObject(std::pair<int, IngredientType> objData)
{
	// Delete Empty Pan Object
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(object->entityID);

	// Assign new object
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	objectSprite->texture = AssetManager::GetTextureByName("fryingpan_raw");

	type = objData.second;
	isOccupied = true;
	isCooked = false;
	cookTime = 2.f;
}

std::pair<int, IngredientType> PanLogic::moveObject()
{
	int objectID = object->entityID;
	IngredientType temp = type;

	if (type == IngredientType::R_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("patty");
	}
	else if (type == IngredientType::C_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("cooked_patty");
	}

	makeEmptyPan();

	return std::make_pair(objectID, temp);
}

void PanLogic::cookObject()
{
	cookTime -= DuckEngine::DeltaTime() / 3.f;
	if (cookTime <= 0.f)
	{
		isCooked = true;
	}
}

// Make the pan empty
void PanLogic::makeEmptyPan()
{
	object = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity();

	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(object->entityID);
	objectTransform->SetPosition(tableTransform->GetPosition());
	objectTransform->scale = Vec2(1.5f, 1.5f);

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(object->entityID);
	objectSprite->texture = AssetManager::GetTextureByName("fryingpan");

	isOccupied = false;
	isCooked = false;

	type = IngredientType::EMPTY;
	cookTime = 2.f;
}