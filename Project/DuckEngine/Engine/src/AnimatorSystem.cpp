#include "AnimatorSystem.h"


void AnimatorSystem::Start()
{

}

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

        if (!animator->currentAnimation) continue;

        animator->currentAnimation->frameTimer += DuckEngine::DeltaTime();

        if (animator->currentAnimation->frameTimer >= animator->currentAnimation->frameDuration)
        {
            animator->currentAnimation->frameTimer = 0.0f;
            animator->currentAnimation->currentFrame = (animator->currentAnimation->currentFrame + 1) % animator->currentAnimation->Frames.size();
        }

        spriteRenderer->texture = animator->currentAnimation->Frames[animator->currentAnimation->currentFrame];
    }
}
