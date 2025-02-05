/******************************************************************************/
/*!
\file       RestockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Declaration of the Restock Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
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
	
	int currentCutsceneIndex = 0;
	float cutsceneTimer = 0.0f;
	float fadeProgress = 0.0f;
	int currentDialogueIndex = 0;
	float dialogueTimer = 0.0f;
	float DialoguefadeProgress = 0.0f;
	
	bool isPlaying = true;
	bool isFading = true;
	bool isCutSceneFading = false;
	bool isShowingDialogue = false;
public:

	CutSceneLogic() :
		GameLogic(nullptr){}

	CutSceneLogic(GameLogicComponent* component) :
		GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	
	/****************************************************************
	* @brief Start function for the Restock Station Logic.
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Restock Station Logic.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Fixed Update function for the Restock Station Logic.
	* ****************************************************************/
	void FixedUpdate() override {}

	bool CutscenePlay();

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CutSceneLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};