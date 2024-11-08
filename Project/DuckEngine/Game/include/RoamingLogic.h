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
    /****************************************************************
    * @brief Hash specialization for Vec2 to enable use in hash-based containers.
    ***************************************************************/
    template <>
    struct hash<Vec2> {
        size_t operator()(const Vec2& p) const {
            // Explicitly cast p.x and p.y to int with comments indicating possible truncation
            return hash<int>()(static_cast<int>(p.x)) ^ hash<int>()(static_cast<int>(p.y));
        }
    };
}

/****************************************************************
* @brief Struct representing a node in pathfinding.
***************************************************************/
struct Node {
    Vec2 position;   // Position of the node in 2D space
    float g;         // Cost from the start node to this node
    float h;         // Heuristic cost from this node to the goal (estimated)
    float f;         // Total cost (f = g + h)
    Node* parent;    // Pointer to the parent node for path reconstruction

    /****************************************************************
    * @brief Equality operator for comparing nodes based on their position.
    *
    * @param other - The other node to compare with.
    * @return True if positions are the same, false otherwise.
    ***************************************************************/
    bool operator==(const Node& other) const {
        return position.x == other.position.x && position.y == other.position.y;
    }

    /****************************************************************
    * @brief Less-than operator for sorting nodes in a priority queue.
    *
    * @param other - The other node to compare with.
    * @return True if this node has a higher f-value (lower priority).
    ***************************************************************/
    bool operator<(const Node& other) const {
        return f > other.f; // Priority queue sorts by greater 'f' value for min-heap behavior
    }
};

/****************************************************************
* @brief Struct for comparing nodes based on their 'f' values.
***************************************************************/
struct CompareNodes {
    bool operator()(const Node& a, const Node& b) {
        return a.f > b.f;  // Comparison for min-heap based on 'f' value
    }
};

/****************************************************************
* @brief Custom hash function for Node to enable use in hash-based containers.
***************************************************************/
struct NodeHash {
    size_t operator()(const Node& node) const {
        // Generate hashes for the x and y coordinates and combine them
        size_t hash1 = std::hash<float>()(node.position.x);
        size_t hash2 = std::hash<float>()(node.position.y);
        return hash1 ^ (hash2 << 1);  // Combine the hashes using bitwise XOR and shift
    }
};

/****************************************************************
* @brief Heuristic function to estimate the distance from the current node to the goal.
*
* @param current - The current node.
* @param goal - The goal node.
* @return The estimated heuristic cost.
***************************************************************/
float heuristic(const Node& current, const Node& goal);

/****************************************************************
* @brief Get neighboring nodes for the current node, adjusted for smooth movement.
*
* @param current - The current node.
* @param player - The goal node (player's position).
* @param step_size - The step size for generating neighbors.
* @return A vector of neighboring nodes.
***************************************************************/
std::vector<Node> get_neighbors(const Node& current, const Node& player, float step_size);

/****************************************************************
* @brief Check if a point (circle) intersects with a bounding box.
*
* @param point - The point to check.
* @param box - The position of the box.
* @param player - The bounding circle of the player.
* @param object - The bounding box of the object.
* @return True if the point intersects with the box, false otherwise.
***************************************************************/
bool point_intersects_box(const Vec2& point, const Vec2& box, BoundingCircle* player, BoundingBox* object);

/****************************************************************
* @brief Check if a line between two points intersects with a bounding box.
*
* @param start - The start node of the line.
* @param end - The end node of the line.
* @param player - The bounding circle of the player.
* @param box - The bounding box of the object.
* @return True if the line intersects with the box, false otherwise.
***************************************************************/
bool line_intersects_obstacle(const Node& start, const Node& end, BoundingCircle* player, BoundingBox* box);

/****************************************************************
* @brief Check if moving to a neighboring position is a valid move.
*
* @param neighbor - The neighbor position to check.
* @param point - The current point position.
* @param player - The bounding circle of the player.
* @param obstacles - The bounding box representing obstacles.
* @param object - The bounding circle of the object.
* @return True if the move is valid, false otherwise.
***************************************************************/
bool is_valid_move(const Vec2& neighbor, const Vec2& point, BoundingCircle* player, BoundingBox* obstacles, BoundingCircle* object);

/****************************************************************
* @brief Reconstruct the path from the goal node to the start node.
*
* @param goal - The goal node.
* @return A vector of nodes representing the path.
***************************************************************/
std::vector<Node> reconstruct_path(Node* goal);

/****************************************************************
* @brief A* pathfinding algorithm to find the optimal path from start to goal.
*
* @param start - The starting node.
* @param goal - The goal node.
* @param step_size - The step size for generating neighbors.
* @param player - The bounding circle of the player.
* @param box - The bounding box of the object.
* @param enemy - The bounding circle of the enemy.
* @return A vector of nodes representing the optimal path.
***************************************************************/
std::vector<Node> find_path(Node& start, Node& goal, float step_size, BoundingCircle* player, BoundingBox* box, BoundingCircle* enemy);

/*****************************************************************************************/