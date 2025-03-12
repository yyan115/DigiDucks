/******************************************************************************/
/*!
\file       GameScene.h
\author     Ernest Ho, h.yonghengernest, 2301223 (50%)
\par        h.yonghengernestt@digipen.edu
\author     Jovan Chua, c.shengkaijovan, 2301244 (25%)
\par        c.shengkaijovan@digipen.edu
\author     Tan Yan Kai, yankai.tan, 2301312 (25%)
\par        yankai.tan@digipen.edu
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
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "SubmitLogic.h"
#include "SpriteRendererComponent.h"
#include "SoundSystem.h"
#include "RestockLogic.h"
#include "CustomerLogic.h"
#include "PauseMenuLogic.h"
#include "GamesettingsLogic.h"
#include "MiniGameLogic.h"

class GameScene : public Scene
{
public:
    void Load() override;
    void Start() override;
    void Update() override;
    void PostUpdate() override;
    void Exit() override;
    void Unload() override;


};
