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
	DuckEngine::SetCameraHeight(25);

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
			timerText->text = "5:00";
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

	customers[0]->StartWalking();

	customerCount = customers.size();
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


	if (!pauseMenuLogic)
	{
		auto pauseMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
		if (pauseMenu)
		{
			pauseMenuLogic = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(pauseMenu->entityID);
		}
	}

	if (!gameSettingsLogic)
	{
		auto settingsMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn").get();
		if (settingsMenu)
		{
			gameSettingsLogic = GameLogicManager::GetLogicForEntity<GameSettingsLogic>(settingsMenu->entityID);
		}
	}

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

	if (pauseMenuLogic)
	{
		if (!pauseMenuLogic->isPaused)
		{
			CameraManager::LerpCameraTo(duckTrans->GetPosition().x, duckTrans->GetPosition().y);
		}

	}

	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());
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

	

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_M))
	{
		std::cout << "H is pressed!\n";
		GameManager::SetActiveScene("EndScene");
	}

	Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station").get();
	auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(submit->entityID);

	if (submitLogic && submitLogic->CheckNewOrder()) {
		UpdateOrderTexture();
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_N))
	{
		submitLogic->increaseScore(10);
	}
	ScoreLogic::scoreValue = submitLogic->getScore();

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_O))
	{
		timeLeft = 11.f;
	}

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
		std::cout << "Escape is pressed!\n";
		TimeLeftSound->Play(2);
		if (pauseMenuLogic)
		{
			pauseMenuLogic->PauseGame(!pauseMenuLogic->isPaused);
		}
	}

	
}

void GameLoopLogic::FixedUpdate()
{
}

void GameLoopLogic::UpdateOrderTexture()
{
	auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OrderTab->entityID);
	if (spriteRenderer) {
		spriteRenderer->isVisible = false;
	}
}

bool GameLoopLogic::IsGameStarted()
{
	return gameStarted;
}
