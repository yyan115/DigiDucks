/******************************************************************************/
/*!
\file     CutSceneLogic.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the CutSceneLogic class
		  which is responsible for handling the cutscene logic in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"

class CutSceneLogic : public GameLogic
{
private:
	SpriteRendererComponent* CutSceneSprite = nullptr;
	SoundComponent* CutSceneSFX = nullptr;
	SoundComponent* CutSceneBGM = nullptr;
	SpriteRendererComponent* FadeOutSprite = nullptr;
	SpriteRendererComponent* DialogueSprite = nullptr;
	Entity* CutSceneButton = nullptr;
	Entity* DialogueButton = nullptr;
	Entity* CutSceneEntity = nullptr;
	int currentCutsceneIndex = 0;
	float cutsceneTimer = 0.0f;
	float fadeProgress = 0.0f;
	// The dialogue is fourteen pictures, Dialogues/intro/1.png to 14.png, and
	// opens on the first. The index is the picture on screen.
	static constexpr int kFirstDialogueLine = 1;
	static constexpr int kLastDialogueLine = 14;
	int currentDialogueIndex = kFirstDialogueLine;
	float DialoguefadeProgress = 0.0f;
	
	bool isPlaying = true;
	bool isFading = true;
	bool isCutSceneFading = false;
	bool isShowingDialogue = false;
	std::string lastPlayedSceneName = "";

	// The gear opens the pause menu, and the pause menu or the options panel
	// can be up over the dialogue, which moves on for any click. Input meant
	// for them, or for the gear, must not also skip a line of dialogue or the
	// whole cutscene underneath. The gear is hidden while the cutscene's
	// pictures are up: they draw over the pause menu, and the game has never
	// paused there.
	SpriteRendererComponent* SettingsPanelSprite = nullptr;
	ButtonComponent* SettingsGearButton = nullptr;
	SpriteRendererComponent* SettingsGearSprite = nullptr;
	bool menuWasUp = false;
	bool MenuHasInput() const;

	// SKIP starts the closing fade on the press, from the picture on screen,
	// and runs it four times as fast. It used to set the picture one short of
	// the last and leave the rest to the timer, so nothing changed for up to
	// a second and a half and the press looked missed.
	bool skipped = false;
	void SkipPictures();
	// Ends the dialogue and hands the level over, as the last line does.
	void EndDialogue();
public:

	CutSceneLogic() :
		GameLogic(nullptr){}

	CutSceneLogic(GameLogicComponent* component) :
		GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	
	/****************************************************************
	* @brief Start function for the Cutscene Logic.
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Cutscene Logic.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Fixed Update function for the Cutscene Logic.
	* ****************************************************************/
	void FixedUpdate() override {}

	/****************************************************************
	* @brief Checks if the cutscene is playing.
	* ****************************************************************/
	bool CutscenePlay();

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CutSceneLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};