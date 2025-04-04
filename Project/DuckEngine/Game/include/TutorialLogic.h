/******************************************************************************/
/*!
\file       TutorialLogic.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the TutorialLogic class
*/
/******************************************************************************/


#pragma once

#include "DuckEngine.h"

class TutorialLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<TutorialLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void ShowNextTexture();

};
