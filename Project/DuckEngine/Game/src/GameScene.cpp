/******************************************************************************/
/*!
\file       GameScene.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Implementation of the GameScene class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameScene.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "Scene.h"
#include "SubmitLogic.h"
#include "SpriteRendererComponent.h"
#include "SoundSystem.h"
#include "ScoreLogic.h"

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void GameScene::Load() 
{
	Scene::Load();
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(17);

	// instantiate prefabs
	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
	if(duck)
	{
		duckTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
	}

	OrderTab = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab").get();
	if (OrderTab)
	{
		orderSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OrderTab->entityID);
		if (orderSprite)
		{
			orderSprite->isVisible = true;
		}
	}

	timer = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Timer_Text").get();
	if(timer)
	{
		timerText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(timer->entityID);
		if (timerText) {
			timerText->text = "Time:";
			timeLeft = 600.f;
		}
	}

	score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text").get();
	if(score)
	{
		scoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(score->entityID);
		if (scoreText) 
		{
			scoreText->text = "Score: 0";
			ScoreLogic::scoreValue = 0;
		}
	}
	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
	}

	isPaused = false;
	pageNumb = 1;

	// Pause Menu
	{
		gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
		if (gamePauseBg)
		{
			gamePauseBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseBg->entityID);
			if (gamePauseBgSpt)
			{
				gamePauseBgSpt->isVisible = false;
			}
		}

		auto gameResumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn").get();
		if (gameResumeBtn)
		{
			gameResumeBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameResumeBtn->entityID);
			gameResumeBtn_Normal = AssetManager::GetTextureByName("pause_resumegame");
			gameResumeBtn_Hover = AssetManager::GetTextureByName("pause_resumegame_hover");
			gameResumeButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameResumeBtn->entityID);
			if (gameResumeButton)
			{
				gameResumeButton->onClick = [this]() { if (isPaused) { PauseGame(false); } };
				gameResumeButton->onHover = [this]() { gameResumeBtnSpt->texture = gameResumeBtn_Hover; };
				gameResumeButton->onFinishHover = [this]() { gameResumeBtnSpt->texture = gameResumeBtn_Normal; };
			}
		}

		auto gameExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Btn").get();
		if (gameExitBtn)
		{
			gameExitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitBtn->entityID);
			gameExitBtn_Normal = AssetManager::GetTextureByName("pause_quitgame");
			gameExitBtn_Hover = AssetManager::GetTextureByName("pause_quitgame_hover");
			gameExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitBtn->entityID);
			if (gameExitButton)
			{
				gameExitButton->onClick = [this]() { ExitConfirm(true); };
				gameExitButton->onHover = [this]() { gameExitBtnSpt->texture = gameExitBtn_Hover; };
				gameExitButton->onFinishHover = [this]() { gameExitBtnSpt->texture = gameExitBtn_Normal; };
			}
		}

		auto gameHTPBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Btn").get();
		if (gameHTPBtn)
		{
			gameHTPBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPBtn->entityID);
			gameHTPBtn_Normal = AssetManager::GetTextureByName("pause_howtoplay");
			gameHTPBtn_Hover = AssetManager::GetTextureByName("pause_howtoplay_hover");
			gameHTPButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBtn->entityID);
			if (gameHTPButton)
			{
				gameHTPButton->onClick = [this]() { HTPShow(true); };
				gameHTPButton->onHover = [this]() {  gameHTPBtnSpt->texture = gameHTPBtn_Hover; };
				gameHTPButton->onFinishHover = [this]() { gameHTPBtnSpt->texture = gameHTPBtn_Normal; };
			}
		}
	}

	// H.T.P Menu
	{
		gameJournal = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Journal").get();
		if (gameJournal)
		{
			gameJournalSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameJournal->entityID);
			if (gameJournalSpt)
			{
				gameJournalSpt->isVisible = false;
			}
		}
		
		gameHTPExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Exit_Btn").get();
		if (gameHTPExitBtn)
		{
			gameHTPExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPExitBtn->entityID);
			if (gameHTPExitButton)
			{
				gameHTPExitButton->onClick = [this]() { std::cout << "EXIT\n"; HTPShow(false); };
			}
		}

		gameHTPBackBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Back_Btn").get();
		if (gameHTPBackBtn)
		{
			gameHTPBackButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBackBtn->entityID);
			if (gameHTPBackButton)
			{
				gameHTPBackButton->onClick = [this]() { std::cout << "BACK\n"; if (pageNumb > 1) { pageNumb--; changePage(); } };
			}
		}

		gameHTPNextBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Next_Btn").get();
		if (gameHTPNextBtn)
		{
			gameHTPNextButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPNextBtn->entityID);
			if (gameHTPNextButton)
			{
				gameHTPNextButton->onClick = [this]() { std::cout << "NEXT\n"; if (pageNumb < 3) { pageNumb++; changePage(); } };
			}
		}

	}

	// Exit Confirmation
	{
		gameExitCfmBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Cfm_Bg").get();
		if (gameExitCfmBg)
		{
			gameExitCfmBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitCfmBg->entityID);
			if (gameExitCfmBgSpt)
			{
				gameExitCfmBgSpt->isVisible = false;
			}
		}

		auto gameExitYesBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Yes_Btn").get();
		if (gameExitYesBtn)
		{
			gameExitYesBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitYesBtn->entityID);
			gameExitYesBtn_Normal = AssetManager::GetTextureByName("exit_yes");
			gameExitYesBtn_Hover = AssetManager::GetTextureByName("exit_yes_hover");
			gameExitYesButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitYesBtn->entityID);
			if (gameExitYesButton)
			{
				gameExitYesButton->onClick = []() { GameManager::DuckEngine.CloseWindow(); };
				gameExitYesButton->onHover = [this]() { gameExitYesBtnSpt->texture = gameExitYesBtn_Hover; };
				gameExitYesButton->onFinishHover = [this]() { gameExitYesBtnSpt->texture = gameExitYesBtn_Normal; };
			}
		} 

		auto gameExitNoBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_No_Btn").get();
		if (gameExitNoBtn)
		{
			gameExitNoBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitNoBtn->entityID);
			gameExitNoBtn_Normal = AssetManager::GetTextureByName("exit_no");
			gameExitNoBtn_Hover = AssetManager::GetTextureByName("exit_no_hover");
			gameExitNoButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitNoBtn->entityID);
			if (gameExitNoButton)
			{
				gameExitNoButton->onClick = [this]() { ExitConfirm(false); };
				gameExitNoButton->onHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Hover; };
				gameExitNoButton->onFinishHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Normal; };
			}
		}
	}


	// MiniGame_1
	{
		gameMiniGame_Text = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Text").get();
		if (gameMiniGame_Text)
		{
			gameMiniGame_Text_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_Text->entityID);
			if (gameMiniGame_Text_Txt)
			{
				gameMiniGame_Text_Txt->isEnabled = false;
			}
		}

		gameMiniGame_BG = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_BG").get();
		if (gameMiniGame_BG)
		{
			gameMiniGame_BG_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_BG->entityID);
			if (gameMiniGame_BG_Spt)
			{
				gameMiniGame_BG_Spt->isVisible = false;
			}
		}

		gameMiniGame_Keypad = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Keypad").get();
		if (gameMiniGame_Keypad)
		{
			gameMiniGame_Keypad_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Keypad->entityID);
			if (gameMiniGame_Keypad_Spt)
			{
				gameMiniGame_Keypad_Spt->isVisible = false;
			}
		}

		gameMiniGame_K1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K1").get();
		if (gameMiniGame_K1)
		{
			gameMiniGame_K1_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K1->entityID);
			if (gameMiniGame_K1_Spt)
			{
				gameMiniGame_K1_Spt->isVisible = false;
			}
			gameMiniGame_K1_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K1->entityID);
			if (gameMiniGame_K1_Btn)
			{
				gameMiniGame_K1_Btn->onClick = [this]() {std::cout << "1" << std::endl; };
			}
		}

		gameMiniGame_T1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T1").get();
		if (gameMiniGame_T1)
		{
			gameMiniGame_T1_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T1->entityID);
			if (gameMiniGame_T1_Txt)
			{
				gameMiniGame_T1_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K2").get();
		if (gameMiniGame_K2)
		{
			gameMiniGame_K2_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K2->entityID);
			if (gameMiniGame_K2_Spt)
			{
				gameMiniGame_K2_Spt->isVisible = false;
			}
			gameMiniGame_K2_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K2->entityID);
			if (gameMiniGame_K2_Btn)
			{
				gameMiniGame_K2_Btn->onClick = [this]() { std::cout << "2" << std::endl; };
			}
		}

		gameMiniGame_T2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T2").get();
		if (gameMiniGame_T2)
		{
			gameMiniGame_T2_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T2->entityID);
			if (gameMiniGame_T2_Txt)
			{
				gameMiniGame_T2_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K3").get();
		if (gameMiniGame_K3)
		{
			gameMiniGame_K3_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K3->entityID);
			if (gameMiniGame_K3_Spt)
			{
				gameMiniGame_K3_Spt->isVisible = false;
			}
			gameMiniGame_K3_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K3->entityID);
			if (gameMiniGame_K3_Btn)
			{
				gameMiniGame_K3_Btn->onClick = [this]() {std::cout << "3" << std::endl; };
			}
		}

		gameMiniGame_T3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T3").get();
		if (gameMiniGame_T3)
		{
			gameMiniGame_T3_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T3->entityID);
			if (gameMiniGame_T3_Txt)
			{
				gameMiniGame_T3_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K4").get();
		if (gameMiniGame_K4)
		{
			gameMiniGame_K4_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K4->entityID);
			if (gameMiniGame_K4_Spt)
			{
				gameMiniGame_K4_Spt->isVisible = false;
			}
			gameMiniGame_K4_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K4->entityID);
			if (gameMiniGame_K4_Btn)
			{
				gameMiniGame_K4_Btn->onClick = [this]() { std::cout << "4" << std::endl; };
			}
		}

		gameMiniGame_T4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T4").get();
		if (gameMiniGame_T4)
		{
			gameMiniGame_T4_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T4->entityID);
			if (gameMiniGame_T4_Txt)
			{
				gameMiniGame_T4_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K5").get();
		if (gameMiniGame_K5)
		{
			gameMiniGame_K5_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K5->entityID);
			if (gameMiniGame_K5_Spt)
			{
				gameMiniGame_K5_Spt->isVisible = false;
			}
			gameMiniGame_K5_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K5->entityID);
			if (gameMiniGame_K5_Btn)
			{
				gameMiniGame_K5_Btn->onClick = [this]() {std::cout << "5" << std::endl; };
			}
		}

		gameMiniGame_T5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T5").get();
		if (gameMiniGame_T5)
		{
			gameMiniGame_T5_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T5->entityID);
			if (gameMiniGame_T5_Txt)
			{
				gameMiniGame_T5_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K6").get();
		if (gameMiniGame_K6)
		{
			gameMiniGame_K6_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K6->entityID);
			if (gameMiniGame_K6_Spt)
			{
				gameMiniGame_K6_Spt->isVisible = false;
			}
			gameMiniGame_K6_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K6->entityID);
			if (gameMiniGame_K6_Btn)
			{
				gameMiniGame_K6_Btn->onClick = [this]() { std::cout << "6" << std::endl; };
			}
		}

		gameMiniGame_T6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T6").get();
		if (gameMiniGame_T6)
		{
			gameMiniGame_T6_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T6->entityID);
			if (gameMiniGame_T6_Txt)
			{
				gameMiniGame_T6_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K7").get();
		if (gameMiniGame_K7)
		{
			gameMiniGame_K7_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K7->entityID);
			if (gameMiniGame_K7_Spt)
			{
				gameMiniGame_K7_Spt->isVisible = false;
			}
			gameMiniGame_K7_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K7->entityID);
			if (gameMiniGame_K7_Btn)
			{
				gameMiniGame_K7_Btn->onClick = [this]() {std::cout << "7" << std::endl; };
			}
		}

		gameMiniGame_T7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T7").get();
		if (gameMiniGame_T7)
		{
			gameMiniGame_T7_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T7->entityID);
			if (gameMiniGame_T7_Txt)
			{
				gameMiniGame_T7_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K8").get();
		if (gameMiniGame_K8)
		{
			gameMiniGame_K8_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K8->entityID);
			if (gameMiniGame_K8_Spt)
			{
				gameMiniGame_K8_Spt->isVisible = false;
			}
			gameMiniGame_K8_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K8->entityID);
			if (gameMiniGame_K8_Btn)
			{
				gameMiniGame_K8_Btn->onClick = [this]() { std::cout << "8" << std::endl; };
			}
		}

		gameMiniGame_T8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T8").get();
		if (gameMiniGame_T8)
		{
			gameMiniGame_T8_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T8->entityID);
			if (gameMiniGame_T8_Txt)
			{
				gameMiniGame_T8_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K9").get(); 
		if (gameMiniGame_K9)
		{
			gameMiniGame_K9_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K9->entityID); 
			if (gameMiniGame_K9_Spt)
			{
				gameMiniGame_K9_Spt->isVisible = false;
			}
			gameMiniGame_K9_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K9->entityID);
			if (gameMiniGame_K9_Btn)
			{
				gameMiniGame_K9_Btn->onClick = [this]() {std::cout << "9" << std::endl; };
			}
		}

		gameMiniGame_T9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T9").get();
		if (gameMiniGame_T9)
		{
			gameMiniGame_T9_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T9->entityID);
			if (gameMiniGame_T9_Txt)
			{
				gameMiniGame_T9_Txt->isEnabled = false;
			}
		}

		gameMiniGame_K10 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K10").get();
		if (gameMiniGame_K10)
		{
			gameMiniGame_K10_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K10->entityID);
			if (gameMiniGame_K10_Spt)
			{
				gameMiniGame_K10_Spt->isVisible = false;
			}
			gameMiniGame_K10_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K10->entityID);
			if (gameMiniGame_K10_Btn)
			{
				gameMiniGame_K10_Btn->onClick = [this]() { std::cout << "10" << std::endl; };
			}
		}

		gameMiniGame_T10 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T10").get();
		if (gameMiniGame_T10)
		{
			gameMiniGame_T10_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T10->entityID);
			if (gameMiniGame_T10_Txt)
			{
				gameMiniGame_T10_Txt->isEnabled = false;
			}
		}
	}
	
	auto TimeLeftEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("TimerSFXManager");
	if (TimeLeftEntity) {
		TimeLeftSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(TimeLeftEntity->entityID);
	}
	auto FadeEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameFadeScreen");
	if (FadeEntity) {
		FadeOutSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeEntity->entityID);
		FadeOutSprite->isVisible = false;
	}
	auto CountdownEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Countdown_Text");
	if (CountdownEntity) {
		CountdownText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(CountdownEntity->entityID);
		CountdownText->isEnabled = false;
	}
	hasStartedFade = false;
	GamefadeElapsedTime = 0.0f;
	gameStarted = false;
	countdownTime = 4.0f;

	PauseGame(false);
	MiniGame_1(false);
}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void GameScene::Start() 
{
	Scene::Start();
	//DuckEngine::showDebugColliders = false;
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void GameScene::Update() 
{	
	if (!robotRestockLogic)
	{
		auto gameRestockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
		if (gameRestockMenu)
		{
			std::cout << "Restock Menu Found" << std::endl;
			robotRestockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(gameRestockMenu->entityID);
			if (robotRestockLogic)
			{
				std::cout << "Restock Logic Found" << std::endl;
			}
			else
			{
				std::cout << "Restock Logic Not Found" << std::endl;
			}
		}
	}

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	// Handle countdown before game starts
	if (!gameStarted) {
		countdownTime -= DuckEngine::DeltaTime();
		int displayNumber = static_cast<int>(ceil(countdownTime));

		if (CountdownText) {
			CountdownText->isEnabled = true;
			if (displayNumber > 1) {
				TimeLeftSound->Play(3);
				CountdownText->text = std::to_string(displayNumber) + "..";
			}
			else {
				CountdownText->text = "Go!";
			}
		}

		// Reduce FadeOutSprite opacity over time
		if (FadeOutSprite) {
			FadeOutSprite->isVisible = true;
			float fadeProgress = countdownTime / 3.0f;
			FadeOutSprite->color.a = static_cast<int>(fadeProgress * 150); // Reduce alpha gradually
		}

		// When countdown finishes, start the game
		if (countdownTime <= 0.0f) {
			gameStarted = true;
			CountdownText->isEnabled = false;
			// Reset sprite
			if (FadeOutSprite) {
				FadeOutSprite->color.a = 0;
				FadeOutSprite->isVisible = false;
			}
		}
		return; // Skip game logic until countdown is done
	}

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	if (!isPaused)
	{
		CameraManager::LerpCameraTo(duckTrans->GetPosition().x, duckTrans->GetPosition().y);
	}

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	// Timer Display
	if (timerText) {
		// Update the timer
		if (timeLeft > 0.f) {
			timeLeft -= DuckEngine::DeltaTime();
			int minutes = static_cast<int>(timeLeft) / 60;
			int seconds = static_cast<int>(timeLeft) % 60;
			timerText->text = "Time: " + std::to_string(minutes) + ":" + std::to_string(seconds);

			if (timeLeft < 10.f && TimeLeftSound != nullptr) {
				timerText->color = { 255, 0, 0, 255 };
				TimeLeftSound->Play(0);
				GamefadeElapsedTime = 0.0f;
			}
		}
		else {
			timerText->text = "Time's up!";

			if (!hasStartedFade && TimeLeftSound != nullptr) {
				SoundSystem::StopSounds(TimeLeftSound->soundID[0]); // Stop warning sound
				TimeLeftSound->Play(1); // Play final sound
				hasStartedFade = true;
				FadeOutSprite->isVisible = true;
			}

			if (hasStartedFade) {
				GamefadeElapsedTime += DuckEngine::DeltaTime();
				float fadeProgress = GamefadeElapsedTime / 3.0f;

				if (fadeProgress >= 1.0f) {
					SoundSystem::SetSoundVolume(TimeLeftSound->soundID[1], 0.0f);					
					SoundSystem::StopSounds(TimeLeftSound->soundID[1]);
					FadeOutSprite->color.a = 0;
					FadeOutSprite->isVisible = false;
					GameManager::SetActiveScene("EndScene");
				}
				else {
					float newVolume = TimeLeftSound->volume * (1.0f - fadeProgress);
					SoundSystem::SetSoundVolume(TimeLeftSound->soundID[1], newVolume);
					FadeOutSprite->color.a = static_cast<int>(fadeProgress * 255.0f);
				}
			}
		}
	}

	// FPS Counter
	if (FPSText != nullptr) {
		FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	}


	// Cheats

	// End the Game
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_M))
	{
		std::cout << "H is pressed!\n";
		GameManager::SetActiveScene("EndScene");
	}
	// Add Score
	Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station").get();
	auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(submit->entityID);

	if (submitLogic && submitLogic->CheckNewOrder()) {
		UpdateOrderTexture(); // Update the order texture on successful submission
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_N))
	{
		submitLogic->increaseScore(10);
	}
	ScoreLogic::scoreValue = submitLogic->getScore();
}

void GameScene::UpdateOrderTexture() {
	// Generate a new random texture path
	auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OrderTab->entityID);
	if (spriteRenderer) {
		spriteRenderer->isVisible = false;
	}
}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void GameScene::PostUpdate()
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}


	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_P))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::ToggleShowDebugColliders();
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE))
	{
		if (robotRestockLogic)
		{
			if (robotRestockLogic->isRestock)
			{
				robotRestockLogic->RestockMenu(false);
				return;
			}
		}
		
		std::cout << "Escape is pressed!\n";
		TimeLeftSound->Play(2);
		Sleep(500);
		PauseGame(!isPaused);
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U))
	{
		if (FPSText)
		{
			FPSText->isEnabled = !FPSText->isEnabled;
		}
	}
}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void GameScene::Exit() 
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void GameScene::Unload() 
{
	// base unload
	Scene::Unload();

}


/****************************************************************
* @brief Pause the game and display the pause menu.
* @param isPaused - true if the game is paused, false if the game
* is unpaused.
* ****************************************************************/
void GameScene::PauseGame(bool state) 
{
	isPaused = state;
	// Hide Texts
	if (scoreText)
	{
		scoreText->isEnabled = !state;
	}
	if (timerText)
	{
		timerText->isEnabled = !state;
	}
	if (FPSText)
	{
		FPSText->isEnabled = !state;
	}

	// Hide Order Tab
	if (orderSprite)
	{
		orderSprite->isVisible = !state;
	}

	// Show Pause Menu
	if (gamePauseBgSpt)
	{
		gamePauseBgSpt->isVisible = state;
	}

	HTPShow(false);
	ExitConfirm(false);
	DuckEngine::SetPaused(state);
}


