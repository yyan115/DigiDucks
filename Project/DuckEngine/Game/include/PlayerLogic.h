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

#include "IngredientType.h"
#include "StockLogic.h"
#include "TableLogic.h"
#include "HoldingLogic.h"
#include "PanLogic.h"
#include "ChopBoardLogic.h"


class PlayerLogic : public GameLogic
{
private:
    BoundingCircle* circleCollider;
    BoundingBox* boxCollider;
    AnimatorComponent* animator;

    Entity* interactObject;


public:
    bool isHolding = false;
	Vec2 offSet = Vec2{ 0.f, 1.5f };

    PlayerLogic()
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), animator(nullptr), interactObject(nullptr) {}

    PlayerLogic(GameLogicComponent* component)
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), animator(nullptr), interactObject(nullptr)
    {
        UNREFERENCED_PARAMETER(component);
    }

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<PlayerLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void InteractPressed();

	void InteractHold();

    Entity* makeObject(ItemType type);    
};