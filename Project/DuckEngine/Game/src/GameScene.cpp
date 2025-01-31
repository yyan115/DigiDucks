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
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "Scene.h"
#include "SubmitLogic.h"
#include "SpriteRendererComponent.h"

Entity* duck;
TransformComponent* duckTrans;
SoundComponent* duckSound;

Entity* OrderTab;
SpriteRendererComponent* orderSprite;
Entity* timer;
TextComponent* timerText;
float timeLeft{};

Entity* score;
TextComponent* scoreText;
int scoreValue{};

/*Pause Menu*/
Entity* gamePauseBg;
SpriteRendererComponent* gamePauseBgSpt;
Entity* gamePauseTxt;
SpriteRendererComponent* gamePauseTxtSpt;
Entity* gameResumeBtn;
SpriteRendererComponent* gameResumeBtnSpt;
ButtonComponent* gameResumeButton;
Entity* gameExitBtn;
SpriteRendererComponent* gameExitBtnSpt;
ButtonComponent* gameExitButton;
Entity* gameHTPBtn;
SpriteRendererComponent* gameHTPBtnSpt;
ButtonComponent* gameHTPButton;

/*HTP Menu*/
Entity* gameJournal;
SpriteRendererComponent* gameJournalSpt;
Entity* gameHTPExitBtn;
SpriteRendererComponent* gameHTPExitBtnSpt;
ButtonComponent* gameHTPExitButton;
Entity* gameHTPBackBtn;
SpriteRendererComponent* gameHTPBackBtnSpt;
ButtonComponent* gameHTPBackButton;
Entity* gameHTPNextBtn;
SpriteRendererComponent* gameHTPNextBtnSpt;
ButtonComponent* gameHTPNextButton;

/*Exit Confirm*/
Entity* gameExitCfmBg;
SpriteRendererComponent* gameExitCfmBgSpt;
Entity* gameExitCfmTxt;
TextComponent* gameExitCfmText;
Entity* gameExitYesBtn;
SpriteRendererComponent* gameExitYesBtnSpt;
ButtonComponent* gameExitYesButton;
Entity* gameExitNoBtn;
SpriteRendererComponent* gameExitNoBtnSpt;
ButtonComponent* gameExitNoButton;

/*MiniGame_1*/
Entity* gameMiniGame_BG;
SpriteRendererComponent* gameMiniGame_BG_Spt;
Entity* gameMiniGame_Keypad;
SpriteRendererComponent* gameMiniGame_Keypad_Spt;
Entity* gameMiniGame_K1;
SpriteRendererComponent* gameMiniGame_K1_Spt;
ButtonComponent* gameMiniGame_K1_Btn;
Entity* gameMiniGame_K2;
SpriteRendererComponent* gameMiniGame_K2_Spt;
ButtonComponent* gameMiniGame_K2_Btn;
Entity* gameMiniGame_T1;
TextComponent* gameMiniGame_T1_Txt;
Entity* gameMiniGame_T2;
TextComponent* gameMiniGame_T2_Txt;
Entity* gameMiniGame_Text;
TextComponent* gameMiniGame_Text_Txt;


