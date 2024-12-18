/******************************************************************************
\file       SpriteRendererSystem.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the SpriteRendererSystem class, which manages the
			rendering of sprites in the game engine. This system organizes
			sprites into layers and adds them to the graphics draw queue.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include "SpriteRendererSystem.h"
#include "GraphicsManager.h"
#include "Color.h"
#include "algorithm"
#include "DuckEngine.h"

namespace {
	void DrawDebug(TransformComponent* transform, SpriteRendererComponent* spriteRenderer) {
		// Calculate the rectangle bounds
		Vector2D halfSize = transform->scale * 0.5f; // Using scale to determine half dimensions
		Vector2D min = transform->GetPosition() - halfSize; // Bottom-left corner
		Vector2D max = transform->GetPosition() + halfSize; // Top-right corner

		// Optionally apply rotation to corners if needed
		// This step can vary based on how you want to handle rotation
		// For a simple case, you can just draw a rectangle without rotation
		DuckEngine::DrawRectangle(min, max, transform->angle, spriteRenderer->color); // Adjust the color as needed
	}
}

struct RenderData
{
	TransformComponent* transform;
	SpriteRendererComponent* spriteRenderer;
	int layer;
	int entityID;
};

void SpriteRendererSystem::Start()
{
	// Initialization code if needed
}

void SpriteRendererSystem::Update()
{

}


/************************************************************************
@brief Updates the SpriteRendererSystem by gathering all entities with
	   SpriteRendererComponent and TransformComponent, sorting them by layer,
	   and adding them to the graphics draw queue for rendering.
*************************************************************************/

void SpriteRendererSystem::Render()
{
    auto* activeScene = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene();
    if (!activeScene) return;

    float totalTime = static_cast<float>(DuckEngine::accumulatedTime);
    float alpha = static_cast<float>((totalTime / DuckEngine::FIXED_TIMESTEP) - std::floor(totalTime / DuckEngine::FIXED_TIMESTEP));
    alpha = std::min(1.0f, std::max(0.0f, alpha));

    auto& allEntities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    for (Entity& entity : allEntities)
    {
        auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entity.entityID);
        if (!spriteRenderer) continue;

        bool isInCorrectLayer = false;
        for (const auto& [layerName, layer] : activeScene->GetLayers())
        {
            if (layer.HasEntityByID(entity.entityID))
            {
                isInCorrectLayer = (layerName == entity.layerName);
                break;
            }
        }

        if (!isInCorrectLayer)
        {
            for (const auto& [layerName, layer] : activeScene->GetLayers())
            {
                if (layer.HasEntityByID(entity.entityID))
                {
                    activeScene->RemoveEntityFromLayer(layerName, entity.entityID);
                    break;
                }
            }

            activeScene->AddEntityToLayer(entity.layerName, &entity);
        }
    }

    std::vector<RenderData> renderQueue;

    float viewportWidth = static_cast<float>(DuckEngine::GetViewportWidth());
    float viewportHeight = static_cast<float>(DuckEngine::GetViewportHeight());

    for (const auto& [layerName, layer] : activeScene->GetLayers())
    {
        int layerOrder = layer.GetOrder();
        bool isUILayer = false;
        isUILayer = (layerName == "UI");

        for (int entityID : layer.GetEntityIDs())
        {
            auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID);
            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID);

            if (!spriteRenderer || !transform || !spriteRenderer->isVisible)
            {
                continue;
            }
 
            if (!DuckEngine::IsPlaying())
            {
                transform->previousPosition = transform->GetPosition();
            }

            RenderData data;
            data.transform = transform;
            data.spriteRenderer = spriteRenderer;
            data.layer = layerOrder;
            data.entityID = entityID;

            renderQueue.push_back(data);
        }
    }

    std::sort(renderQueue.begin(), renderQueue.end(),
        [](const RenderData& a, const RenderData& b) {
            if (a.layer != b.layer)
                return a.layer < b.layer;
            return a.spriteRenderer->sortingOrder < b.spriteRenderer->sortingOrder;
        });

    for (const RenderData& data : renderQueue)
    {
        DrawOptions drawOptions;
        bool isUILayer = !data.transform->relativeToCamera;

        if (isUILayer)
        {
            Vector2D uiPos = data.transform->GetPosition();
            drawOptions.translation = Vector2D(
                uiPos.x * viewportWidth,
                uiPos.y * viewportHeight
            );
        }
        else if (data.transform->relativeToCamera)
        {
            if (data.transform->previousPosition == data.transform->GetPosition())
            {
                drawOptions.translation = data.transform->GetPosition();
            }
            else
            {
                Vector2D interpolatedPosition = data.transform->previousPosition +
                    (data.transform->GetPosition() - data.transform->previousPosition) * alpha;
                drawOptions.translation = interpolatedPosition;
            }
        }
        else
        {
            drawOptions.translation = data.transform->GetPosition();
        }

        if (isUILayer)
        {
            drawOptions.scale = Vector2D(
                data.transform->scale.x * viewportWidth,
                data.transform->scale.y * viewportHeight
            );
        }
        else
        {
            drawOptions.scale = data.transform->scale;
        }

        drawOptions.rotation = data.transform->angle;

        if (data.spriteRenderer->texture)
        {
            drawOptions.useTexture = true;
            drawOptions.texture = &data.spriteRenderer->texture;
        }
        else if (data.spriteRenderer->useColor)
        {
            drawOptions.useColor = true;
            drawOptions.color = data.spriteRenderer->color;
        }
        else
        {
            drawOptions.useColor = true;
            drawOptions.color = { 255.f, 0.f, 255.f, 255.f };
        }

        drawOptions.relativeToCamera = data.transform->relativeToCamera;

        GraphicsManager::AddToDrawQueue(drawOptions);
    }
}