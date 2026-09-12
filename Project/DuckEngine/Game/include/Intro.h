/******************************************************************************/
/*!
\file     Intro.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the Intro class
		  which is responsible for handling the Intro scene logic in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Scene.h"
#include "DuckEngine.h"

class Intro : public Scene {
public:
	/****************************************************************
	* @brief Load all necessary resources for the scene.
	* This function is called before the scene starts.
	* ****************************************************************/
	void Load() override;

	/****************************************************************
   * @brief Starts the scene, initializing game objects and setting up the initial state.
   * This function is called when the scene begins running.
   * ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Updates the scene logic each frame, including processing input and updating game objects.
	* This function is called every frame.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Performs any operations after the main update logic, such as cleanup or post-processing.
	* This function is called every frame, after the Update() method.
	* ****************************************************************/
	void PostUpdate() override;

	/****************************************************************
	* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
	* ****************************************************************/
	void Exit() override;

	/****************************************************************
	* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
	* ****************************************************************/
	void Unload() override;

private:
	enum class IntroPhase {
		FADE_IN_LOGO,
		SHOW_LOGO,
		FADE_OUT_LOGO,
		FADE_IN_LOGO2,
		SHOW_LOGO2,
		FADE_OUT_LOGO2,
		FADE_IN_GAMEPAD,
		SHOW_GAMEPAD,
		FADE_OUT_GAMEPAD,
		COMPLETE
	};
	IntroPhase currentPhase = IntroPhase::FADE_IN_LOGO;
	float phaseTimer = 0.0f;
	float fadeDuration = 2.0f;
	float displayDuration = 2.0f;
	Entity* fadeScreen = nullptr;
	Entity* logo = nullptr;
	Entity* logo2 = nullptr;
	Entity* fmodLogo = nullptr;
	Entity* gamepadNotice = nullptr;
	SpriteRendererComponent* fadeRenderer = nullptr;
	SpriteRendererComponent* logoRenderer = nullptr;
	SpriteRendererComponent* logo2Renderer = nullptr;
	SpriteRendererComponent* fmodLogoRenderer = nullptr;
	SpriteRendererComponent* gamepadNoticeRenderer = nullptr;

};
