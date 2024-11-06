#pragma once
#include "DuckEngine.h"
#include <unordered_map>
#include <iostream>

enum ObjectState
{
	Idle,
	Moving,
	Chasing
};

// Structure to hold individual state data for each duck
struct ObjectDatas {
	float objectRange = 5.f;
	float waitTime = 5.f;
	float objectSpeed = 3.f;
	bool reachedPos = false;
	bool firstToSecond = true;
	ObjectState state = Moving;
	float timer = 0.f;
};


void RoamSelectedObject(std::string prefabName, Vec2 firstPos, Vec2 secondPos);

// Function to handle roaming logic
void RoamChar(int objectID, Vec2& firstPos, Vec2& secondPos);
