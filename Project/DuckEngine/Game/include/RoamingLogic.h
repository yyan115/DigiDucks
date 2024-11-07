/******************************************************************************/
/*!
\file		RoamingLogic.h
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Nov 6 2024
\brief		Declaration of Roaming Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "DuckEngine.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>


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

/**
* @brief
* Function to roam any object with a specific prefab name between two positions
* 
* @param prefabName - The name of the prefab to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedPrefab(std::string prefabName, Vec2 firstPos, Vec2 secondPos);

/**
* @brief
* Function to roam any object with a specific object name between two positions
*	
* @param objectName - The name of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedObject(std::string objectName, Vec2 firstPos, Vec2 secondPos);

/**
* @brief
* Function to roam any object with a specific object ID between two positions
*	
* @param objectID - The ID of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamTwoPos(int objectID, Vec2& firstPos, Vec2& secondPos);


/*****************************************************************************************/


/**
* @brief
* Function to roam any object with a specific prefab name in a specific direction
* for a specific time
* 
* @param prefabName - The name of the prefab to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamSelectedPrefab(std::string prefabName, Vec2 dir, float time);

/**
* @brief
* Function to roam any object with a specific object name in a specific direction
* for a specific time
* 
* @param objectName - The name of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamSelectedObject(std::string prefabName, Vec2 dir, float time);

/**
* @brief
* Function to roam any object with a specific object ID in a specific direction
* for a specific time
*	
* @param objectID - The ID of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamDir(int objectID, Vec2& dir, float time);

// Position struct representing a 2D point in space
struct Position {
    float x, y;

    Position(float x = 0, float y = 0) : x(x), y(y) {}

    // Calculate the Euclidean distance from another position
    float distanceTo(const Position& other) const {
        return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
    }

    // Move towards a target position by a step
    void moveTowards(const Position& target, float step = 1.0f) {
        float angle = std::atan2(target.y - y, target.x - x);
        x += std::cos(angle) * step;  // Move in x direction
        y += std::sin(angle) * step;  // Move in y direction
    }

    // Heuristic: Euclidean distance to the target position
    float heuristic(const Position& target) const {
        return distanceTo(target);
    }
};

// A* algorithm in a continuous space
std::vector<Position> aStarPathfinding(Position start, Position goal, float maxStep = 1.0f);