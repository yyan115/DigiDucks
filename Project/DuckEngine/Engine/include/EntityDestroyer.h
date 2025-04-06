/******************************************************************************/
/*!
\file       EntityDestroyer.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Declares the EntityDestroyer class, which provides a static
			utility for marking and safely destroying entities in the
			DuckEngine system. Entities marked for destruction are
			deferred and processed in batches, preventing issues with
			real-time entity removal during update loops.

			This is useful for ensuring stability and avoiding invalid
			accesses during gameplay or logic execution.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


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