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
private:

public:
    std::string name;
    int currentFrame;        // Keeps track of the current frame
    float frameTimer;        // Timer to track how long the current frame has been displayed
    float frameDuration;     // Duration for each frame in seconds
    std::vector<Texture> Frames;  // List of frames for the animation
    std::string animationFilePath;

    Animation(float durationPerFrame = 0.2f)
        : currentFrame(0), frameTimer(0.0f), frameDuration(durationPerFrame) {}

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

    // add 1 texture animation
/************************************************************************
@brief Adds a single-frame animation to the animator component.
@param name The name of the animation (used as the key in the animations map).
@param animation A `shared_ptr` to the texture that will be used as the single frame of the animation.
@param frameDuration The duration (in seconds) for which the frame will be displayed (default is 0.2f).
@return nothing
*************************************************************************/
    DUCKENGINE_API void AddAnimation(const std::string& name, const std::shared_ptr<Texture>& animation, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);
        animationToAdd.Frames.push_back(*animation);
        animations[name] = animationToAdd;
    }

    DUCKENGINE_API void AddAnimation(const std::string& name, const std::vector<std::shared_ptr<Texture>>& frames, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);

        for (const auto& frame : frames)
        {
            animationToAdd.Frames.push_back(*frame);
        }

        animations[name] = animationToAdd;
    }



    // add multiple textures animation
/************************************************************************
@brief Adds a multi-frame animation to the animator component.
@param animationName The name of the animation (used as the key in the animations map).
@param animation A vector of `shared_ptr` to textures that will be used as the frames of the animation.
@param frameDuration The duration (in seconds) for which each frame will be displayed (default is 0.2f).
@return nothing
*************************************************************************/
    DUCKENGINE_API void AddAnimation(std::string animationName, const std::vector<std::shared_ptr<Texture>>& animation, const std::string filePath, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);

        animationToAdd.animationFilePath = filePath;
        
        for (std::shared_ptr<Texture> texturePtr : animation)
        {
            animationToAdd.Frames.push_back(*texturePtr);
        }

        animations[animationName] = animationToAdd;
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

