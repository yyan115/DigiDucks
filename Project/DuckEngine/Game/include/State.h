#pragma once

#include "DuckEngine.h"


class State
{
public:
	GameLogic* owner;
	
	explicit State(GameLogic* gameLogicOwner) : owner(gameLogicOwner) {}

	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;



};