/****************************************************************
* @brief Display the How To Play menu.
* ****************************************************************/
void GameScene::HTPShow(bool state) 
{
	if (gameJournalSpt)
	{
		gameJournalSpt->isVisible = state;
	}

	// Disable Quit and Resume Btn
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}
	if (gameExitButton)
	{
		gameExitButton->isEnabled = !state;
	}
}



/****************************************************************
* @brief Change the visibility of the exit confirmation menu.
* ****************************************************************/
void GameScene::ExitConfirm(bool state)
{

	if (gameExitCfmBg)
	{
		gameExitCfmBgSpt->isVisible = state;
	}

	// Disable HTP  and Quit Btn
	if (gameHTPButton)
	{
		gameHTPButton->isEnabled = !state;
	}
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}

}


/****************************************************************
* @brief Change the current page of the How To Play menu.
* ****************************************************************/
void GameScene::changePage() 
{
	switch (pageNumb)
	{
	case 1:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_1");
		break;
	case 2:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_2");
		break;
	case 3:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_3");
		break;
	default:
		break;
	};
}




void GameScene::MiniGame_1(bool state)
{
	// Hide Texts
	if (scoreText)
	{
		scoreText->isEnabled = !state;
	}
	if (timerText)
	{
		timerText->isEnabled = !state;
	}

	// Hide Order Tab
	if (orderSprite)
	{
		orderSprite->isVisible = !state;
	}

// show minigame
	if (gameMiniGame_BG)
	{
		gameMiniGame_BG_Spt->isVisible = state;
	}
	if (gameMiniGame_Keypad)
	{
		gameMiniGame_Keypad_Spt->isVisible = state;
	}
	if (gameMiniGame_K1)
	{
		gameMiniGame_K1_Spt->isVisible = state;
	}
	if (gameMiniGame_K2)
	{
		gameMiniGame_K2_Spt->isVisible = state;
	}
	if (gameMiniGame_T1)
	{
		gameMiniGame_T1_Txt->isEnabled = state;
	}
	if (gameMiniGame_T2)
	{
		gameMiniGame_T2_Txt->isEnabled = state;
	}
	if (gameMiniGame_K3)
	{
		gameMiniGame_K3_Spt->isVisible = state;
	}
	if (gameMiniGame_K4)
	{
		gameMiniGame_K4_Spt->isVisible = state;
	}
	if (gameMiniGame_T3)
	{
		gameMiniGame_T3_Txt->isEnabled = state;
	}
	if (gameMiniGame_T4)
	{
		gameMiniGame_T4_Txt->isEnabled = state;
	}
	if (gameMiniGame_K5)
	{
		gameMiniGame_K5_Spt->isVisible = state;
	}
	if (gameMiniGame_K6)
	{
		gameMiniGame_K6_Spt->isVisible = state;
	}
	if (gameMiniGame_T5)
	{
		gameMiniGame_T5_Txt->isEnabled = state;
	}
	if (gameMiniGame_T6)
	{
		gameMiniGame_T6_Txt->isEnabled = state;
	}
	if (gameMiniGame_K7)
	{
		gameMiniGame_K7_Spt->isVisible = state;
	}
	if (gameMiniGame_K8)
	{
		gameMiniGame_K8_Spt->isVisible = state;
	}
	if (gameMiniGame_T7)
	{
		gameMiniGame_T7_Txt->isEnabled = state;
	}
	if (gameMiniGame_T8)
	{
		gameMiniGame_T8_Txt->isEnabled = state;
	}
	if (gameMiniGame_K9)
	{
		gameMiniGame_K9_Spt->isVisible = state;
	}
	if (gameMiniGame_K10)
	{
		gameMiniGame_K10_Spt->isVisible = state;
	}
	if (gameMiniGame_T9)
	{
		gameMiniGame_T9_Txt->isEnabled = state;
	}
	if (gameMiniGame_T10)
	{
		gameMiniGame_T10_Txt->isEnabled = state;
	}
	if (gameMiniGame_Text)
	{
		gameMiniGame_Text_Txt->isEnabled = state;
	}
}