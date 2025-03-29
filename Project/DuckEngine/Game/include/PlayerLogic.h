/******************************************************************************/
/*!
\file       PlayerLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Player Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>

#include "MovementLogic.h"
#include "IngredientType.h"
#include "StockLogic.h"
#include "TableLogic.h"
#include "HoldingLogic.h"
#include "StoveLogic.h"
#include "ChopBoardLogic.h"
#include "RobotLogic.h"
#include "RestockLogic.h"
#include "OrderTabLogic.h"

enum FacingDirection : int
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    CHOP,
    COOK
};

class PlayerLogic : public GameLogic
{
private:
    OrderTabLogic* orderTabLogic = nullptr;
    BoundingCircle* circleCollider = nullptr;
    BoundingBox* boxCollider = nullptr;
    AnimatorComponent* animator = nullptr;
    FacingDirection dir = FRONT;
    Entity* interactObject = nullptr;
    SoundComponent* sound = nullptr;
    SoundComponent* SFXsound = nullptr;
	SoundComponent* PlateSFXsound = nullptr;
	SoundComponent* BowlSFXsound = nullptr;
	SoundComponent* PanSFXsound = nullptr;
	SoundComponent* PotSFXsound = nullptr;
	std::shared_ptr<HoldingLogic> holding = nullptr;
	std::shared_ptr<MovementLogic> movement = nullptr;
	std::shared_ptr<RestockLogic> restockLogic = nullptr;


public:
    bool isHolding = false;
    bool isIdle = false;
	Vec2 offSet = Vec2{ 0.f, 1.5f };

    PlayerLogic() : GameLogic(nullptr) {}

    PlayerLogic(GameLogicComponent* component) : GameLogic(nullptr)
    {
        UNREFERENCED_PARAMETER(component);
    }

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<PlayerLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    /****************************************************************
	* @brief Start function for the Player Logic
    * ****************************************************************/
    void Start() override;

	/****************************************************************
	* @brief Update function for the Player Logic
	* ****************************************************************/
    void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the Player Logic
	* ****************************************************************/
    void FixedUpdate() override;

	/****************************************************************
	* @brief Function to handle when player press a key
	* ****************************************************************/
    void InteractPressed();

	/****************************************************************
	* @brief Function to handle when player hold a key
	* ****************************************************************/
	void InteractHold();

	/****************************************************************
	* @brief Function that makes a new object based on the type
    * 
	* @param type - the type of object to be made
    * 
	* @return - Pointer to the object
	* ****************************************************************/
    Entity* makeObject(ItemType type);

    
	/****************************************************************
	* @brief Function that handles the state of the restock menu
	* ****************************************************************/
    void setRestockMenu(bool state);

    SoundComponent* GetSFXForType(int type);
};