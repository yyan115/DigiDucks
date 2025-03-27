/******************************************************************************/
/*!
\file       ExitConfirmLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 4 2025
\brief      Declartion of all Exit Confirm Logic functions
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "PauseMenuLogic.h"
class ExitConfirmLogic : public GameLogic
{
private:
	// Exit Background
	Entity* gameExitCfmBg = nullptr;
	SpriteRendererComponent* gameExitCfmBgSpt = nullptr;
	// Yes Button
	ButtonComponent* gameExitYesButton = nullptr;
	SpriteRendererComponent* gameExitYesBtnSpt = nullptr;
	TransformComponent* gameExitYesBtnTransform = nullptr;
	Vec2 gameExitYesBtnOriginalScale;
	Texture gameExitYesBtn_Normal{};
	Texture gameExitYesBtn_Hover{};
	SoundComponent* gameExitYesBtnSound = nullptr;
	// No Button
	ButtonComponent* gameExitNoButton = nullptr;
	SpriteRendererComponent* gameExitNoBtnSpt = nullptr;
	TransformComponent* gameExitNoBtnTransform = nullptr;
	Vec2 gameExitNoBtnOriginalScale;
	Texture gameExitNoBtn_Normal{};
	Texture gameExitNoBtn_Hover{};
	SoundComponent* gameExitNoBtnSound = nullptr;
	// PauseMenu Logic
	std::shared_ptr<PauseMenuLogic> pauseMenuLogic = nullptr;

	// Button scale increase factor
	const float buttonScaleIncrease = 1.1f;

	// Gamepad navigation
	enum class MenuSelection
	{
		YES = 0,
		NO = 1
	};

	MenuSelection currentSelection = MenuSelection::NO;
	bool isUsingController = false;
	float controllerNavigationCooldown = 0.0f;
	const float controllerNavigationDelay = 0.2f;

	void UpdateMenuSelection();
	void SelectButton(MenuSelection selection);
	void DeselectAllButtons();
	void ActivateSelectedButton();

public:
	ExitConfirmLogic() : GameLogic(nullptr) {}
	ExitConfirmLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}
	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<ExitConfirmLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
	/****************************************************************
	* @brief Start function for the Exit Confirm Logic
	* ****************************************************************/
	void Start() override;
	/****************************************************************
	* @brief Update function for the Exit Confirm Logic
	* ****************************************************************/
	void Update() override;
	/****************************************************************
	* @brief FixedUpdate function for the Exit Confirm Logic
	* ****************************************************************/
	void FixedUpdate() override;
	/****************************************************************
	* @brief Shows the exit confirmation menu.
	*
	* @param state - true if the exit confirmation menu should be shown,
	* * false if it should be hidden.
	* ****************************************************************/
	void ShowExitConfirm(bool state);
};
