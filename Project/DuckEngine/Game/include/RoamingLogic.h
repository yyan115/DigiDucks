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
