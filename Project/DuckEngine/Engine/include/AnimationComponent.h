#pragma once
#include "Component.h"
#include <string>
#include "Texture.h"
#include <vector>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class Frame {
public:
    int x, y, width, height; // Position and size of the frame in the sprite sheet
    float duration; // Duration for which this frame should be displayed

    Frame(int x, int y, int width, int height, float duration)
        : x(x), y(y), width(width), height(height), duration(duration) {}
};

class Animation {
private:
    std::vector<Frame> Frames;  // List of frames for the animation
    int currentFrame;      // Keeps track of the current frame
    float frameTimer;
    float totalDuration;        // animation cycle

public:
    Animation() : currentFrame(0), frameTimer(0.0f), totalDuration(0.0f) {}
};

class DUCKENGINE_API AnimationComponent : public Component
{
private:
    Animation* currentAnimation;

public:
    AnimationComponent() : currentAnimation(nullptr) {}
    std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<AnimationComponent>(*this);
    }
};

