/******************************************************************************/
/*!
\file     Intro.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the Intro class
		  which is responsible for handling the Intro scene logic in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Intro.h"
#include "DuckEngine_Input.h"
#include "Scene.h"
#include "GameManager.h"

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

void Intro::Load()
{
	Scene::Load();
	DuckEngine::EnableLogging(false);
	CameraManager::SetHeight(20);

	fadeScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeScreen").get();
	logo = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Logo").get();
	logo2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Logo2").get();

	fadeRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fadeScreen->entityID);
	logoRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(logo->entityID);
	logo2Renderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(logo2->entityID);

	fadeRenderer->color.a = 255;
	fadeRenderer->isVisible = true;

	logoRenderer->isVisible = true;
	logo2Renderer->isVisible = false;

	logoRenderer->color.a = 0;
	logo2Renderer->color.a = 0;

	phaseTimer = 0.0f;
	currentPhase = IntroPhase::FADE_IN_LOGO;


}


/****************************************************************
* @brief Starts the scene, initializing game objects and setting up the initial state.
* This function is called when the scene begins running.
* ****************************************************************/
void Intro::Start()
{
	Scene::Start();
}

/****************************************************************
* @brief Updates the scene logic each frame, including processing input and updating game objects.
* This function is called every frame.
* ****************************************************************/
void Intro::Update()
{
	const bool skipRequested =
		DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
		DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT) ||
		DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER) ||
		DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_SPACE) ||
		DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE) ||
		(DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1) &&
			(DuckEngine_Input::IsGamepadButtonPressed(
				DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START) ||
			 DuckEngine_Input::IsGamepadButtonPressed(
				 DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A)));

	if (skipRequested)
	{
		currentPhase = IntroPhase::COMPLETE;
		GameManager::SetActiveScene("MainMenu");
		return;
	}

	float dt = DuckEngine::DeltaTime();
	phaseTimer += dt;

	switch (currentPhase) {
	case IntroPhase::FADE_IN_LOGO:
		fadeRenderer->color.a = static_cast<unsigned char>(255 - (phaseTimer / fadeDuration) * 255);
		logoRenderer->color.a = static_cast<unsigned char>((phaseTimer / fadeDuration) * 255);
		if (phaseTimer >= fadeDuration) {
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::SHOW_LOGO;
			fadeRenderer->color.a = 0;
		}
		break;

	case IntroPhase::SHOW_LOGO:
		if (phaseTimer >= displayDuration) {
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::FADE_OUT_LOGO;
		}
		break;

	case IntroPhase::FADE_OUT_LOGO:
		fadeRenderer->color.a = static_cast<unsigned char>((phaseTimer / fadeDuration) * 255);
		logoRenderer->color.a = static_cast<unsigned char>(255 - (phaseTimer / fadeDuration) * 255);
		if (phaseTimer >= fadeDuration) {
			logoRenderer->isVisible = false;
			logo2Renderer->isVisible = true;
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::FADE_IN_LOGO2;
			logo2Renderer->color.a = 0;
		}
		break;

	case IntroPhase::FADE_IN_LOGO2:
		fadeRenderer->color.a = static_cast<unsigned char>(255 - (phaseTimer / fadeDuration) * 255);
		logo2Renderer->color.a = static_cast<unsigned char>((phaseTimer / fadeDuration) * 255);
		if (phaseTimer >= fadeDuration) {
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::SHOW_LOGO2;
			fadeRenderer->color.a = 0;
		}
		break;

	case IntroPhase::SHOW_LOGO2:
		if (phaseTimer >= displayDuration) {
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::FADE_OUT_LOGO2;
		}
		break;

	case IntroPhase::FADE_OUT_LOGO2:
		fadeRenderer->color.a = static_cast<unsigned char>((phaseTimer / fadeDuration) * 255);
		logo2Renderer->color.a = static_cast<unsigned char>(255 - (phaseTimer / fadeDuration) * 255);
		if (phaseTimer >= fadeDuration) {
			currentPhase = IntroPhase::COMPLETE;
			GameManager::SetActiveScene("MainMenu");
		}
		break;

	default:
		break;
	}

}

/****************************************************************
* @brief Performs any operations after the main update logic, such as cleanup or post-processing.
* This function is called every frame, after the Update() method.
* ****************************************************************/
void Intro::PostUpdate()
{

}

/****************************************************************
* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
* ****************************************************************/
void Intro::Exit()
{

}

/****************************************************************
* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
* ****************************************************************/
void Intro::Unload()
{
	// base unload
	Scene::Unload();
}
