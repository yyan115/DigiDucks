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

class EndCutSceneLogic : public GameLogic
{
private:
	SpriteRendererComponent* CutSceneSprite = nullptr;
	SoundComponent* CutSceneSFX = nullptr;
	SoundComponent* CutSceneBGM = nullptr;
	SpriteRendererComponent* FadeOutSprite = nullptr;
	Entity* CutSceneButton = nullptr;
	Entity* CutSceneEntity = nullptr;
	int currentCutsceneIndex = 0;
	float cutsceneTimer = 0.0f;
	float fadeProgress = 0.0f;
	int currentDialogueIndex = 0;
	float DialoguefadeProgress = 0.0f;
	
	bool isPlaying = true;
	bool isFading = true;
	bool isCutSceneFading = false;
	std::string lastPlayedSceneName = "";
public:

	EndCutSceneLogic() :
		GameLogic(nullptr){}

	EndCutSceneLogic(GameLogicComponent* component) :
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
		auto clone = std::make_shared<EndCutSceneLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};