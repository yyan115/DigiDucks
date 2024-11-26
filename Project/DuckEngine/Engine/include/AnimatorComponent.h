/******************************************************************************/
/*!
\file       AnimatorComponent.h
\author     Jovan chua 2301244
\par        email c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Holds the Animation class and implements the animation in different texture modes while executing it.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include "unordered_map"
#include "Component.h"
#include "Texture.h"


// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief Represents an animation consisting of multiple frames and their duration.
*************************************************************************/
class Animation
{
public:
    std::string name;                     // Name of the animation
    int currentFrame;                     // Current frame index
    float frameTimer;                     // Timer for frame duration
    float frameDuration;                  // Duration for each frame
    std::vector<std::shared_ptr<Texture>> Frames;  // Loaded textures for each frame
    std::vector<std::string> texturePaths;         // File paths for the textures

    Animation(float durationPerFrame = 0.2f)
        : currentFrame(0), frameTimer(0.0f), frameDuration(durationPerFrame) {
    }

    void Reset()
    {
        currentFrame = 0;
        frameTimer = 0.0f;
    }
};


/************************************************************************
@brief Manages animations for an entity and provides functions to play and add animations.
*************************************************************************/
class AnimatorComponent : public Component
{
public:
    Animation* currentAnimation;
    bool isPaused;
    std::unordered_map<std::string, Animation> animations;

    DUCKENGINE_API AnimatorComponent() : currentAnimation(nullptr), isPaused(true) {}

    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<AnimatorComponent>(*this);
    }

    DUCKENGINE_API void AddAnimation(const std::string& name, const std::vector<std::string>& texturePaths, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);
        animationToAdd.name = name;
        animationToAdd.texturePaths = texturePaths;
        animations[name] = animationToAdd;
    }

    //play animation
/************************************************************************
@brief Plays the animation specified by its name.
@param animationName The name of the animation to play.
@return nothing
*************************************************************************/
    DUCKENGINE_API void PlayAnimation(std::string animationName)
    {
        isPaused = false;
        if (animations.find(animationName) != animations.end())
        {
            currentAnimation = &animations[animationName];
        }
    }

    DUCKENGINE_API void Pause() 
    {
        isPaused = true;
    }

    DUCKENGINE_API void Resume() 
    {
        isPaused = false;
    }

    // Check if the animation is currently playing
    bool IsPlaying() const
    {
        return currentAnimation != nullptr && !isPaused;
    }

    // Set a new animation by name
    DUCKENGINE_API void SetAnimation(const std::string& animationName) 
    {
        if (animations.find(animationName) != animations.end()) 
        {
            currentAnimation = &animations[animationName];
        }
    }

    DUCKENGINE_API const std::unordered_map<std::string, Animation>& GetAnimations() const
    {
        return animations;
    }


};

