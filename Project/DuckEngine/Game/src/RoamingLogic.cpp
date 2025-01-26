/******************************************************************************/
/*!
\file		RoamingLogic.CPP
\author 	Ernest Ho, h.yonghengernest, 2301223 (65%)
            Jovan Chua,c.shengkaijovan, 2301244 (35%)
\email    	h.yonghengernestt@digipen.edu
\date   	Nov 6 2024
\brief		Definition of Roaming Logic functions, as well as pathfinding functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "RoamingLogic.h"


// Function to check if a point (circle) intersects with the bounding box (rectangle) of an obstacle
/****************************************************************
* @brief Checks if a circular area (player) intersects a bounding box (obstacle).
*
* @param point - The position of the circle's center.
* @param box - The position of the box's center.
* @param player - The player's bounding circle.
* @param object - The obstacle's bounding box.
* @return True if the circle intersects the box, false otherwise.
***************************************************************/
bool point_intersects_box(const Vec2& point, const Vec2& box, BoundingCircle* player, BoundingBox* object) {
    float distFromPlayer = Vec2Dist(box, point);  // Calculate distance from point to box center
    float playerRadius_ObjSizeX = player->getRadius() + object->getSize().x;
    float playerRadius_ObjSizeY = player->getRadius() + object->getSize().y;

    // Check if the distance is less than or equal to the sum of the player's radius and the box size
    if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
        return true;
    }
    return false;
}

// Check if a line between two points (representing enemy movement) intersects with the box obstacle
/****************************************************************
* @brief Checks if the line between two nodes (representing movement) intersects with an obstacle.
*
* @param start - The starting node.
* @param end - The ending node.
* @param player - The player's bounding circle.
* @param box - The bounding box representing the obstacle.
* @return True if the line intersects with the box, false otherwise.
***************************************************************/
bool line_intersects_obstacle(const Node& start, const Node& end, BoundingCircle* player, BoundingBox* box) {
    // Check if either endpoint of the line is colliding with the box
    return point_intersects_box(start.position, box->getCenter(), player, box) ||
        point_intersects_box(end.position, box->getCenter(), player, box);
}

// Check if a move to a neighboring position is valid
/****************************************************************
* @brief Checks if moving to a neighboring position is valid (no collisions).
*
* @param neighbor - The position of the neighboring node.
* @param point - The current position.
* @param player - The player's bounding circle.
* @param obstacles - The bounding box representing obstacles.
* @param object - The enemy's bounding circle.
* @return True if the move is valid, false if it results in a collision.
***************************************************************/
bool is_valid_move(const Vec2& neighbor, const Vec2& point, BoundingCircle* player, BoundingBox* obstacles, BoundingCircle* object) {
    float colliderrad = player->getRadius() + object->getRadius();  // Sum of radii for collision check
    if (Vec2Dist(neighbor, point) < colliderrad) {
        return false;  // The enemy collides with the player
    }

    // Check for collision with obstacles
    if (point_intersects_box(neighbor, obstacles->getCenter(), player, obstacles)) {
        return false;  // Path is blocked by an obstacle
    }

    return true;  // No collisions, valid move
}

// Get neighboring nodes (steps toward the goal)
/****************************************************************
* @brief Generates neighboring nodes based on movement towards the player.
*
* @param current - The current node.
* @param player - The goal node (player's position).
* @param step_size - The size of each step towards the player.
* @return A vector of neighboring nodes.
***************************************************************/
std::vector<Node> get_neighbors(const Node& current, const Node& player, float step_size) {
    std::vector<Node> neighbors;
    Vec2 direction = player.position - current.position;  // Direction vector to the player
    float distance_to_player = Vec2Dist(current.position, player.position);

    if (distance_to_player > step_size) {
        Vec2 step = direction * (step_size / distance_to_player);  // Normalize and scale the direction
        neighbors.push_back({ current.position + step, 0, 0, 0, nullptr });  // Create new neighbor
    }
    else {
        neighbors.push_back({ player.position, 0, 0, 0, nullptr });  // Add player position as a neighbor
    }

    return neighbors;
}

// Reconstruct the path from the goal to the start node
/****************************************************************
* @brief Reconstructs the path from the goal node to the start node.
*
* @param goal - The goal node.
* @return A vector of nodes representing the path.
***************************************************************/
std::vector<Node> reconstruct_path(Node* goal) {
    std::vector<Node> path;
    Node* current = goal;
    while (current != nullptr) {
        path.push_back(*current);
        current = current->parent;  // Move to the parent node
    }
    std::reverse(path.begin(), path.end());  // Reverse the path for correct order
    return path;
}

