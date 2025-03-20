/******************************************************************************/
/*!
\file       HoldingLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Holding Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"

class HoldingLogic : public GameLogic
{
private:
    Entity* holder;
    TransformComponent* holderTransform;
    Entity* object;
	TransformComponent* objectTransform;
	ItemType type;

public:
    Vec2 offSet{ 0.f, 1.5f };

    HoldingLogic() :
        GameLogic(nullptr), holder(nullptr), holderTransform(nullptr), object(nullptr), objectTransform(nullptr), type(ItemType::EMPTY) {}

    HoldingLogic(GameLogicComponent* component, Entity* holder_) :
        GameLogic(nullptr), holder(holder_), holderTransform(nullptr), object(nullptr), objectTransform(nullptr), type(ItemType::EMPTY)
    {
        UNREFERENCED_PARAMETER(component);
    }
    
    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<HoldingLogic>(*this);
        clone->component = nullptr;
        return clone;
    }


    /****************************************************************
	* @brief Start function for HoldingLogic
    * ****************************************************************/
    void Start() override;

	/****************************************************************
	* @brief Update function for HoldingLogic
	* ****************************************************************/
    void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for HoldingLogic
	* ****************************************************************/
    void FixedUpdate() override;

	/****************************************************************
	* @brief setObject function for HoldingLogic
    * 
	* @param objData - pair of object ID and ItemType
	* ****************************************************************/
    void setObject(std::pair<int, ItemType> objData);

	/****************************************************************
	* @brief deleteObject function for HoldingLogic
	* ****************************************************************/
	void deleteObject();
    
	/****************************************************************
	* @brief moveObject function for HoldingLogic
    * 
	* @return pair of object ID and ItemType
	* ****************************************************************/
    std::pair<int, ItemType> moveObject();

	/****************************************************************
	* @brief getType function for HoldingLogic
    * 
	* @return ItemType - type of object
	* ****************************************************************/
	ItemType getType() const { return type; }

	/****************************************************************
	* @brief Get the object ID
	* ****************************************************************/
	int getObjectID() const { return object->entityID; }
};