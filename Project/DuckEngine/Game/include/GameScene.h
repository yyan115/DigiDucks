/******************************************************************************/
/*!
\file       GameScene.h
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
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "SubmitLogic.h"
#include "SpriteRendererComponent.h"
#include "SoundSystem.h"

class GameScene : public Scene
{
public:
    void Load() override;
    void Start() override;
    void Update() override;
    void PostUpdate() override;
    void Exit() override;
    void Unload() override;

private:
    // Helper functions
    void UpdateOrderTexture();
    void PauseGame(bool state);
    void HTPShow(bool state);
    void ExitConfirm(bool state);
    void changePage();
    void MiniGame_1(bool state);

    // Member variables
    Entity* duck = nullptr;
    TransformComponent* duckTrans = nullptr;
    SoundComponent* TimeLeftSound = nullptr;

    Entity* OrderTab = nullptr;
    SpriteRendererComponent* orderSprite = nullptr;
    Entity* timer = nullptr;
    TextComponent* timerText = nullptr;
    TextComponent* FPSText = nullptr;
    float timeLeft = 0.f;

    Entity* score = nullptr;
    TextComponent* scoreText = nullptr;

    // Pause Menu
    Entity* gamePauseBg = nullptr;
    SpriteRendererComponent* gamePauseBgSpt = nullptr;
    Entity* gameResumeBtn = nullptr;
    ButtonComponent* gameResumeButton = nullptr;
    Entity* gameExitBtn = nullptr;
    ButtonComponent* gameExitButton = nullptr;
    Entity* gameHTPBtn = nullptr;
    ButtonComponent* gameHTPButton = nullptr;

    // HTP Menu
    Entity* gameJournal = nullptr;
    SpriteRendererComponent* gameJournalSpt = nullptr;
    Entity* gameHTPExitBtn = nullptr;
    ButtonComponent* gameHTPExitButton = nullptr;
    Entity* gameHTPBackBtn = nullptr;
    ButtonComponent* gameHTPBackButton = nullptr;
    Entity* gameHTPNextBtn = nullptr;
    ButtonComponent* gameHTPNextButton = nullptr;

    // Exit Confirm
    Entity* gameExitCfmBg = nullptr;
    SpriteRendererComponent* gameExitCfmBgSpt = nullptr;
    Entity* gameExitCfmTxt = nullptr;
    TextComponent* gameExitCfmText = nullptr;
    Entity* gameExitYesBtn = nullptr;
    ButtonComponent* gameExitYesButton = nullptr;
    Entity* gameExitNoBtn = nullptr;
    ButtonComponent* gameExitNoButton = nullptr;

    // Restock Menu
    Entity* gameRestockMenu = nullptr;
    SpriteRendererComponent* gameRestockMenuSpt = nullptr;
    Entity* gameRestockExitBtn = nullptr;
    ButtonComponent* gameRestockExitButton = nullptr;

    // MiniGame_1
    Entity* gameMiniGame_BG = nullptr;
    SpriteRendererComponent* gameMiniGame_BG_Spt = nullptr;
    Entity* gameMiniGame_Keypad = nullptr;
    SpriteRendererComponent* gameMiniGame_Keypad_Spt = nullptr;
    Entity* gameMiniGame_K1 = nullptr;
    SpriteRendererComponent* gameMiniGame_K1_Spt = nullptr;
    ButtonComponent* gameMiniGame_K1_Btn = nullptr;
    Entity* gameMiniGame_K2 = nullptr;
    SpriteRendererComponent* gameMiniGame_K2_Spt = nullptr;
    ButtonComponent* gameMiniGame_K2_Btn = nullptr;
    Entity* gameMiniGame_T1 = nullptr;
    TextComponent* gameMiniGame_T1_Txt = nullptr;
    Entity* gameMiniGame_T2 = nullptr;
    TextComponent* gameMiniGame_T2_Txt = nullptr;
    Entity* gameMiniGame_Text = nullptr;
    TextComponent* gameMiniGame_Text_Txt = nullptr;
    Entity* gameMiniGame_K3 = nullptr;
    SpriteRendererComponent* gameMiniGame_K3_Spt = nullptr;
    ButtonComponent* gameMiniGame_K3_Btn = nullptr;
    Entity* gameMiniGame_K4 = nullptr;
    SpriteRendererComponent* gameMiniGame_K4_Spt = nullptr;
    ButtonComponent* gameMiniGame_K4_Btn = nullptr;
    Entity* gameMiniGame_T3 = nullptr;
    TextComponent* gameMiniGame_T3_Txt = nullptr;
    Entity* gameMiniGame_T4 = nullptr;
    TextComponent* gameMiniGame_T4_Txt = nullptr;
    Entity* gameMiniGame_K5 = nullptr;
    SpriteRendererComponent* gameMiniGame_K5_Spt = nullptr;
    ButtonComponent* gameMiniGame_K5_Btn = nullptr;
    Entity* gameMiniGame_K6 = nullptr;
    SpriteRendererComponent* gameMiniGame_K6_Spt = nullptr;
    ButtonComponent* gameMiniGame_K6_Btn = nullptr;
    Entity* gameMiniGame_T5 = nullptr;
    TextComponent* gameMiniGame_T5_Txt = nullptr;
    Entity* gameMiniGame_T6 = nullptr;
    TextComponent* gameMiniGame_T6_Txt = nullptr;
    Entity* gameMiniGame_K7 = nullptr;
    SpriteRendererComponent* gameMiniGame_K7_Spt = nullptr;
    ButtonComponent* gameMiniGame_K7_Btn = nullptr;
    Entity* gameMiniGame_K8 = nullptr;
    SpriteRendererComponent* gameMiniGame_K8_Spt = nullptr;
    ButtonComponent* gameMiniGame_K8_Btn = nullptr;
    Entity* gameMiniGame_T7 = nullptr;
    TextComponent* gameMiniGame_T7_Txt = nullptr;
    Entity* gameMiniGame_T8 = nullptr;
    TextComponent* gameMiniGame_T8_Txt = nullptr;
    Entity* gameMiniGame_K9 = nullptr;
    SpriteRendererComponent* gameMiniGame_K9_Spt = nullptr;
    ButtonComponent* gameMiniGame_K9_Btn = nullptr;
    Entity* gameMiniGame_K10 = nullptr;
    SpriteRendererComponent* gameMiniGame_K10_Spt = nullptr;
    ButtonComponent* gameMiniGame_K10_Btn = nullptr;
    Entity* gameMiniGame_T9 = nullptr;
    TextComponent* gameMiniGame_T9_Txt = nullptr;
    Entity* gameMiniGame_T10 = nullptr;
    TextComponent* gameMiniGame_T10_Txt = nullptr;

    // Other variables
    int pageNumb = 1;
    bool isPaused = false;
    bool hasStartedFade = false;
    float GamefadeElapsedTime = 0.f;
};