// Heuristic function for A* pathfinding
/****************************************************************
* @brief Heuristic function for estimating the cost from the current node to the goal.
*
* @param current - The current node.
* @param goal - The goal node.
* @return The estimated cost (Euclidean distance).
***************************************************************/
float heuristic(const Node& current, const Node& goal) {
    return Vec2Dist(current.position, goal.position);
}

// A* pathfinding algorithm
/****************************************************************
* @brief Finds the optimal path from start to goal using the A* algorithm.
*
* @param start - The starting node.
* @param goal - The goal node.
* @param step_size - The step size for generating neighbors.
* @param player - The player's bounding circle.
* @param box - The obstacle's bounding box.
* @param enemy - The enemy's bounding circle.
* @return A vector of nodes representing the optimal path.
***************************************************************/
std::vector<Node> find_path(Node& start, Node& goal, float step_size, BoundingCircle* player, BoundingBox* box, BoundingCircle* enemy) {
    std::priority_queue<Node, std::vector<Node>, CompareNodes> open_list;  // Priority queue for best node selection
    std::unordered_set<Node, NodeHash> closed_list;  // Set for visited nodes

    start.g = 0;  // Initial cost
    start.h = heuristic(start, goal);  // Initial heuristic
    start.f = start.g + start.h;  // Initial total cost
    open_list.push(start);

    while (!open_list.empty()) {
        Node current = open_list.top();  // Node with lowest f-value
        open_list.pop();

        // If goal is reached, reconstruct the path
        if (current == goal) {
            return reconstruct_path(&current);
        }

        closed_list.insert(current);  // Mark current node as visited

        // Check neighboring nodes
        for (auto& neighbor : get_neighbors(current, goal, step_size)) {
            if (is_valid_move(neighbor.position, current.position, player, box, enemy) &&
                closed_list.find(neighbor) == closed_list.end()) {
                neighbor.g = current.g + Vec2Dist(current.position, neighbor.position);  // Calculate cost
                neighbor.h = heuristic(neighbor, goal);
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.parent = &current;  // Set parent for path reconstruction

                open_list.push(neighbor);  // Add to open list
            }
        }
    }

    return {};  // Return empty vector if no path found
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
        if (entity.get()->prefabName == prefabName) {
            RoamTwoPos(entity.get()->entityID, firstPos, secondPos);
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
        if (entity.get()->name == objectName) {
            RoamTwoPos(entity.get()->entityID, firstPos, secondPos);
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
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
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
        if (Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition()) < state.objectRange) { state.state = Chasing; }

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
        if (Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition()) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (Vec2Dist(objTrf->GetPosition(), secondPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->GetPosition(), secondPos, state.objectSpeed);
        }
        else {
            if (Vec2Dist(objTrf->GetPosition(), firstPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->GetPosition(), firstPos, state.objectSpeed);
        }
        break;

    case Chasing:
        float distFromPlayer = Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition());
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
            float playerRadius_ObjSizeX = playerCircle->getRadius() + objBox->getSize().x;
            float playerRadius_ObjSizeY = playerCircle->getRadius() + objBox->getSize().y;
            if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
                objRb->isStatic = true;
                break;
            }
        }

        objRb->velocity = getSpeed(objTrf->GetPosition(), playerTrf->GetPosition(), state.objectSpeed);

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
        if (entity.get()->prefabName == prefabName) {
            RoamDir(entity.get()->entityID, dir, time);
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
        if (entity.get()->name == objectName) {
            RoamDir(entity.get()->entityID, dir, time);
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
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
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
        if (Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition()) < state.objectRange) { state.state = Chasing; }

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
        if (Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition()) < state.objectRange) { state.state = Chasing; }

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
        float distFromPlayer = Vec2Dist(objTrf->GetPosition(), playerTrf->GetPosition());
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
            float playerRadius_ObjSizeX = playerCircle->getRadius() + objBox->getSize().x;
            float playerRadius_ObjSizeY = playerCircle->getRadius() + objBox->getSize().y;
			if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
				objRb->isStatic = true;
				break;
			}
        }

        
        objRb->velocity = getSpeed(objTrf->GetPosition(), playerTrf->GetPosition(), state.objectSpeed);

        
        
        break;
    }
}