int pageNumb = 1;
bool isPaused = false;

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void GameScene::Load() 
{
	Scene::Load();
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(15);

	// instantiate prefabs
	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
	if(duck)
	{
		duckTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
		duckSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(duck->entityID);
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
			timerText->text = "Time: 10:00";
			timeLeft = 60.f;
		}
	}

	score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text").get();
	if(score)
	{
		scoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(score->entityID);
		if (scoreText) 
		{
			scoreText->text = "Score: 0";
			scoreValue = 0;
		}
	}

	isPaused = false;
	pageNumb = 1;

	// Pause Menu
	{
		gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Bg").get();
		if (gamePauseBg)
		{
			gamePauseBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseBg->entityID);
			if (gamePauseBgSpt)
			{
				gamePauseBgSpt->isVisible = false;
			}
		}

		gamePauseTxt = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Text").get();
		if (gamePauseTxt)
		{
			gamePauseTxtSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseTxt->entityID);
			if (gamePauseTxtSpt)
			{
				gamePauseTxtSpt->isVisible = false;
			}
		}

		gameResumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn").get();
		if (gameResumeBtn)
		{
			gameResumeBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameResumeBtn->entityID);
			if (gameResumeBtnSpt)
			{
				gameResumeBtnSpt->isVisible = false;
			}
			gameResumeButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameResumeBtn->entityID);
			if (gameResumeButton)
			{
				gameResumeButton->onClick = [this]() { std::cout << "RESUME\n"; if (isPaused) { PauseGame(false); } };
			}
		}

		gameExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Btn").get();
		if (gameExitBtn)
		{
			gameExitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitBtn->entityID);
			if (gameExitBtnSpt)
			{
				gameExitBtnSpt->isVisible = false;
			}
			gameExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitBtn->entityID);
			if (gameExitButton)
			{
				gameExitButton->onClick = [this]() { std::cout << "QUIT\n"; ExitConfirm(true); };
			}
		}

		gameHTPBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Btn").get();
		if (gameHTPBtn)
		{
			gameHTPBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPBtn->entityID);
			if (gameHTPBtnSpt)
			{
				gameHTPBtnSpt->isVisible = false;
			}
			gameHTPButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBtn->entityID);
			if (gameHTPButton)
			{
				gameHTPButton->onClick = [this]() { std::cout << "HTP\n"; HTPShow(true); };
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
			gameHTPExitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPExitBtn->entityID);
			if (gameHTPExitBtnSpt)
			{
				gameHTPExitBtnSpt->isVisible = false;
			}
			gameHTPExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPExitBtn->entityID);
			if (gameHTPExitButton)
			{
				gameHTPExitButton->onClick = [this]() { std::cout << "EXIT\n"; HTPShow(false); };
			}
		}

		gameHTPBackBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Back_Btn").get();
		if (gameHTPBackBtn)
		{
			gameHTPBackBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPBackBtn->entityID);
			if (gameHTPBackBtnSpt)
			{
				gameHTPBackBtnSpt->isVisible = false;
			}
			gameHTPBackButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBackBtn->entityID);
			if (gameHTPBackButton)
			{
				gameHTPBackButton->onClick = [this]() { std::cout << "BACK\n"; if (pageNumb > 1) { pageNumb--; changePage(); } };
			}
		}

		gameHTPNextBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Next_Btn").get();
		if (gameHTPNextBtn)
		{
			gameHTPNextBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPNextBtn->entityID);
			if (gameHTPNextBtnSpt)
			{
				gameHTPNextBtnSpt->isVisible = false;
			}
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

		gameExitCfmTxt = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Cfm_Txt").get();
		if (gameExitCfmTxt)
		{
			gameExitCfmText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameExitCfmTxt->entityID);
			if (gameExitCfmText)
			{
				gameExitCfmText->isEnabled = false;
			}
		}

		gameExitYesBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Yes_Btn").get();
		if (gameExitYesBtn)
		{
			gameExitYesBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitYesBtn->entityID);
			if (gameExitYesBtnSpt)
			{
				gameExitYesBtnSpt->isVisible = false;
			}
			gameExitYesButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitYesBtn->entityID);
			if (gameExitYesButton)
			{
				gameExitYesButton->onClick = []() { std::cout << "YES\n"; GameManager::DuckEngine.CloseWindow(); };
			}
		}

		gameExitNoBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_No_Btn").get();
		if (gameExitNoBtn)
		{
			gameExitNoBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitNoBtn->entityID);
			if (gameExitNoBtnSpt)
			{
				gameExitNoBtnSpt->isVisible = false;
			}
			gameExitNoButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitNoBtn->entityID);
			if (gameExitNoButton)
			{
				gameExitNoButton->onClick = [this]() { std::cout << "NO\n"; ExitConfirm(false); };
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
	}

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

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

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

	if (timerText) {
		// Update the timer
		if (timeLeft > 0.f) {
			timeLeft -= DuckEngine::DeltaTime();
			int minutes = static_cast<int>(timeLeft) / 60;
			int seconds = static_cast<int>(timeLeft) % 60;
			timerText->text = "Time: " + std::to_string(minutes) + ":" + std::to_string(seconds);
		}
		else {
			timerText->text = "Time's up!";
			// Change to End Scene.
			GameManager::SetActiveScene("EndScene");
		}
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
	scoreValue = submitLogic->getScore();
}

void GameScene::UpdateOrderTexture() {
	// Generate a new random texture path
	std::string newOrderTexture = "Resources/Sprites/Ingredients/Dishes/Dish_" + std::to_string((rand() % 2) + 1) + ".png";
	auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OrderTab->entityID);
	if (spriteRenderer) {
		spriteRenderer->texture = *AssetManager::GetTexture(newOrderTexture).get();
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

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (duckSound) {
			std::cout << "Sound stopped\n";
			duckSound->Stop();
		}
	}


	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_P))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::ToggleShowDebugColliders();
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE))
	{
		std::cout << "Escape is pressed!\n";

		PauseGame(!isPaused);
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
	if (gamePauseTxtSpt)
	{
		gamePauseTxtSpt->isVisible = state;
	}
	if (gameResumeBtnSpt)
	{
		gameResumeBtnSpt->isVisible = state;
	}
	if (gameExitBtnSpt)
	{
		gameExitBtnSpt->isVisible = state;
	}
	if (gameHTPBtnSpt)
	{
		gameHTPBtnSpt->isVisible = state;
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
	if (gameHTPExitBtnSpt)
	{
		gameHTPExitBtnSpt->isVisible = state;
	}
	if (gameHTPBackBtnSpt)
	{
		gameHTPBackBtnSpt->isVisible = state;
	}
	if (gameHTPNextBtnSpt)
	{
		gameHTPNextBtnSpt->isVisible = state;
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
* @brief Display Comfirmation to exit the game.
* ****************************************************************/
void GameScene::ExitConfirm(bool state)
{

	if (gameExitCfmBg)
	{
		gameExitCfmBgSpt->isVisible = state;
	}
	if (gameExitCfmTxt)
	{
		gameExitCfmText->isEnabled = state;
	}
	if (gameExitYesBtn)
	{
		gameExitYesBtnSpt->isVisible = state;
	}
	if (gameExitNoBtn)
	{
		gameExitNoBtnSpt->isVisible = state;
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
	if (gameMiniGame_Text)
	{
		gameMiniGame_Text_Txt->isEnabled = state;
	}
}