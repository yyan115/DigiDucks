#include "ExitConfirmLogic.h"
#include "GameManager.h"
#include "ProjectSettings.h"

void ExitConfirmLogic::Start()
{
	 auto gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
	 if (gamePauseBg)
	 {
		 pauseMenuLogic = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(gamePauseBg->entityID);
	 }

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
		gameExitYesBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitYesBtn->entityID);
		if (gameExitYesButton)
		{
			gameExitYesButton->onClick = [this]() {
				gameExitYesBtnSound->Resume();
				gameExitYesBtnSound->Play();
				// need to transition
				GameManager::SetActiveScene("MainMenu"); };
			gameExitYesButton->onHover = [this]() {
				gameExitYesBtnSound->Resume();
				gameExitYesBtnSound->Play(1);
				gameExitYesBtnSpt->texture = gameExitYesBtn_Hover; };
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
		gameExitNoBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitNoBtn->entityID);
		if (gameExitNoButton)
		{
			gameExitNoButton->onClick = [this]() {
				gameExitNoBtnSound->Resume();
				gameExitNoBtnSound->Play();
				if(pauseMenuLogic)
					pauseMenuLogic->PauseGame(true);
				};
			gameExitNoButton->onHover = [this]() {
				gameExitNoBtnSound->Resume();
				gameExitNoBtnSound->Play(1);
				gameExitNoBtnSpt->texture = gameExitNoBtn_Hover; };
			gameExitNoButton->onFinishHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Normal; };
		}
	}
}

void ExitConfirmLogic::Update()
{
}

void ExitConfirmLogic::FixedUpdate()
{
}

void ExitConfirmLogic::ShowExitConfirm(bool state)
{
	if (gameExitCfmBg && gameExitCfmBgSpt)
	{
		gameExitCfmBgSpt->isVisible = state;
	}
}