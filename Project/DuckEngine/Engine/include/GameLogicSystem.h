/******************************************************************************/
/*!
\file       GameLogicSystem.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Declares the GameLogicSystem class, which is responsible for
            managing the lifecycle of game logic components in the game engine.
            The system handles initialization, updates, and fixed updates
            of game logic objects for entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "System.h"

class GameLogicSystem : public System
{
public:
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
};
