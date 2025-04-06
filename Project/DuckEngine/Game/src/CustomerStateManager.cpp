/******************************************************************************/
/*!
\file       CustomerStateManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       April 6 2025
\brief      Implements the CustomerStateManager class. This system scans the
			entity manager for predefined seat entities (e.g., those named
			"Seat_1", "Seat_2", etc.) and registers their references into a
			location list. Each entry tracks whether the seat is currently
			occupied.

			The commented-out code shows an optional sorting mechanism
			for seat positions based on the numeric suffix in their names.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerStateManager.h"

// stores all the chair locations for each level. for now, just store a bunch of places for customer to stand to wait

void CustomerStateManager::Start() {

	//// Find and get all game objects named "Seat"
	//auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

	//for (auto& entity : entities) {
	//	if (entity->name.find("Seat") != std::string::npos) {
	//		locations.emplace_back(entity.get(), false);
	//		std::cout << "emplaced " << entity->name << "\n";
	//	}
	//}

	//// Ensure sorting works correctly
	//auto sortEntities = [](const std::pair<Entity*, bool>& a, const std::pair<Entity*, bool>& b) {
	//	std::string nameA = a.first->name;
	//	std::string nameB = b.first->name;

	//	// Find the underscore position
	//	size_t posA = nameA.find_last_of('_');
	//	size_t posB = nameB.find_last_of('_');

	//	// Ensure that there is a number after the underscore
	//	if (posA == std::string::npos || posB == std::string::npos) return false;

	//	std::string numStrA = nameA.substr(posA + 1);
	//	std::string numStrB = nameB.substr(posB + 1);

	//	// Validate if the extracted part is actually a number
	//	if (!std::all_of(numStrA.begin(), numStrA.end(), ::isdigit) ||
	//		!std::all_of(numStrB.begin(), numStrB.end(), ::isdigit)) {
	//		std::cerr << "Warning: Invalid entity name format -> " << nameA << " or " << nameB << "\n";
	//		return false; // Keep original order if invalid
	//	}

	//	int numA = std::stoi(numStrA);
	//	int numB = std::stoi(numStrB);
	//	return numA < numB;
	//	};

	//// Sort locations based on numerical suffix
	//std::sort(locations.begin(), locations.end(), sortEntities);
}
