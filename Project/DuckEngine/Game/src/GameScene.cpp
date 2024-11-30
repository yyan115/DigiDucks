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

int pageNumb = 1;
bool isPaused = false;

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void GameScene::Load() 
{
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(20);

	// instantiate prefabs
	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	if(duck)
	{
		duckTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
		duckSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(duck->entityID);
	}

	OrderTab = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab");
	if (OrderTab)
	{
		orderSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OrderTab->entityID);
		if (orderSprite)
		{
			orderSprite->isVisible = true;
		}
	}

	timer = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Timer_Text");
	if(timer)
	{
		timerText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(timer->entityID);
		if (timerText) {
			timerText->text = "Time: 10:00";
			timeLeft = 60.f;
		}
	}

	score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text");
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
		gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Bg");
		if (gamePauseBg)
		{
			gamePauseBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseBg->entityID);
			if (gamePauseBgSpt)
			{
				gamePauseBgSpt->isVisible = false;
			}
		}

		gamePauseTxt = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Text");
		if (gamePauseTxt)
		{
			gamePauseTxtSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseTxt->entityID);
			if (gamePauseTxtSpt)
			{
				gamePauseTxtSpt->isVisible = false;
			}
		}

		gameResumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn");
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

		gameExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Btn");
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
				gameExitButton->onClick = []() { std::cout << "QUIT\n"; GameManager::DuckEngine.CloseWindow(); };
			}
		}

		gameHTPBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Btn");
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
		gameJournal = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Journal");
		if (gameJournal)
		{
			gameJournalSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameJournal->entityID);
			if (gameJournalSpt)
			{
				gameJournalSpt->isVisible = false;
			}
		}
		
		gameHTPExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Exit_Btn");
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

		gameHTPBackBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Back_Btn");
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
				gameHTPBackButton->onClick = [this]() { std::cout << "BACK\n"; if (pageNumb > 1) { pageNumb--; } };
			}
		}

		gameHTPNextBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Next_Btn");
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
				gameHTPNextButton->onClick = [this]() { std::cout << "NEXT\n"; if (pageNumb < 3) { pageNumb++; } };
			}
		}

	}
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


	// Cheats

	// End the Game
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_M))
	{
		std::cout << "H is pressed!\n";
		GameManager::SetActiveScene("EndScene");
	}
	// Add Score
	Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station");
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
	if (state)
	{
		CameraManager::SetPosition(0.0f, 50.0f);
	}
	else
	{
		CameraManager::SetPosition(duckTrans->GetPosition().x, duckTrans->GetPosition().y);
	}

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
}