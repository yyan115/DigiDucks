#pragma once
#include "vector"
#include "DuckEngine.h"

class EntityDestroyer 
{
private:
	static std::vector<int> entitiesToDestroy;

public:
	static void MarkForDestruction(int entityID) 
	{
		// Add to destruction queue if not already there
		if (std::find(entitiesToDestroy.begin(), entitiesToDestroy.end(), entityID) == entitiesToDestroy.end()) 
		{
			entitiesToDestroy.push_back(entityID);
		}
	}

	static void ProcessDestructions() 
	{
		// Process all destructions
		for (int entityID : entitiesToDestroy) 
		{
			DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(entityID);
		}
		entitiesToDestroy.clear();
	}
};

std::vector<int> EntityDestroyer::entitiesToDestroy;