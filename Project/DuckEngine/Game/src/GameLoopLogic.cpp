/******************************************************************************/
/*!
\file       GameLoopLogic.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the GameLoopLogic class, which manages the core
			gameplay loop, including initialization, updates, and fixed
			updates. It handles UI elements, timers, scoring, customer
			logic, and player interactions. This system interacts with
			DuckEngine�s entity and component managers, as well as various
			logic systems such as PauseMenuLogic, GameSettingsLogic, and
			CutSceneLogic.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameLoopLogic.h"
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
#include "CustomerLogic.h"
#include "Emitter.h"
#include "CustomerStateManager.h"

#include "CustomerTableLogic.h"

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


// Score
Entity* score = nullptr;
TextComponent* scoreText = nullptr;

// Pause Menu
std::shared_ptr<PauseMenuLogic> pauseMenuLogic = nullptr;

// Settings Menu
std::shared_ptr<GameSettingsLogic> gameSettingsLogic = nullptr;

// Other Variables
TextComponent* CountdownText = nullptr;
float countdownTime = 4.0f;
bool gameStarted = false;

// Fade out
bool hasStartedFade = false;
float GamefadeElapsedTime = 0.f;
SpriteRendererComponent* FadeOutSprite = nullptr;

// state manger for this level
CustomerStateManager stateManager;

void GameLoopLogic::Start()
{
	DuckEngine::EnableLogging(false);
	CameraManager::SetHeight(24);
	CameraManager::SetPosition(-3.13f, -1.55f);


	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
	if (duck)
	{
		duckTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
	}

	OrderTab = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab").get();

	ui = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("UI").get();
	if (ui)
	{
		uiSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ui->entityID);
	}

	timer = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Timer_Text").get();
	if (timer)
	{
		timerText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(timer->entityID);
		if (timerText) {
			timerText->text = "0:00";
			// Placeholder Time:
			timeLeft = 300.f;
		}
	}

	score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score_Text").get();
	if (score)
	{
		scoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(score->entityID);
		if (scoreText)
		{
			scoreText->text = "0";
			ScoreLogic::scoreValue = 0;
		}
	}
	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
		if (GameManager::GetGlobalVariable("ShowFPS").empty()) { FPSText->isEnabled = false; }
		else if (GameManager::GetGlobalVariable("ShowFPS") == "true") { FPSText->isEnabled = true; }
		else { FPSText->isEnabled = false; }
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

	auto pauseMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
	if (pauseMenu)
	{
		pauseMenuLogic = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(pauseMenu->entityID);
	}

	auto settingsMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn").get();
	if (settingsMenu)
	{
		gameSettingsLogic = GameLogicManager::GetLogicForEntity<GameSettingsLogic>(settingsMenu->entityID);
	}

	CutScene = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CutSceneManager").get();
	if (CutScene) CutSceneManager = GameLogicManager::GetLogicForEntity<CutSceneLogic>(CutScene->entityID);

	hasStartedFade = false;
	GamefadeElapsedTime = 0.0f;
	gameStarted = false;
	countdownTime = 3.0f;

	Emitter dust;
	dust.spawnCountMin = 1;
	dust.spawnCountMax = 2;
	dust.lifetimeMin = 0.2f;
	dust.lifetimeMax = 0.3f;
	dust.scaleMin = 0.03f;
	dust.scaleMax = 0.15f;
	dust.speedMin = 0.05f;
	dust.speedMax = 0.1f;
	dust.baseColor = { 160,160,160,255 };
	dust.layer = 0;
	dust.sortingOrder = 0;
	DuckEngine::RegisterEmitter("Dust", dust);

	Emitter sparks;
	sparks.spawnCountMin = 1;
	sparks.spawnCountMax = 2;
	sparks.lifetimeMin = 0.3f;
	sparks.lifetimeMax = 0.7f;
	sparks.scaleMin = 0.05f;
	sparks.scaleMax = 0.25f;
	sparks.speedMin = 0.5f;
	sparks.speedMax = 1.0f;
	sparks.baseColor = { 255, 100, 0, 255 };
	sparks.layer = 1;
	sparks.sortingOrder = 4;
	DuckEngine::RegisterEmitter("CookingSparks", sparks);

	// initialize seats
	seatingLocations.clear();
	auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
	for (auto& entity : entities)
	{
		if (entity->name.find("Seat_") != std::string::npos)
		{
			seatingLocations.emplace_back(entity.get(), false, std::vector<Entity*>{});

			// Find the position of the underscore
			size_t pos = entity->name.find_last_of('_');
			std::string numStr;
			if (pos != std::string::npos && pos < entity->name.length() - 1)
			{
				// Extract the substring after the underscore
				numStr = entity->name.substr(pos + 1);
			}

			// SECOND LOOP TO FIND ALL POINTS FOR MOVING TOWARDS THIS SEAT
			for (auto& entity2 : entities)
			{
				if (entity2->name.find("SeatPoint" + numStr + "_") != std::string::npos)
				{
					std::get<2>(seatingLocations.back()).emplace_back(entity2.get());
					std::cout << "Added seat point: " << entity2->name << "\n";
				}
			}

			//Entity* customerTable = nullptr;

			// SET THE TABLE LOGIC'S SEAT ENTITY ID TO THIS SEAT
			for (auto& entity2 : entities)
			{
				if (entity2->name.find("CustomerTable_" + numStr) != std::string::npos)
				{
					auto customerTable = GameLogicManager::GetLogicForEntity<CustomerTableLogic>(entity2.get()->entityID);
					customerTable.get()->seatEntity = entity.get();
					std::cout << "CUSTOMERTABLELOGIC INITED\n";
				}
			}

			customersAtSeats.emplace_back(entity.get(), nullptr);

			std::cout << "Added seat: " << entity->name << "\n";
		}
	}

	// Sort seats by number
	auto sortEntitiesSeats = [](const std::tuple<Entity*, bool, std::vector<Entity*>>& a, const std::tuple<Entity*, bool, std::vector<Entity*>>& b) {
		std::string nameA = std::get<0>(a)->name;
		std::string nameB = std::get<0>(b)->name;

		size_t posA = nameA.find_last_of('_');
		size_t posB = nameB.find_last_of('_');

		if (posA == std::string::npos || posB == std::string::npos) return false;

		std::string numStrA = nameA.substr(posA + 1);
		std::string numStrB = nameB.substr(posB + 1);

		if (!std::all_of(numStrA.begin(), numStrA.end(), ::isdigit) ||
			!std::all_of(numStrB.begin(), numStrB.end(), ::isdigit)) {
			return false;
		}

		int numA = std::stoi(numStrA);
		int numB = std::stoi(numStrB);
		return numA < numB;
		};

	std::sort(seatingLocations.begin(), seatingLocations.end(), sortEntitiesSeats);

	// Now sort each inner vector of Entity pointers
	auto sortSeatPoints = [](Entity* a, Entity* b) {
		std::string nameA = a->name;
		std::string nameB = b->name;

		// Extract numbers from the names
		size_t posA = nameA.find_last_of('_');
		size_t posB = nameB.find_last_of('_');

		if (posA == std::string::npos || posB == std::string::npos) return false;

		std::string numStrA = nameA.substr(posA + 1);
		std::string numStrB = nameB.substr(posB + 1);

		if (!std::all_of(numStrA.begin(), numStrA.end(), ::isdigit) ||
			!std::all_of(numStrB.begin(), numStrB.end(), ::isdigit)) {
			return false;
		}

		int numA = std::stoi(numStrA);
		int numB = std::stoi(numStrB);
		return numA < numB;
		};

	// Apply the sort to each inner vector
	for (auto& seatTuple : seatingLocations) {
		std::vector<Entity*>& seatPoints = std::get<2>(seatTuple);
		std::sort(seatPoints.begin(), seatPoints.end(), sortSeatPoints);
	}


	for (int i = 1; ; ++i)
	{
		std::string customerName = "Customer_" + std::to_string(i);
		Entity* customer = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(customerName).get();

		if (!customer) break;

		CustomerLogic* customerLogic = GameLogicManager::GetLogicForEntity<CustomerLogic>(customer->entityID).get();
		if (customerLogic)
		{
			customers.push_back(customerLogic);
		}
	}

	// Don't start the customer walking here, let the countdown logic handle it
	// customers[0]->StartWalking();
	customerCount = static_cast<int>(customers.size());
	timeSinceLastCustomer = 0.0f;

	
}

void GameLoopLogic::Update()
{
	if (FPSText) FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U))
	{
		if (FPSText)
		{
			FPSText->isEnabled = !FPSText->isEnabled;
			GameManager::SetGlobalVariable("ShowFPS", FPSText->isEnabled ? "true" : "false");
		}
	}

	if (CutScene && CutSceneManager && CutSceneManager->CutscenePlay()) return;


	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	if (!gameStarted) {
		static int lastDisplayedNumber = -1;

		countdownTime -= DuckEngine::DeltaTime();
		int displayNumber = static_cast<int>(ceil(countdownTime));

		if (CountdownText) {
			CountdownText->isEnabled = true;

			if (displayNumber != lastDisplayedNumber) {
				lastDisplayedNumber = displayNumber;

				if (displayNumber > 0) {
					TimeLeftSound->Play(3);
					CountdownText->text = std::to_string(displayNumber) + "..";
				}
				else {
					TimeLeftSound->Play(4);
					CountdownText->text = "Go!";
					currentCustomerIndex = 0;
					
					// Start only the first customer when game begins
					if (!customers.empty() && !isSpawningCustomer) {
						customers[0]->StartWalking();
						isSpawningCustomer = true;
						customerSpawnCooldown = 3.0f;
					}
				}
			}

			if (countdownTime <= -0.5f) {
				CountdownText->isEnabled = false;
				gameStarted = true;
			}
		}

		if (FadeOutSprite) {
			FadeOutSprite->isVisible = true;
			float fadeValue = (countdownTime / 3.0f) * 255.0f;
			FadeOutSprite->color.a = (fadeValue >= 0.0f) ? fadeValue : 0;
		}

		return;
	}
	else 
	{
		if (!isSpawningCustomer) 
		{
			if (!customerQueuePaused) 
			{
				timeSinceLastCustomer += DuckEngine::DeltaTime();
			}
		}
		else
		{
			customerSpawnCooldown -= DuckEngine::DeltaTime();
			if (customerSpawnCooldown <= 0.0f)
			{
				isSpawningCustomer = false;
				customerSpawnCooldown = 3.0f;
				customerQueuePaused = false;
			}
		}

		bool anyCustomerAtCounter = false;
		for (auto* customer : customers) 
		{
			if (customer->stateMachine.currentState == customer->WaitingOrderState) 
			{
				anyCustomerAtCounter = true;
				break;
			}

			// Also check for customers walking to counter
			if (customer->stateMachine.currentState == customer->WalkState) {
				CustomerWalkState* walkState = customer->WalkState.get();
				if (!walkState->isOrderTaken &&
					!walkState->customerAngryLeave &&
					walkState->currentTargetIndex < walkState->queueTargets.size()) {
					anyCustomerAtCounter = true;
					break;
				}
			}
		}

		if (anyCustomerAtCounter) 
		{
			customerQueuePaused = true;
		}

		if (timeSinceLastCustomer >= customerSpawnInterval &&
			currentCustomerIndex < customerCount - 1 &&
			!isSpawningCustomer) 
		{

			bool anyCustomerWaiting = false;
			bool anyCustomerWalking = false;
			
			// Check if any customer is waiting to place an order or is walking to the counter
			for (auto* customer : customers) 
			{
				// Check if any customer is in waiting state
				if (customer->WaitingOrderState->GetIsOrderTaken() == false &&
					customer->stateMachine.currentState == customer->WaitingOrderState) 
				{
					anyCustomerWaiting = true;
					break;
				}
				
				// Check if the most recently spawned customer is still walking to the counter
				// This prevents spawning the next customer while current one is still approaching
				if (customer->stateMachine.currentState == customer->WalkState &&
				    currentCustomerIndex > 0 && // Only for customers after the first one
				    customers[currentCustomerIndex] == customer) // Only check most recent customer
				{
					// Check if customer is walking to take the order (not leaving/going to seat)
					if (!customer->WalkState.get()->isOrderTaken && 
					    !customer->WalkState.get()->customerAngryLeave)
					{
						anyCustomerWalking = true;
						break;
					}
				}
			}
		
		// Only spawn a new customer if no customer is waiting or walking to counter
		if (!anyCustomerWaiting && !anyCustomerWalking) 
		{
			timeSinceLastCustomer = 0.0f;
			isSpawningCustomer = true;
			
			currentCustomerIndex++;
			std::cout << "Spawning next customer due to time interval" << std::endl;
			customers[currentCustomerIndex]->StartWalking();
			// Reset cooldown timer to prevent immediate spawning of next customer
			customerSpawnCooldown = 3.0f;
		}
		}
	}


	//if (pauseMenuLogic)
	//{
	//	if (!pauseMenuLogic->isPaused)
	//	{
	//		CameraManager::LerpCameraTo(duckTrans->GetPosition().x, duckTrans->GetPosition().y);
	//	}

	//}

	for (const auto& [entityId, sComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) 
	{
		SoundComponent* soundComponent = static_cast<SoundComponent*>(sComponent.get());
		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	if (timerText) {
		if (timeLeft > 0.f) {
			timeLeft -= DuckEngine::DeltaTime();
			int minutes = static_cast<int>(timeLeft) / 60;
			int seconds = static_cast<int>(timeLeft) % 60;

			std::string secStr = (seconds < 10 ? "0" : "") + std::to_string(seconds);
			timerText->text = std::to_string(minutes) + ":" + secStr;

			if (timeLeft < 10.f && TimeLeftSound != nullptr) {
				timerText->color = { 255, 0, 0, 255 };
				TimeLeftSound->Play(0);
				GamefadeElapsedTime = 0.0f;
			}
		}

		else {
			timerText->text = "END!";

			if (!hasStartedFade && TimeLeftSound != nullptr) {
				SoundSystem::StopSounds(TimeLeftSound->soundID[0]);
				TimeLeftSound->Play(1);
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
					FadeOutSprite->color.a = fadeProgress * 255.0f;
				}
			}
		}
	}

	

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_M) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_SHIFT))
	{
		std::cout << "H is pressed!\n";
		GameManager::SetActiveScene("EndScene");
	}

	Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station").get();
	if(submit)
	{
		auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(submit->entityID);

		if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_N) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_SHIFT))
		{
			submitLogic->increaseScore(10);
		}
		ScoreLogic::scoreValue = submitLogic->getScore();
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_O) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_SHIFT))
	{
		timeLeft = 11.f;
	}

	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_P))
	{
		std::cout << "P is pressed!\n";
		DuckEngine::ToggleShowDebugColliders();
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE) || DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START))
	{
		std::cout << "Escape is pressed!\n";
		if (pauseMenuLogic)
		{		
			pauseMenuLogic->PauseGame(!pauseMenuLogic->isPaused);	
			pauseMenuLogic->playPauseSound();
		}
	}

	
}

void GameLoopLogic::FixedUpdate()
{
}

bool GameLoopLogic::IsGameStarted()
{
	return gameStarted;
}

bool GameLoopLogic::IsSeatOccupied(Entity* seat)
{
	for (auto& pairSeat : seatingLocations)
	{
		if (std::get<0>(pairSeat) == seat)
		{
			return std::get<1>(pairSeat);
		}
	}
	return false;
}

bool GameLoopLogic::OccupySeat(Entity* seat)
{
	for (auto& pairSeat : seatingLocations)
	{
		if (std::get<0>(pairSeat) == seat && !std::get<1>(pairSeat))
		{
			std::get<1>(pairSeat) = true;
			return true;
		}
	}
	return false;
}

void GameLoopLogic::FreeSeat(Entity* seat)
{
	for (auto& pairSeat : seatingLocations)
	{
		if (std::get<0>(pairSeat) == seat)
		{
			std::get<1>(pairSeat) = false;
			return;
		}
	}
}

void GameLoopLogic::ResetCustomerQueue()
{
	customerQueuePaused = true;

	timeSinceLastCustomer = 0.0f;
	isSpawningCustomer = true;
	customerSpawnCooldown = 6.0f;

	std::cout << "Customer queue reset - pausing new spawns temporarily" << std::endl;
}