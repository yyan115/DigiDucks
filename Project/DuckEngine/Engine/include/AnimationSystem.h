#pragma once
#include <unordered_map>
#include "AnimationComponent.h"
#include "System.h"

class DUCKENGINE_API AnimationSystem : public System
{
public:

    void Start() override;

    // Function to update animations based on time
    void Update() override;

    // Function to play a specific animation on a component
    void PlayAnimation(AnimationComponent* component, Animation* animation);

    // Function to stop any animation playing on a component
    void StopAnimation(AnimationComponent* component);

private:
    // Keeps track of animations currently playing on components
    std::unordered_map<AnimationComponent*, Animation*> activeAnimations;
};