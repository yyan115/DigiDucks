/******************************************************************************/
/*!
\file       SpriteMovementScene.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the SpriteMovementScene class, which handles the logic
			for a scene focused on sprite movement. This includes loading
			assets, setting up entities, and managing input to control player
			movement and animations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "DuckEngine.h"
#include "DuckEngine_Sound.h"
#include "DuckEngine_Input.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"
#include "ResourcePath.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <set>

Entity* player;
Entity* camera;

TransformComponent* playerTransform;
RigidbodyComponent* playerRb;
AnimatorComponent* playerAnimator;
SoundComponent* playerSound;

BoundingCircle* circle;

// Test text
Entity* textBox;
TextComponent* textComponent;

// Test button
Entity* buttonBox;
//ButtonComponent buttonComponent;

// test UI
Entity* testUI;

// no texture sprite
Entity* noTextureEntity;

// Test Roaming Logic
Vec2 positon1 = { 10.f, 10.f };
Vec2 position2 = { -10.f, 10.f };

//Test Messaging System
InputEventManager inputEventManager;
Player message;

struct MyStruct {
	int x;
	int y;
};

bool operator<(const MyStruct& lhs, const MyStruct& rhs) {
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}


//// Define a Node structure representing each point on the path grid
//struct Node {
//	Vec2 position;           // Position of the node in 2D space
//	Node* parent = nullptr;  // Parent node to allow backtracking when constructing the path
//	float actualCost = 0.0;     // Actual cost from the start node to this node (often called "g")
//	float totalCost = 0.0;      // Total cost (actualCost + heuristic) to target (often called "f")
//};
//
//// Comparator for sorting nodes by totalCost in the priority queue
//struct CompareCost {
//	bool operator()(Node* a, Node* b) {
//		return a->totalCost > b->totalCost; // Nodes with lower totalCost are given priority
//	}
//};
//
//struct Vec2Comparator {
//	bool operator()(const Vec2& lhs, const Vec2& rhs) const {
//		return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
//	}
//};
//
//// Calculate the heuristic (Manhattan distance) between two positions
//float Heuristic(const Vec2& pos1, const Vec2& pos2) {
//	return static_cast<float>(std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y));
//}
//
//// Generate the neighbors (up, down, left, right) of a given position
//std::vector<Vec2> GetNeighbors(const Vec2& pos) {
//	return {
//		{pos.x + 1, pos.y},   // Right neighbor
//		{pos.x - 1, pos.y},   // Left neighbor
//		{pos.x, pos.y + 1},   // Up neighbor
//		{pos.x, pos.y - 1}    // Down neighbor
//	};
//}
//
//bool ArePositionsEqual(const Vec2& pos1, const Vec2& pos2) {
//	return pos1.x == pos2.x && pos1.y == pos2.y;
//}
//
//// Main A* pathfinding function to find the shortest path from start to target
//std::vector<Vec2> static AStarPathfinding(const Vec2& start, const Vec2& target) {
//	std::priority_queue<Node*, std::vector<Node*>, CompareCost> openList; // Priority queue for nodes to explore
//	//std::unordered_set<Vec2> closedList; // Set to track explored nodes
//	//std::set<Vec2> closedList;
//	//std::map<Vec2, Node*> allNodes; // Map of all generated nodes to avoid duplicate nodes
//
//	std::set<Vec2, Vec2Comparator> closedList;
//	std::map<Vec2, Node*, Vec2Comparator> allNodes;
//
//	// Initialize the starting node and add it to the open list
//	Node* startNode = new Node{ start, nullptr, 0, Heuristic(start, target) };
//	openList.push(startNode); // Push the start node into the priority queue
//	allNodes[start] = startNode; // Track the start node in allNodes
//
//	// Continue searching while there are nodes in the open list
//	while (!openList.empty()) {
//		Node* currentNode = openList.top(); // Get the node with the lowest totalCost
//		openList.pop(); // Remove it from the open list
//
//		// If the current node is the target, construct and return the path
//		if (ArePositionsEqual(currentNode->position, target)) {
//			std::vector<Vec2> path;
//			Node* pathNode = currentNode;
//
//			// Backtrack from target to start to build the path
//			while (pathNode) {
//				path.push_back(pathNode->position); // Add each position to the path
//				pathNode = pathNode->parent; // Move to the parent node
//			}
//
//			std::reverse(path.begin(), path.end()); // Reverse the path to get start-to-target order
//			return path; // Return the completed path
//		}
//
//		// Add the current node's position to the closed list to avoid re-exploration
//		closedList.insert(currentNode->position);
//
//		// Loop through each neighboring position of the current node
//		for (auto& neighborPos : GetNeighbors(currentNode->position)) {
//			// Skip this neighbor if it's already in the closed list
//			if (closedList.find(neighborPos) != closedList.end()) continue;
//
//			// Calculate the actual cost to reach the neighbor and the total cost (actual + heuristic)
//			float actualCost = currentNode->actualCost + 1; // Assuming uniform movement cost
//			float totalCost = actualCost + Heuristic(neighborPos, target);
//
//			// If the neighbor hasn't been created, or the new path is cheaper, add/update it
//			if (allNodes.find(neighborPos) == allNodes.end() || allNodes[neighborPos]->actualCost > actualCost) {
//				// Create a new node for this neighbor
//				Node* neighborNode = new Node{ neighborPos, currentNode, actualCost, totalCost };
//				openList.push(neighborNode); // Add it to the open list to explore later
//				allNodes[neighborPos] = neighborNode; // Track the neighbor in allNodes
//			}
//		}
//	}
//
//	return {}; // Return an empty path if no path was found to the target
//}

/************************************************************************
@brief Loads the necessary assets and entities for the SpriteMovementScene.
	   Initializes the player, obstacles, UI elements, and their components.
*************************************************************************/
void SpriteMovementScene::Load()
{
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(20);

	// Load necessary assets (textures, sounds, etc.)
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_OLDMAN.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_BACKGROUND.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CRATE.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CHARACTERIDLE.c_str(), 19, 24);
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CHARACTERWALK.c_str(), 19, 24);

	PrefabManager::LoadPrefabsFromFile("../Resources/Prefab.json");
	LevelManager::LoadLevelGame("../Resources/Scenes/SpriteMovementScene.json");

	// instantiate prefabs
	player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
	playerRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(player->entityID);
	playerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(player->entityID);
	circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);
	circle->SetCollisionCallback([](Entity* otherEntity)
		{
			UNREFERENCED_PARAMETER(otherEntity);
			std::cout << "Player collided with another entity!" << std::endl;

		});
	playerSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(player->entityID);

	//background entity
	//Entity* background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Background");

	// test UI
	//testUI = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", {400.0f, 400.0f}, {100.0f, 100.0f});
	//auto testUITransfrom = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(testUI->entityID);
	//testUITransfrom->relativeToCamera = false;

	//auto testUISprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(testUI->entityID);
	//testUISprite->layer = 2;

	//textBox = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", { 400.0f, 400.0f }, { 100.0f, 100.0f });
	//auto textComponent = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Obstacle1");
	//textComponent->text = "TEST TEXT COMPONENT";
	//textComponent->position = { 0.f, 0.f };
	//textComponent->fontSize = 1;
	//textComponent->color = { 255.f, 50.f, 100.f, 250.f };

	//buttonBox = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", { 400.0f, 400.0f }, { 100.0f, 100.0f });
	//auto buttonComponent = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<ButtonComponent>(buttonBox->entityID);
	//buttonComponent->minPos = {0.f, 0.f};
	//buttonComponent->maxPos = { 200.f, 100.f };
	//buttonComponent->onClick = []() { std::cout << "Button clicked!"; };

	//auto buttonSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(buttonBox->entityID);
	//buttonSprite->color = { 0.0f, 255.0f, 255.0f, 255.0f };
	//buttonSprite->useColor = true;
	//buttonSprite->sprite = true;

	Entity* buttonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("buttonBox");
	auto buttonTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(buttonEntity->entityID);
	auto buttonComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(buttonEntity->entityID);
	// Calculate width and height from minPos and maxPos
	float buttonWidth = buttonComponent->maxPos.x - buttonComponent->minPos.x;
	float buttonHeight = buttonComponent->maxPos.y - buttonComponent->minPos.y;

	buttonComponent->onClick = []() { std::cout << "Button clicked!\n"; };
	// Convert minPos and maxPos from top-left to OpenGL centered coordinates
	Vector2D centeredMinPos = {
		buttonComponent->minPos.x - DuckEngine::GetWindowWidth() / 2.0f,
		DuckEngine::GetWindowHeight() / 2.0f - buttonComponent->minPos.y
	};

	Vector2D centeredMaxPos = {
		buttonComponent->maxPos.x - DuckEngine::GetWindowWidth() / 2.0f,
		DuckEngine::GetWindowHeight() / 2.0f - buttonComponent->maxPos.y
	};

	// Set Transform scale
	buttonTransform->scale = { buttonWidth, buttonHeight };

	// Set Transform position as the center of the converted min and max positions
	buttonTransform->position = (centeredMinPos + centeredMaxPos) / 2.0f;

	buttonTransform->relativeToCamera = false;

	//buttonTransform->position = buttonCenter;

	//// Given minPos and maxPos as the bounding coordinates
	//Vector2D minPos = buttonComponent->minPos;
	//Vector2D maxPos = buttonComponent->maxPos;

	//// Calculate the width (scale in x) and height (scale in y)
	//float width = maxPos.x - minPos.x;  // Scale along the x-axis
	//float height = maxPos.y - minPos.y;  // Scale along the y-axis

	//// Set the scale based on the calculated width and height
	//buttonTransform->scale = { width, height };


	//float buttonWidth = buttonComponent->maxPos.x - buttonComponent->minPos.x;
	//float buttonHeight = buttonComponent->maxPos.y - buttonComponent->minPos.y;
	//buttonTransform->scale = { buttonWidth, buttonHeight };
	//buttonTransform->position = {
	//(buttonComponent->minPos.x + buttonComponent->maxPos.x) / 2,
	//(buttonComponent->minPos.y + buttonComponent->maxPos.y) / 2
	//};
	//buttonTransform->relativeToCamera = false;

	inputEventManager.addListener(&message);
}

