#pragma once
#include "DuckEngine.h"

enum ObjectState
{
	Idle,
	Moving,
	Chasing
};

// Distance needed for player to be in range of object
const float objectRange = 3.f;

// Wait Time for object.
const float waitTime = 5.f;

// Object Speed
const float objectSpeed = 3.f;

// Function to handle roaming logic
void RoamChar(Entity& object, Vec2& firstPos, Vec2& secondPos);