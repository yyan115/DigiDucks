/******************************************************************************/
/*!
\file       AnimatorSystem.cpp
\author     Jovan chua 2301244
\par        email c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Implements the AnimatorSystem class, updating the animation status when in idle or walking mode per frame.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "AnimatorSystem.h"


void AnimatorSystem::Start()
{

}

/************************************************************************
@brief Updates the animation states for all entities with an AnimatorComponent.
@return nothing
*************************************************************************/
void AnimatorSystem::Update()
{
    for (const auto& [entityId, animatorComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<AnimatorComponent>())
    {
        SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);
        AnimatorComponent* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entityId);

        if (!animator) {
            std::cout << "Animator Component cannot be found on Entity ID: " << entityId << std::endl;
            continue;
        }
        if (!spriteRenderer) {
            std::cout << "Sprite Renderer Component cannot be found on Entity ID: " << entityId << std::endl;
            continue;
        }

        if (!animator->currentAnimation || animator->isPaused) continue;

        animator->currentAnimation->frameTimer += DuckEngine::DeltaTime();

        if (animator->currentAnimation->frameTimer >= animator->currentAnimation->frameDuration)
        {
            animator->currentAnimation->frameTimer = 0.0f;
            animator->currentAnimation->currentFrame = (animator->currentAnimation->currentFrame + 1) % animator->currentAnimation->Frames.size();
        }

        spriteRenderer->texture = *(animator->currentAnimation->Frames[animator->currentAnimation->currentFrame]);
    }
}
