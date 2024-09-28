#pragma once
#include <unordered_map>
#include "AnimatorComponent.h"
#include "System.h"

class DUCKENGINE_API AnimatorSystem : public System
{
public:

    void Start() override;

    // Function to update animations based on time
    void Update() override;

    // Function to play a specific animation on a component
    void PlayAnimation(AnimatorComponent* component, Animation* animation);

    // Function to stop any animation playing on a component
    void StopAnimation(AnimatorComponent* component);

private:
    // Keeps track of animations currently playing on components
    std::unordered_map<AnimatorComponent*, Animation*> activeAnimations;
};