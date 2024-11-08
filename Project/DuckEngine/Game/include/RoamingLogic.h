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
#include <functional>
#include <unordered_set>
#include <queue>

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

namespace std {
	template <>
	struct hash<Vec2> {
		size_t operator()(const Vec2& p) const {
			return hash<int>()(p.x) ^ hash<int>()(p.y);
		}
	};
}

struct Node {
	Vec2 position;
	float g;     // Cost from start to this node
	float h;     // Heuristic from this node to the goal (player's position)
	float f;     // f = g + h
	Node* parent; // Parent node for path reconstruction

	bool operator==(const Node& other) const {
		return position.x == other.position.x && position.y == other.position.y;
	}

	bool operator<(const Node& other) const {
		return f > other.f; // Priority queue uses this for sorting
	}
};

struct CompareNodes {
	bool operator()(const Node& a, const Node& b) {
		return a.f > b.f;  // Min-heap based on 'f' value (f = g + h)
	}
};

struct NodeHash {
	size_t operator()(const Node& node) const {
		size_t hash1 = std::hash<float>()(node.position.x);  // Hash for the x coordinate
		size_t hash2 = std::hash<float>()(node.position.y);  // Hash for the y coordinate
		return hash1 ^ (hash2 << 1);  // Combine the hashes
	}
};

// Heuristic function: Euclidean distance from the current node to the goal (player's position)
float heuristic(const Node& current, const Node& goal);

// Get neighboring nodes (step towards the player, adjusted for smooth movement)
std::vector<Node> get_neighbors(const Node& current, const Node& player, float step_size);

// Function to check if a point (circle) intersects with the bounding box (rectangle) of an obstacle
bool point_intersects_box(const Vec2& point, const Vec2& box, BoundingCircle* player, BoundingBox* object); 

// Check if a line between two points (representing enemy movement) intersects with the box obstacle
bool line_intersects_obstacle(const Node& start, const Node& end, BoundingCircle* player, BoundingBox* box);

// Get neighboring nodes (step towards the player, adjusted for smooth movement)
std::vector<Node> get_neighbors(const Node& current, const Node& player, float step_size);

// Collision detection for obstacles (check if moving to the new position would intersect with any box)
bool is_valid_move(const Vec2& neighbor, const Vec2& point, BoundingCircle* player, BoundingBox* obstacles, BoundingCircle* object);

// Reconstruct the path from the goal to the start node
std::vector<Node> reconstruct_path(Node* goal);

// A* pathfinding algorithm
std::vector<Node> find_path(Node& start, Node& goal, float step_size, BoundingCircle* player, BoundingBox* box, BoundingCircle* enemy);