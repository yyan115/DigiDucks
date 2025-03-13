/******************************************************************************/
/*!
\file       State.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the generic State class template, which serves as the
			foundation for implementing a finite state machine (FSM) in
			DuckEngine. This class is designed to be inherited by specific
			state implementations, providing core lifecycle methods such as
			Enter, Update, FixedUpdate, and Exit.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"


template <typename T>
class State
{
public:
	T* owner;

	explicit State(T* owner) : owner(owner) {}
	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Exit() = 0;
};
