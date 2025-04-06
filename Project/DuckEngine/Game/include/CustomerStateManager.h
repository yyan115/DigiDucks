/******************************************************************************/
/*!
\file       CustomerStateManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       April 6 2025
\brief      Declares the CustomerStateManager class, which manages a list of
            positions (typically seats or waiting spots) for customers within
            a level. Each position is stored alongside an occupancy state
            (true/false) to indicate whether it is currently taken.

            Intended for use in customer queueing, seating, or AI placement
            systems during gameplay.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <vector>
#include <utility>
#include "DuckEngine.h"

// stores all the chair locations for each level. for now, just store a bunch of places for customer to stand to wait

class CustomerStateManager {
public:
	std::vector<std::pair<Entity*, bool>> locations{};

	void Start();
};