#include "GraphicsManager.h"
#include "ButtonSystem.h"
#include "ButtonComponent.h"
#include "DuckEngine_Input.h"
#include "imgui.h"
#include "CameraManager.h"

// Utility function to check if a point is within button bounds
bool IsPointInside(const Vector2D& point, const Vector2D& position, const Vector2D& scale)
{
    Vector2D min = position - scale * 0.5f; // Calculate the minimum boundary
    Vector2D max = position + scale * 0.5f; // Calculate the maximum boundary

    //std::cout << "Not inside. Mouse pos: " << point.x << ", " << point.y << ". min:" << min.x << ", " << min.x << ". max: " << max.x << ", " << max.y << ".\n";

    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

void ButtonSystem::Start()
{
}

void ButtonSystem::Update()
{
}

// Temporary render function until FixedUpdate and Update are implemented
void ButtonSystem::Render()
{
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
    {
        ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
        //TransformComponent* sprite = static_cast<TransformComponent*>(component.get());
        TransformComponent* sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (!sprite) continue;

        // Retrieve position and scale from TransformComponent
        Vector2D position = sprite->GetPosition();
        Vector2D scale = sprite->scale;

        // Skip if disabled
        if (!button->isEnabled)
        {
            continue;
        }

        // Editor mode handling
        if (DuckEngine::isEditor)
        {
            if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
            {
                Vector2D mousePosWorld = DuckEngine::editorMouseWorldPos;
                if (IsPointInside(mousePosWorld, position, scale))
                {
                    if (button->onClick)
                    {
                        button->onClick();
                    }
                }
            }
        }
        else // Game mode handling
        {
            if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
            {
                Vector2D mousePosScreen = { static_cast<float>(DuckEngine_Input::GetMouseX()), static_cast<float>(DuckEngine_Input::GetMouseY()) };

                Vector2D mousePosWorld = GraphicsManager::ScreenToWorld(mousePosScreen);

                //std::cout << "pos: " << position.x << ", " << position.y << ". scale: " << scale.x << ", " << scale.y << "\n";

                if (IsPointInside(mousePosWorld, position, scale))
                {
                    //std::cout << "Button clicked. Mouse Pos: " << DuckEngine_Input::GetMouseX() << ", " << DuckEngine_Input::GetMouseY() << ".\n";
                    if (button->onClick)
                    {
                        button->onClick();
                    }
                }
                else {

                }
            }
        }

        // On-hover functionality could be added here in the future
    }
}
