#ifndef RAYCAST_H
#define RAYCAST_H

#include "Vector2.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <limits>

// Struct representing a ray
struct Ray {
    Vector2D origin;
    Vector2D direction;
};

// Namespace for raycasting utilities
namespace Raycast {
    // Calculates a ray in world coordinates from the mouse screen position
    Ray CalculateRayFromMouse(const Vector2D& mouseScreenPos);

    // Checks if a ray intersects an entity's bounding box
    float RayIntersectsEntity(const Ray& ray, const TransformComponent& transform);

    // Finds the closest entity intersected by the ray
    Entity* GetEntityFromRay(const Vector2D& mouseScreenPos);
}

#endif // RAYCAST_H
