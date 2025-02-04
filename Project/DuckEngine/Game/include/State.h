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
