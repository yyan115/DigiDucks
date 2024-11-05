#pragma once
#include "DuckEngine.h"
#include "ResourcePath.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Event Listener Interface
class InputEventListener {
public:
    virtual void onMouseButtonPressed(const std::string& button) = 0;
    virtual void onScrollEvent(int scrollOffset) = 0;
    virtual void onKeyPressed(char key) = 0;
    virtual void onAnimationPlay(const std::string& animation) = 0;
    virtual ~InputEventListener() = default;
};

// Input Event Manager
class InputEventManager {
public:
    void addListener(InputEventListener* listener) {
        listeners.push_back(listener);
    }

    void removeListener(InputEventListener* listener) {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    }

    void notifyMouseButtonPressed(const std::string& button) {
        for (auto* listener : listeners) {
            listener->onMouseButtonPressed(button);
        }
    }

    void notifyScrollEvent(int scrollOffset) {
        for (auto* listener : listeners) {
            listener->onScrollEvent(scrollOffset);
        }
    }

    void notifyKeyPressed(char key) {
        for (auto* listener : listeners) {
            listener->onKeyPressed(key);
        }
    }

    void notifyAnimationPlay(const std::string& animation) {
        for (auto* listener : listeners) {
            listener->onAnimationPlay(animation);
        }
    }

private:
    std::vector<InputEventListener*> listeners;
};

// Example Subscriber: Player
class Player : public InputEventListener {
public:
    void onMouseButtonPressed(const std::string& button) override {
        if (button == "Left") {
            std::cout << "Left mouse button pressed!\n";
        }
    }

    void onScrollEvent(int scrollOffset) override {
        if (scrollOffset < 0) {
            std::cout << "Mouse scrolled down!\n";
        }
        else if (scrollOffset > 0) {
            std::cout << "Mouse scrolled up!\n";
        }
    }

    void onKeyPressed(char key) override {
        if (key == 'U') {
            std::cout << "Sound stopped\n";
            // Assume playerSound is an available object with a Stop method
            //playerSound->Stop();
        }

        if (key == 'K') {
            std::cout << "K is pressed!\n";
            // Assume showDebugDraw is a static member to toggle debug display
            DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
        }
    }

    void onAnimationPlay(const std::string& animation) override {
        // Handle animation play
        std::cout << "Playing animation: " << animation << "\n";
    }

    void update(float velocityX, float velocityY) {
        if (velocityX == 0.0f && velocityY == 0.0f) {
            onAnimationPlay("IdleAnimation");
        }
    }
};

