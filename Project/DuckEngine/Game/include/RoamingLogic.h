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
#include <set>

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

struct Node {
    int x, y;            // Position of the node (can be in any coordinate system)
    bool isObstacle;     // Flag to check if it's an obstacle
    int gCost;           // Cost from the start node
    int hCost;           // Heuristic cost to the goal node
    int fCost() const { return gCost + hCost; } // f = g + h (used to prioritize nodes)
    Node* parent;        // Pointer to parent node in the path

    Node(int x, int y, bool isObstacle = false)
        : x(x), y(y), isObstacle(isObstacle), gCost(0), hCost(0), parent(nullptr) {}

    // For easier comparison in the priority queue
    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

using NodeList = std::vector<Node>;
using NodePtr = Node*;



std::vector<Node*> getNeighbors(Node* node, const std::vector<std::vector<Node>>& nodes); 

std::vector<Node*> AStar(Node* start, Node* goal, const std::vector<Node*>& allNodes);

