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