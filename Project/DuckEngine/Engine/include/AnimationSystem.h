#pragma once
#include "AnimationComponent.h"
#include <unordered_map>

class DUCKENGINE_API AnimationSystem {
public:
    // Function to update animations based on time
    void Update(float deltaTime);

    // Function to play a specific animation on a component
    void PlayAnimation(AnimationComponent* component, Animation* animation);

    // Function to stop any animation playing on a component
    void StopAnimation(AnimationComponent* component);

private:
    // Keeps track of animations currently playing on components
    std::unordered_map<AnimationComponent*, Animation*> activeAnimations;
};