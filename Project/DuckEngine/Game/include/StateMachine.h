/******************************************************************************/
/*!
\file       StateMachine.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the StateMachine class template, which manages
			state transitions for entities in DuckEngine. The class
			handles switching between states, updating the current state,
			and executing state-specific logic in the Update and FixedUpdate
			methods.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include "DuckEngine.h"
#include "State.h"

template<typename T>
class StateMachine
{
public:
	std::shared_ptr<State<T>> currentState;

	StateMachine() : currentState(nullptr) {}

	void ChangeState(std::shared_ptr<State<T>> newState)
	{
		if (currentState != nullptr)
		{
			currentState->Exit();
		}

		currentState = std::move(newState);

		if (currentState != nullptr)
		{
			currentState->Enter();
		}
	}

	void Update()
	{
		if (currentState != nullptr)
		{
			currentState->Update();
		}
	}

	void FixedUpdate()
	{
		if (currentState != nullptr)
		{
			currentState->FixedUpdate();
		}
	}
};