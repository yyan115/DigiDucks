#pragma once

#include <vector>
#include <string>
#include "Entity.h"
#include "EntityManager.h"

class Hierarchy {
public:
    // Render the hierarchy window
    static void ShowHierarchy();

private:
    // Render child entities
    static void ShowChildHierarchy(int parentID);

    static int selectedEntityID; // Currently selected entity ID
};
