/******************************************************************************/
/*!
\file		RoamingLogic.CPP
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Nov 6 2024
\brief		Definition of Roaming Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "RoamingLogic.h"

std::vector<Node*> getNeighbors(Node* node, const std::vector<Node*>& allNodes) {
    std::vector<Node*> neighbors;

    // Example of checking the possible 4 directions (expand as needed)
    for (Node* neighbor : allNodes) {
        // Ensure neighbor is not an obstacle and is not the same as the current node
        if (!neighbor->isObstacle && neighbor != node) {
            // Only add neighbors that are adjacent to the current node (same row or column)
            if ((std::abs(node->x - neighbor->x) == 1 && node->y == neighbor->y) || // Horizontal neighbors
                (std::abs(node->y - neighbor->y) == 1 && node->x == neighbor->x)    // Vertical neighbors
                ) {
                neighbors.push_back(neighbor);
            }
        }
    }

    return neighbors;
}

// Heuristic (Manhattan Distance)
static int heuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);  // Manhattan Distance
}

std::vector<Node*> AStar(Node* start, Node* goal, const std::vector<Node*>& allNodes) {
    std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> openSet;
    std::set<Node*> openSetTracker;  // Track nodes that are in the open set
    std::vector<Node*> closedSet;

    start->gCost = 0;
    start->hCost = heuristic(start, goal);
    openSet.push(start);
    openSetTracker.insert(start);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();
        openSetTracker.erase(current);

        // Check if we reached the goal
        if (current == goal) {
            // Path reconstruction
            std::vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());  // Reverse the path to start-to-goal
            return path;
        }

        closedSet.push_back(current);

        // Explore neighbors
        for (Node* neighbor : getNeighbors(current, allNodes)) {
            // Skip if already processed
            if (std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end())
                continue;

            int tentativeGCost = current->gCost + 1;  // Assuming uniform cost between nodes

            bool inOpenSet = openSetTracker.find(neighbor) != openSetTracker.end();

            if (!inOpenSet || tentativeGCost < neighbor->gCost) {
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = heuristic(neighbor, goal);
                neighbor->parent = current;

                if (!inOpenSet) {
                    openSet.push(neighbor);
                    openSetTracker.insert(neighbor);
                }
            }
        }
    }

    return {};  // Return empty path if no path is found
}

// Pathing function to run A* and output the path
void pathing() {
    // Create nodes (locations or waypoints in the world)
    Node node1(0, 0);
    Node node2(1, 0);
    Node node3(2, 0, true);  // Obstacle
    Node node4(3, 0);
    Node node5(4, 0);
    Node node6(5, 0);
    Node node7(6, 0, true);  // Obstacle

    Node node8(0, 1);
    Node node9(1, 1);
    Node node10(2, 1, true);  // Obstacle
    Node node11(3, 1);
    Node node12(4, 1, true);  // Obstacle
    Node node13(5, 1);
    Node node14(6, 1);

    Node node15(0, 2);
    Node node16(1, 2, true);  // Obstacle
    Node node17(2, 2);
    Node node18(3, 2, true);  // Obstacle
    Node node19(4, 2);
    Node node20(5, 2);
    Node node21(6, 2);

    Node node22(0, 3);
    Node node23(1, 3);
    Node node24(2, 3, true);  // Obstacle
    Node node25(3, 3);
    Node node26(4, 3);
    Node node27(5, 3);
    Node node28(6, 3, true);  // Obstacle

    Node node29(0, 4);
    Node node30(1, 4);
    Node node31(2, 4);
    Node node32(3, 4);
    Node node33(4, 4);
    Node node34(5, 4);
    Node node35(6, 4, true);  // Obstacle

    Node node36(0, 5);
    Node node37(1, 5);
    Node node38(2, 5);
    Node node39(3, 5,true);
    Node node40(4, 5);
    Node node41(5, 5);
    Node node42(6, 5);

    Node node43(0, 6);
    Node node44(1, 6,true);
    Node node45(2, 6);
    Node node46(3, 6);
    Node node47(4, 6,true);
    Node node48(5, 6);
    Node node49(6, 6, false);  // Obstacle

    // Set up all the nodes (world)
    std::vector<Node*> allNodes = { &node1, &node2, &node3, &node4, &node5, &node6, &node7,
                                    &node8, &node9, &node10, &node11, &node12, &node13, &node14,
                                    &node15, &node16, &node17, &node18, &node19, &node20, &node21,
                                    &node22, &node23, &node24, &node25, &node26, &node27, &node28,
                                    &node29, &node30, &node31, &node32, &node33, &node34, &node35,
                                    &node36, &node37, &node38, &node39, &node40, &node41, &node42,
                                    &node43, &node44, &node45, &node46, &node47, &node48, &node49 };

    // Start (enemy) and goal (player)
    Node* start = &node1;
    Node* goal = &node49;

    // Run A* to find the path
    std::vector<Node*> path = AStar(start, goal, allNodes);

    // Output the path
    std::cout << "Path from enemy to player:" << std::endl;
    for (Node* node : path) {
        std::cout << "(" << node->x << ", " << node->y << ")" << std::endl;
    }
}

namespace {
    Vec2 getSpeed(Vec2& firstPos, Vec2& secondPos, float speed) {
        Vec2 dir = secondPos - firstPos;
        dir = dir.normalized();
        dir *= speed;
        return dir;
    }
}

// Define the global objectState variable here
std::unordered_map<int, ObjectDatas> objectState;

/**
* @brief
* Function to roam any object with a specific prefab name between two positions
*
* @param prefabName - The name of the prefab to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedPrefab(std::string prefabName, Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == prefabName) {
            RoamTwoPos(entity.entityID, firstPos, secondPos);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object name between two positions
*
* @param objectName - The name of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedObject(std::string objectName, Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.name == objectName) {
            RoamTwoPos(entity.entityID, firstPos, secondPos);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object ID between two positions
*
* @param objectID - The ID of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamTwoPos(int objectID, Vec2& firstPos, Vec2& secondPos) {
    // Retrieve or initialize the Object's specific state
    ObjectDatas& state = objectState[objectID];

    // Get components
    TransformComponent* objTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectID);
    RigidbodyComponent* objRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(objectID);
    BoundingBox* objBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(objectID);
    BoundingCircle* objCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(objectID);
    if (objRb->isStatic) { objRb->isStatic = false; }

	// Get player components
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
	BoundingCircle* playerCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);

    // Check for missing components
    if (!objTrf || !objRb || !playerTrf) {
        std::cerr << "Missing necessary components on ObjectPrefab or Player entity." << std::endl;
        return;
    }
   

    // FSM to handle object movement
    switch (state.state) {
    case Idle:
		// Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.reachedPos) {
            if (state.timer >= state.waitTime) {
                state.reachedPos = false;
                state.timer = 0.f;
                state.state = Moving;
            }
            else {
                state.timer += DuckEngine::DeltaTime();
            }
        }
        else {
            state.state = Moving;
        }
        break;

    case Moving:
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (Vec2Dist(objTrf->position, secondPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->position, secondPos, state.objectSpeed);
        }
        else {
            if (Vec2Dist(objTrf->position, firstPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->position, firstPos, state.objectSpeed);
        }
        break;

    case Chasing:
        float distFromPlayer = Vec2Dist(objTrf->position, playerTrf->position);
        if (distFromPlayer > (state.objectRange * 1.5f)) { state.state = Idle; }

        // Check if object has a circle or box collider
        if (!objBox) {
            float playerRadius_ObjRadius = playerCircle->getRadius() + objCircle->getRadius();
            if (distFromPlayer <= playerRadius_ObjRadius) {
                objRb->isStatic = true;
                break;
            }
        }
        else {
            float playerRadius_ObjSize = playerCircle->getRadius() + objBox->getSize().length();
            if (distFromPlayer <= playerRadius_ObjSize) {
                objRb->isStatic = true;
                break;
            }
        }

        objRb->velocity = getSpeed(objTrf->position, playerTrf->position, state.objectSpeed);

        break;
    }
}

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
void RoamSelectedPrefab(std::string prefabName, Vec2 dir, float time) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == prefabName) {
            RoamDir(entity.entityID, dir, time);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object name in a specific direction
* for a specific time
*
* @param objectName - The name of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamSelectedObject(std::string objectName, Vec2 dir, float time) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.name == objectName) {
            RoamDir(entity.entityID, dir, time);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object ID in a specific direction
* for a specific time
*
* @param objectID - The ID of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamDir(int objectID, Vec2& dir, float time) {
    // Retrieve or initialize the Object's specific state
    ObjectDatas& state = objectState[objectID];

    // Get components
    TransformComponent* objTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectID);
    RigidbodyComponent* objRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(objectID);
	BoundingBox* objBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(objectID);
	BoundingCircle* objCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(objectID);
    if (objRb->isStatic) { objRb->isStatic = false; }

    // Get player components
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
    BoundingCircle* playerCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);

    // Check for missing components
    if (!objTrf || !objRb || !playerTrf) {
        std::cerr << "Missing necessary components on ObjectPrefab or Player entity." << std::endl;
        return;
    }
    
    
    //pathing();

    // FSM to handle object movement
    switch (state.state) {
    case Idle:
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.reachedPos) {
            if (state.timer >= state.waitTime) {
                state.reachedPos = false;
                state.timer = 0.f;
                state.state = Moving;
            }
            else {
                state.timer += DuckEngine::DeltaTime();
            }
        }
        else {
            state.state = Moving;
        }
        break;

    case Moving:
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (state.timer >= time) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
				state.timer = 0.f;
            }
			objRb->velocity = dir.normalized() * state.objectSpeed;
			state.timer += DuckEngine::DeltaTime();
        }
        else {
            if (state.timer >= time) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
                state.timer = 0.f;
            }
            objRb->velocity = -dir.normalized() * state.objectSpeed;
            state.timer += DuckEngine::DeltaTime();
        }
        break;

    case Chasing:
        float distFromPlayer = Vec2Dist(objTrf->position, playerTrf->position);
        if (distFromPlayer > (state.objectRange * 1.5f)) { state.state = Idle; }

        // Check if object has a circle or box collider
        if (!objBox) {
			float playerRadius_ObjRadius = playerCircle->getRadius() + objCircle->getRadius();
            if (distFromPlayer <= playerRadius_ObjRadius) {
                objRb->isStatic = true;
                break;
            }
        }
        else {
            float playerRadius_ObjSize = playerCircle->getRadius() + objBox->getSize().length();
			if (distFromPlayer <= playerRadius_ObjSize) {
				objRb->isStatic = true;
				break;
			}
        }

       
        objRb->velocity = getSpeed(objTrf->position, playerTrf->position, state.objectSpeed);

        
        break;
    }
}