/************************************************************************
@brief Starts the SpriteMovementScene, initializing sounds or other
	   elements before the main update loop.
*************************************************************************/
void SpriteMovementScene::Start()
{
	//DuckEngine_Sound::PlaySounds("TestSound");
	DuckEngine::showDebugDraw = false;
}

/************************************************************************
@brief Updates the logic of the SpriteMovementScene, including player input,
	   sprite animations, and rendering of objects.
*************************************************************************/
void SpriteMovementScene::Update()
{
	float moveSpeed = 10.0f;

	// Reset the player's velocity at the start of each fixed update
	playerRb->velocity = Vec2(0.f, 0.f);

	// Store input state - don't directly modify velocity
	Vector2D inputDirection(0.0f, 0.0f);

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		inputDirection.y += 1.0f;
		playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		inputDirection.y -= 1.0f;
		playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		inputDirection.x -= 1.0f;
		playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		inputDirection.x += 1.0f;
		playerAnimator->PlayAnimation("WalkAnimation");
	}

	// Normalize the input direction if it's not zero
	if (inputDirection.x != 0.0f || inputDirection.y != 0.0f)
	{
		float length = std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
		inputDirection.x /= length;
		inputDirection.y /= length;
	}

	// Set velocity based on normalized input
	playerRb->velocity = inputDirection * moveSpeed;
	
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}
	
	
	/*if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_DOWN)
	{
		std::cout << "Mouse scrolled down!\n";
	}

	if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_UP)
	{
		std::cout << "Mouse scrolled up!\n";
	}*/
	
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (playerSound) {
			std::cout << "Sound stopped\n";
			playerSound->Stop();
		}
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
		
	}

	inputEventManager.notifyScrollEvent(static_cast<int>(DuckEngine_Input::GetScrollOffsetY()));
	
	if (playerRb->velocity.x == 0.0f && playerRb->velocity.y == 0.0f)
	{
		playerAnimator->PlayAnimation("IdleAnimation");
	}

	
	//// Get the current positions of the player and the obstacle
	//Vec2 playerPos = playerTransform->position;
	//Vec2 obstaclePos = obstacleTransform->position;

	//// Call A* to find the shortest path from the obstacle to the player
	//std::vector<Vec2> path = AStarPathfinding(obstaclePos, playerPos);

	//// Check if a valid path was found and that it contains at least two positions
	//if (!path.empty() && path.size() > 1) {
	//	Vec2 nextPosition = path[1]; // The next position for the obstacle to move toward

	//	// Calculate the direction vector and normalize it
	//	Vec2 direction = { nextPosition.x - obstaclePos.x, nextPosition.y - obstaclePos.y };
	//	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y); // Distance to next position
	//	if (length != 0) { // Prevent division by zero
	//		direction.x /= length; // Normalize x-component
	//		direction.y /= length; // Normalize y-component
	//	}

	//	// Set the obstacle's velocity towards the next position along the path
	//	obstacleRb->velocity = { direction.x * moveSpeed, direction.y * moveSpeed };
	//}
	//else {
	//	// If no valid path, stop the obstacle by setting its velocity to zero
	//	obstacleRb->velocity = { 0.0f, 0.0f };
	//}

	//DuckEngine::DrawCircle(circle->getCenter(), circle->getRadius());

	//DuckEngine::DrawRectangle(box->getBtmL(), box->getTopR(), box->rotation);

	//DuckEngine::DrawCircle(box2->getCenter(), box2->getRadius());

	//DuckEngine::DrawLine({ 7.f, 5.f }, {-10.f, 10.f}, 0.05f);

	//DuckEngine::DrawPoint({ -3.f, -3.f }, 10.f);

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(playerTransform->position.x, playerTransform->position.y);

	//DuckEngine::RenderText("TEST SCENE", { 20.f , DuckEngine::GetWindowHeight() - 200.f }, 1.f, { 255.f, 50.f, 100.f, 250.f });

	//DuckEngine::RenderText("TEST TEXT", { DuckEngine::GetWindowWidth() - 300.f  , 250.f }, 1.f, { 0.f, 255.f, 150.f, 250.0f });

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	// Test Roaming Logic
	// Between 2 Pos
	RoamSelectedPrefab("DuckPrefab", positon1, position2);
	// A direction for a time.
	//RoamSelectedPrefab("DuckPrefab", positon1, 3.f);
	
}

/************************************************************************
@brief Handles post-update logic after the main update loop.
*************************************************************************/
void SpriteMovementScene::PostUpdate()
{

}

/************************************************************************
@brief Exits the SpriteMovementScene, handling any necessary cleanup.
*************************************************************************/
void SpriteMovementScene::Exit()
{
}

/************************************************************************
@brief Unloads the SpriteMovementScene, freeing any resources used.
*************************************************************************/
void SpriteMovementScene::Unload()
{
	// base unload
	Scene::Unload();

}

void SpriteMovementScene::SetupLayers()
{

}