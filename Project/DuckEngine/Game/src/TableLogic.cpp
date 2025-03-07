/******************************************************************************/
/*!
\file       TableLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 26 2024
\brief      Definition of all Table logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "TableLogic.h"

/****************************************************************
* @brief Start function for the Table Logic.
* ****************************************************************/
void TableLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());

	objectOnTable = nullptr;
	objectTransform = nullptr;
	isOccupied = false;

}

/****************************************************************
* @brief Shutdown function for the Table Logic.
*
* @param objData - The object data to set.
* ****************************************************************/
void TableLogic::setObject(std::pair<int, ItemType> objData) {

    if (objData.first < 0) {
        std::cerr << "Attempted to set invalid object ID: " << objData.first << std::endl;
        return;
    }

    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first).get();
    if (!entity) {
        std::cerr << "Entity not found for ID: " << objData.first << std::endl;
        return;
    }

    std::cout << "Table Object ID: " << objData.first << " Type: " << whatType(objData.second) << std::endl;
    objectOnTable = entity;
    objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
    if (objectTransform) {
        if (isIngredient(objData.second))
            objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.3f));
        else
            objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.6f));
    }
    type = objData.second;
    isOccupied = true;
}

/****************************************************************
* @brief Move the object from the table.
*
* @return The object data to move.
* ****************************************************************/
std::pair<int, ItemType> TableLogic::moveObject() {
    std::cout << "Moving Object from Table";
    if (!objectOnTable) {
        std::cout << "OBJ is NULLPTR" << std::endl;
        return std::pair<int, ItemType>();
    }

    int objectID = objectOnTable->entityID;
    if (objectID < 0 || !DuckEngine::DUCKENGINE_EntityManager.GetEntity(objectID)) {
        std::cout << "Invalid entity ID: " << objectID << std::endl;
        objectOnTable = nullptr;
        objectTransform = nullptr;
        isOccupied = false;
        type = ItemType::EMPTY;
        return std::pair<int, ItemType>();
    }

    std::cout << "Object ID: " << objectOnTable->entityID << " Type: " << whatType(type) << std::endl;

    ItemType tempType = type;

    objectOnTable = nullptr;
    objectTransform = nullptr;
    isOccupied = false;
    type = ItemType::EMPTY;

    return std::make_pair(objectID, tempType);
}


/****************************************************************
* @brief Find and Set the Pan on the table.
* ****************************************************************/
void TableLogic::setPan()
{
	objectOnTable = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pan").get();
    if (objectOnTable)
    {
		objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectOnTable->entityID);
        if (objectTransform)
        {
            objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.6f));
	        type = ItemType::PAN;
	        isOccupied = true;
        }
    }
    else
    {
		std::cout << "Pan not found" << std::endl;
    }
}

/****************************************************************
* @brief Find and Set the Pot on the table.
* ****************************************************************/
void TableLogic::setPot() 
{
    objectOnTable = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pot").get();
	if (objectOnTable)
	{
		objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectOnTable->entityID);
		if (objectTransform)
		{
			objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.6f));
			type = ItemType::POT;
			isOccupied = true;
		}
	}
    else
    {
        std::cout << "Pot not found" << std::endl;
    }
}