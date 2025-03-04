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

class GameScene : public Scene
{
public:
    void Load() override;
    void Start() override;
    void Update() override;
    void PostUpdate() override;
    void Exit() override;
    void Unload() override;
    
    bool IsGameStarted() { return gameStarted; }

	std::vector<CustomerLogic*> customers{};
	int currentCustomerIndex = 0;
    

private:
    // Helper functions
    void UpdateOrderTexture();
    void MiniGame_1(bool state);
    void passwordInput(std::string num);
    void enterPassword();
    void deletePassword();

    // Member variables
    Entity* duck = nullptr;
    TransformComponent* duckTrans = nullptr;
    SoundComponent* TimeLeftSound = nullptr;

	// Order Tab
    Entity* OrderTab = nullptr;
    SpriteRendererComponent* orderSprite = nullptr;
    TextComponent* FPSText = nullptr;

    // UI
    Entity* ui = nullptr;
	SpriteRendererComponent* uiSprite = nullptr;

    // Timer
    Entity* timer = nullptr;
    TextComponent* timerText = nullptr;
    float timeLeft = 0.f;

	// Score
    Entity* score = nullptr;
    TextComponent* scoreText = nullptr;

    // Pause Menu
    std::shared_ptr<PauseMenuLogic> pauseMenuLogic = nullptr;

    // Restock Menu
	std::shared_ptr<RestockLogic> robotRestockLogic = nullptr;

    // MiniGame_1
    Entity* gameMiniGame_BG = nullptr;
    SpriteRendererComponent* gameMiniGame_BG_Spt = nullptr;

    Entity* gameMiniGame_Keypad = nullptr;
    SpriteRendererComponent* gameMiniGame_Keypad_Spt = nullptr;
    
    Entity* gameMiniGame_Text = nullptr;
    TextComponent* gameMiniGame_Text_Txt = nullptr;

    Entity* gameMiniGame_Input = nullptr;
    TextComponent* gameMiniGame_Input_Txt = nullptr;

    Entity* gameMiniGame_K1 = nullptr;
    SpriteRendererComponent* gameMiniGame_K1_Spt = nullptr;
    ButtonComponent* gameMiniGame_K1_Btn = nullptr;
    Entity* gameMiniGame_T1 = nullptr;
    TextComponent* gameMiniGame_T1_Txt = nullptr;

    Entity* gameMiniGame_K2 = nullptr;
    SpriteRendererComponent* gameMiniGame_K2_Spt = nullptr;
    ButtonComponent* gameMiniGame_K2_Btn = nullptr;
    Entity* gameMiniGame_T2 = nullptr;
    TextComponent* gameMiniGame_T2_Txt = nullptr;
    
    Entity* gameMiniGame_K3 = nullptr;
    SpriteRendererComponent* gameMiniGame_K3_Spt = nullptr;
    ButtonComponent* gameMiniGame_K3_Btn = nullptr;
    Entity* gameMiniGame_T3 = nullptr;
    TextComponent* gameMiniGame_T3_Txt = nullptr;

    Entity* gameMiniGame_K4 = nullptr;
    SpriteRendererComponent* gameMiniGame_K4_Spt = nullptr;
    ButtonComponent* gameMiniGame_K4_Btn = nullptr;
    Entity* gameMiniGame_T4 = nullptr;
    TextComponent* gameMiniGame_T4_Txt = nullptr;
    
    Entity* gameMiniGame_K5 = nullptr;
    SpriteRendererComponent* gameMiniGame_K5_Spt = nullptr;
    ButtonComponent* gameMiniGame_K5_Btn = nullptr;
    Entity* gameMiniGame_T5 = nullptr;
    TextComponent* gameMiniGame_T5_Txt = nullptr;

    Entity* gameMiniGame_K6 = nullptr;
    SpriteRendererComponent* gameMiniGame_K6_Spt = nullptr;
    ButtonComponent* gameMiniGame_K6_Btn = nullptr;
    Entity* gameMiniGame_T6 = nullptr;
    TextComponent* gameMiniGame_T6_Txt = nullptr;

    Entity* gameMiniGame_K7 = nullptr;
    SpriteRendererComponent* gameMiniGame_K7_Spt = nullptr;
    ButtonComponent* gameMiniGame_K7_Btn = nullptr;
    Entity* gameMiniGame_T7 = nullptr;
    TextComponent* gameMiniGame_T7_Txt = nullptr;

    Entity* gameMiniGame_K8 = nullptr;
    SpriteRendererComponent* gameMiniGame_K8_Spt = nullptr;
    ButtonComponent* gameMiniGame_K8_Btn = nullptr;
    Entity* gameMiniGame_T8 = nullptr;
    TextComponent* gameMiniGame_T8_Txt = nullptr;

    Entity* gameMiniGame_K9 = nullptr;
    SpriteRendererComponent* gameMiniGame_K9_Spt = nullptr;
    ButtonComponent* gameMiniGame_K9_Btn = nullptr;
    Entity* gameMiniGame_T9 = nullptr;
    TextComponent* gameMiniGame_T9_Txt = nullptr;

    Entity* gameMiniGame_K0 = nullptr;
    SpriteRendererComponent* gameMiniGame_K0_Spt = nullptr;
    ButtonComponent* gameMiniGame_K0_Btn = nullptr;
    Entity* gameMiniGame_T0 = nullptr;
    TextComponent* gameMiniGame_T0_Txt = nullptr;
    
    Entity* gameMiniGame_Enter = nullptr;
    SpriteRendererComponent* gameMiniGame_Enter_Spt = nullptr;
    ButtonComponent* gameMiniGame_Enter_Btn = nullptr;
    Entity* gameMiniGame_TextEnter = nullptr;
    TextComponent* gameMiniGame_TextEnter_Txt = nullptr;

    Entity* gameMiniGame_Delete = nullptr;
    SpriteRendererComponent* gameMiniGame_Delete_Spt = nullptr;
    ButtonComponent* gameMiniGame_Delete_Btn = nullptr;
    Entity* gameMiniGame_TextDelete = nullptr;
    TextComponent* gameMiniGame_TextDelete_Txt = nullptr;
    
    Entity* gameMiniGame_Password = nullptr;
    SpriteRendererComponent* gameMiniGame_Password_Spt = nullptr;
    Entity* gameMiniGame_TextPassword = nullptr;
    TextComponent* gameMiniGame_TextPassword_Txt = nullptr;

	// Other Variables
    TextComponent* CountdownText = nullptr;
    float countdownTime = 4.0f;
    bool gameStarted = false;
    bool textcount = true;

	// Fade out
    bool hasStartedFade = false;
    float GamefadeElapsedTime = 0.f;
	SpriteRendererComponent* FadeOutSprite = nullptr;


};
