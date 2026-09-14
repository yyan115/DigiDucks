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
	fmodLogo = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FMODLogo").get();
	peripheralNotice = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("PeripheralNotice").get();

	fadeRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fadeScreen->entityID);
	logoRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(logo->entityID);
	logo2Renderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(logo2->entityID);
	fmodLogoRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fmodLogo->entityID);
	if (peripheralNotice)
	{
		peripheralNoticeRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(peripheralNotice->entityID);
	}

	fadeRenderer->color.a = 255;
	fadeRenderer->isVisible = true;

	logoRenderer->isVisible = true;
	logo2Renderer->isVisible = false;
	fmodLogoRenderer->isVisible = false;

	logoRenderer->color.a = 0;
	logo2Renderer->color.a = 0;
	fmodLogoRenderer->color.a = 0;

	if (peripheralNoticeRenderer)
	{
		peripheralNoticeRenderer->isVisible = false;
		peripheralNoticeRenderer->color.a = 255;
	}

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
namespace
{
	// The phase timer is advanced before the fade values below are computed,
	// so a frame long enough to overshoot the fade duration makes the ratio
	// exceed one. Casting the resulting out-of-range float to unsigned char is
	// undefined behaviour: UndefinedBehaviorSanitizer reported values such as
	// -770 and 1025 here. Clamping the progress leaves every in-range frame
	// byte-identical and makes a long frame land on a fully faded value
	// instead of whatever the cast happened to wrap to.
	float FadeProgress(float elapsed, float duration)
	{
		if (duration <= 0.0f)
		{
			return 1.0f;
		}
		const float progress = elapsed / duration;
		if (progress < 0.0f) return 0.0f;
		if (progress > 1.0f) return 1.0f;
		return progress;
	}
}

void Intro::ShowPeripheralNotice()
{
	// The notice is on the UI layer with the fade screen, above it by sorting
	// order. Layer beats sorting order in SpriteRendererSystem, so a notice on
	// the Gameplay layer is covered by the fade however high its order is.
	// Leaving the fade opaque gives the notice the same black backdrop the
	// logos had, without needing a background of its own.
	if (logoRenderer) { logoRenderer->isVisible = false; }
	if (logo2Renderer) { logo2Renderer->isVisible = false; }
	if (fmodLogoRenderer) { fmodLogoRenderer->isVisible = false; }
	if (fadeRenderer) { fadeRenderer->color.a = 255; }
	if (peripheralNoticeRenderer) { peripheralNoticeRenderer->isVisible = true; }
	phaseTimer = 0.0f;
	currentPhase = IntroPhase::PERIPHERAL_NOTICE;
}

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
		if (currentPhase == IntroPhase::FADE_IN_LOGO ||
			currentPhase == IntroPhase::SHOW_LOGO ||
			currentPhase == IntroPhase::FADE_OUT_LOGO)
		{
			logoRenderer->isVisible = false;
			logo2Renderer->isVisible = true;
			fmodLogoRenderer->isVisible = true;
			logo2Renderer->color.a = 255;
			fmodLogoRenderer->color.a = 255;
			fadeRenderer->color.a = 0;
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::SHOW_LOGO2;
		}
		else if (currentPhase != IntroPhase::PERIPHERAL_NOTICE)
		{
			// The logos are done, so the supported-input notice is next
			// rather than the menu: skipping the logos should not skip the
			// screen the Gallery asks for before the menu.
			ShowPeripheralNotice();
		}
		else
		{
			currentPhase = IntroPhase::COMPLETE;
			GameManager::SetActiveScene("MainMenu");
		}
		return;
	}

	float dt = DuckEngine::DeltaTime();

	// Loading the scene's assets happens between the clock's last tick and the
	// first Update, so that first delta is the loading time rather than a
	// frame: measured here at about 2.7 seconds. Added to the phase timer it
	// finishes the two second fade before anything is drawn, and the logo
	// appears at full brightness with no fade at all. The Gallery asks for the
	// fade, so the first delta is dropped.
	if (firstUpdate)
	{
		firstUpdate = false;
		dt = 0.0f;
	}

	phaseTimer += dt;

	switch (currentPhase) {
	case IntroPhase::FADE_IN_LOGO:
		fadeRenderer->color.a = static_cast<unsigned char>(255 - FadeProgress(phaseTimer, fadeDuration) * 255);
		logoRenderer->color.a = static_cast<unsigned char>(FadeProgress(phaseTimer, fadeDuration) * 255);
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
		fadeRenderer->color.a = static_cast<unsigned char>(FadeProgress(phaseTimer, fadeDuration) * 255);
		logoRenderer->color.a = static_cast<unsigned char>(255 - FadeProgress(phaseTimer, fadeDuration) * 255);
		if (phaseTimer >= fadeDuration) {
			logoRenderer->isVisible = false;
			logo2Renderer->isVisible = true;
			fmodLogoRenderer->isVisible = true;
			phaseTimer = 0.0f;
			currentPhase = IntroPhase::FADE_IN_LOGO2;
			logo2Renderer->color.a = 0;
			fmodLogoRenderer->color.a = 0;
		}
		break;

	case IntroPhase::FADE_IN_LOGO2:
		fadeRenderer->color.a = static_cast<unsigned char>(255 - FadeProgress(phaseTimer, fadeDuration) * 255);
		logo2Renderer->color.a = static_cast<unsigned char>(FadeProgress(phaseTimer, fadeDuration) * 255);
		fmodLogoRenderer->color.a = logo2Renderer->color.a;
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
		fadeRenderer->color.a = static_cast<unsigned char>(FadeProgress(phaseTimer, fadeDuration) * 255);
		logo2Renderer->color.a = static_cast<unsigned char>(255 - FadeProgress(phaseTimer, fadeDuration) * 255);
		fmodLogoRenderer->color.a = logo2Renderer->color.a;
		if (phaseTimer >= fadeDuration) {
			ShowPeripheralNotice();
		}
		break;

	case IntroPhase::PERIPHERAL_NOTICE:
		if (phaseTimer >= noticeDuration) {
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
