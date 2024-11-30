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
Entity* pauseBG;
SpriteRendererComponent* pauseBgSprite;
Entity* pauseTxt;
SpriteRendererComponent* pauseTxtSprite;
Entity* resumeBtn;
SpriteRendererComponent* resumeBtnSprite;
ButtonComponent* resumeButton;
bool isPause = false;


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

	pauseBG = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Bg");
	if (pauseBG)
	{
		pauseBgSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(pauseBG->entityID);
		if (pauseBgSprite)
		{
			pauseBgSprite->isVisible = false;
		}
	}

	pauseTxt = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Text");
	if (pauseTxt)
	{
		pauseTxtSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(pauseTxt->entityID);
		if (pauseTxtSprite)
		{
			pauseTxtSprite->isVisible = false;
		}
	}

	resumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn");
	if (resumeBtn)
	{
		resumeBtnSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(resumeBtn->entityID);
		if (resumeBtnSprite)
		{
			resumeBtnSprite->isVisible = false;
		}
		resumeButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(resumeBtn->entityID);
		if (resumeButton)
		{
			resumeButton->onClick = []() { std::cout << "RESUME\n"; if (DuckEngine::IsPlaying) { DuckEngine::SetPaused(false); } };
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
	CameraManager::LerpCameraTo(duckTrans->GetPosition().x, duckTrans->GetPosition().y);

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
		if (pauseBgSprite)
		{
			pauseBgSprite->isVisible = true;
		}
		if (pauseTxtSprite)
		{
			pauseTxtSprite->isVisible = true;
		}
		if (resumeBtnSprite)
		{
			resumeBtnSprite->isVisible = true;
		}
		DuckEngine::SetPaused(true);
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