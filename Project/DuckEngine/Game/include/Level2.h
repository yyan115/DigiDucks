/******************************************************************************/
/*!
\file       Level0.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Definition of the GameScene class, which handles the game logic
            for the main game scene. This includes loading assets, initializing
            game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Scene.h"
#include "CutSceneLogic.h"
#include "GameLoopLogic.h"


class Level2 : public Scene
{
public:
    void Load() override;
    void Start() override;
    void Update() override;
    void PostUpdate() override;
    void Exit() override;
    void Unload() override;

};
