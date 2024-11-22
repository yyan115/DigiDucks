#define NOMINMAX
#include <algorithm>
#include "Raycast.h"
#include "CameraManager.h"
#include "DuckEngine.h"
#include "imgui.h"

// Utility functions to replace std::max and std::min
template <typename T>
T CustomMax(const T& a, const T& b) {
    return (a > b) ? a : b;
}

template <typename T>
T CustomMin(const T& a, const T& b) {
    return (a < b) ? a : b;
}
constexpr float MaxFloatValue = 3.402823e+38f;


namespace Raycast {

    /**************************************************************************
    @brief Calculates a ray in world coordinates from the mouse screen position.
    @return A ray structure containing the origin and direction.
    **************************************************************************/
    Ray CalculateRayFromMouse(const Vector2D& mouseScreenPos)
    {
        ImVec2 fboSize = ImGui::GetContentRegionAvail();

        float normalizedX = (mouseScreenPos.x / fboSize.x) * 2.0f - 1.0f;
        float normalizedY = 1.0f - (mouseScreenPos.y / fboSize.y) * 2.0f;

        Vector2D cameraPos = CameraManager::GetPosition();
        float cameraHeight = CameraManager::GetHeight();
        float aspectRatio = CameraManager::GetAR();
        float cameraWidth = cameraHeight * aspectRatio;

        Ray ray;
        ray.origin = cameraPos;
        ray.direction.x = normalizedX * (cameraWidth / 2.0f) + cameraPos.x;
        ray.direction.y = normalizedY * (cameraHeight / 2.0f) + cameraPos.y;
        ray.direction = (ray.direction - ray.origin).normalized();

        std::cout << "Ray Origin: (" << ray.origin.x << ", " << ray.origin.y
            << "), Direction: (" << ray.direction.x << ", " << ray.direction.y << ")" << std::endl;

        return ray;
    }

    /**************************************************************************
    @brief Checks if a ray intersects an entity's bounding box and returns the distance.
    @param ray The ray to check.
    @param transform The transform component of the entity.
    @return The distance to the intersection or -1 if no intersection.
    **************************************************************************/

    float RayIntersectsEntity(const Ray& ray, const TransformComponent& transform)
    {
        // Define entity bounding box in world coordinates
        float left = transform.position.x - transform.scale.x / 2;
        float right = transform.position.x + transform.scale.x / 2;
        float top = transform.position.y - transform.scale.y / 2;
        float bottom = transform.position.y + transform.scale.y / 2;

        // Ray intersection calculation
        float tMinX = (left - ray.origin.x) / ray.direction.x;
        float tMaxX = (right - ray.origin.x) / ray.direction.x;
        if (tMinX > tMaxX) std::swap(tMinX, tMaxX);

        float tMinY = (top - ray.origin.y) / ray.direction.y;
        float tMaxY = (bottom - ray.origin.y) / ray.direction.y;
        if (tMinY > tMaxY) std::swap(tMinY, tMaxY);

        // Calculate entry and exit distances
        float tEnter = std::max(tMinX, tMinY);
        float tExit = std::min(tMaxX, tMaxY);

        // If the ray doesn't intersect or starts outside the box, return -1
        if (tEnter > tExit || tExit < 0.0f)
            return -1.0f;

        std::cout
            << " Bounding Box: Left=" << left << " Right=" << right
            << " Top=" << top << " Bottom=" << bottom << std::endl;

        return tEnter;
    }



    /**************************************************************************
    @brief Finds the closest entity intersected by the ray.
    @param mouseScreenPos The mouse screen position.
    @return A pointer to the closest entity or nullptr if none is found.
    **************************************************************************/
    Entity* Raycast::GetEntityFromRay(const Vector2D& mouseScreenPos) {
        Ray ray = CalculateRayFromMouse(mouseScreenPos);
        Entity* closestEntity = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        auto& transformComponents = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TransformComponent>();

        for (auto& [entityID, component] : transformComponents) {
            auto* transform = std::static_pointer_cast<TransformComponent>(component).get();

            float distance = RayIntersectsEntity(ray, *transform);
            if (distance > 0 && distance < closestDistance) {
                closestEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
                closestDistance = distance;

                std::cout << "Ray intersects entity: " << entityID
                    << " at distance: " << distance << std::endl;
            }
        }

        if (!closestEntity) {
            std::cout << "No entity intersected by ray." << std::endl;
        }

        return closestEntity;
    }




} // namespace Raycast
