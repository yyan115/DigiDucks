/******************************************************************************/
/*!
\file       MovementLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Movement Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"


class MovementLogic : public GameLogic
{
private:
    TransformComponent* transform;
    RigidbodyComponent* rigidbody;
    AnimatorComponent* animator;
	BoundingCircle* circleCollider;
    float moveSpeed;

public:
    MovementLogic()
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(10.0f) {}

    MovementLogic(GameLogicComponent* component, float speed = 10.0f)
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(speed)
    {
        UNREFERENCED_PARAMETER(component);
    }

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<MovementLogic>(*this);
        clone->component = nullptr;
        return clone;
    }


    /****************************************************************
	* @brief Start function for the Movement Logic
    * ****************************************************************/
    void Start() override;

	/****************************************************************
	* @brief Update function for the Movement Logic
	* ****************************************************************/
    void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the Movement Logic
	* ****************************************************************/
    void FixedUpdate() override;
};
