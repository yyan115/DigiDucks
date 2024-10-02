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


//class Frame {
//public:
//    int x, y, width, height; // Position and size of the frame in the sprite sheet
//    float duration; // Duration for which this frame should be displayed
//
//    Frame(int x, int y, int width, int height, float duration)
//        : x(x), y(y), width(width), height(height), duration(duration) {}
//};

class Animation 
{
private:

public:
    int currentFrame;        // Keeps track of the current frame
    float frameTimer;        // Timer to track how long the current frame has been displayed
    float frameDuration;     // Duration for each frame in seconds
    std::vector<Texture> Frames;  // List of frames for the animation
    Animation(float durationPerFrame = 0.2f)
        : currentFrame(0), frameTimer(0.0f), frameDuration(durationPerFrame) {}
};

class AnimatorComponent : public Component
{
public:
    Animation* currentAnimation;
    std::unordered_map<std::string, Animation> animations;
    DUCKENGINE_API AnimatorComponent() : currentAnimation(nullptr) {}
    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<AnimatorComponent>(*this);
    }

    // add 1 texture animation
    DUCKENGINE_API void AddAnimation(const std::string& name, const std::shared_ptr<Texture>& animation, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);

        animationToAdd.Frames.push_back(*animation);

        animations[name] = animationToAdd;
    }

    // add multiple textures animation
    DUCKENGINE_API void AddAnimation(std::string animationName, const std::vector<std::shared_ptr<Texture>>& animation, float frameDuration = 0.2f)
    {
        Animation animationToAdd(frameDuration);
        
        for (std::shared_ptr<Texture> texturePtr : animation)
        {
            animationToAdd.Frames.push_back(*texturePtr);
        }

        animations[animationName] = animationToAdd;
    }

    DUCKENGINE_API void PlayAnimation(std::string animationName)
    {
        if (animations.find(animationName) != animations.end())
        {
            currentAnimation = &animations[animationName];
        }
    }

};

