/******************************************************************************/
/*
\file:      MessagingSystem.h
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:	    yankai.tan@digipen.edu


\brief:     Contains the definitions that are used for Subscriber-Listener model
 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/
#pragma once

#include "DuckEngine.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/****************************************************************
* @brief Interface for handling input events in the system.
***************************************************************/
class InputEventListener {
public:
    /****************************************************************
    * @brief Event handler for mouse button press.
    *
    * @param button - The button that was pressed.
    ***************************************************************/
    virtual void onMouseButtonPressed(const std::string& button) = 0;

    /****************************************************************
    * @brief Event handler for mouse scroll events.
    *
    * @param scrollOffset - The scroll offset value (positive or negative).
    ***************************************************************/
    virtual void onScrollEvent(int scrollOffset) = 0;

    /****************************************************************
    * @brief Event handler for key press events.
    *
    * @param key - The key that was pressed.
    ***************************************************************/
    virtual void onKeyPressed(char key) = 0;

    /****************************************************************
    * @brief Event handler for playing an animation.
    *
    * @param animation - The name of the animation to play.
    ***************************************************************/
    virtual void onAnimationPlay(const std::string& animation) = 0;

    virtual ~InputEventListener() = default;
};

/****************************************************************
* @brief Manager class to handle input event listeners and notify them.
***************************************************************/
class InputEventManager {
public:
    /****************************************************************
    * @brief Add an event listener to the manager.
    *
    * @param listener - Pointer to the listener to be added.
    ***************************************************************/
    void addListener(InputEventListener* listener) {
        listeners.push_back(listener);
    }

    /****************************************************************
    * @brief Remove an event listener from the manager.
    *
    * @param listener - Pointer to the listener to be removed.
    ***************************************************************/
    void removeListener(InputEventListener* listener) {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    }

    /****************************************************************
    * @brief Notify all listeners about a mouse button press event.
    *
    * @param button - The button that was pressed.
    ***************************************************************/
    void notifyMouseButtonPressed(const std::string& button) {
        for (auto* listener : listeners) {
            listener->onMouseButtonPressed(button);
        }
    }

    /****************************************************************
    * @brief Notify all listeners about a scroll event.
    *
    * @param scrollOffset - The scroll offset value (positive or negative).
    ***************************************************************/
    void notifyScrollEvent(int scrollOffset) {
        for (auto* listener : listeners) {
            listener->onScrollEvent(scrollOffset);
        }
    }

    /****************************************************************
    * @brief Notify all listeners about a key press event.
    *
    * @param key - The key that was pressed.
    ***************************************************************/
    void notifyKeyPressed(char key) {
        for (auto* listener : listeners) {
            listener->onKeyPressed(key);
        }
    }

    /****************************************************************
    * @brief Notify all listeners to play an animation.
    *
    * @param animation - The name of the animation to play.
    ***************************************************************/
    void notifyAnimationPlay(const std::string& animation) {
        for (auto* listener : listeners) {
            listener->onAnimationPlay(animation);
        }
    }

private:
    std::vector<InputEventListener*> listeners;
};

/****************************************************************
* @brief Example subscriber class that listens to input events.
***************************************************************/
class Player : public InputEventListener {
public:
    /****************************************************************
    * @brief Handle mouse button press event for the player.
    *
    * @param button - The button that was pressed.
    ***************************************************************/
    void onMouseButtonPressed(const std::string& button) override {
        if (button == "Left") {
            std::cout << "Left mouse button pressed!\n";
        }
    }

    /****************************************************************
    * @brief Handle mouse scroll event for the player.
    *
    * @param scrollOffset - The scroll offset value (positive or negative).
    ***************************************************************/
    void onScrollEvent(int scrollOffset) override {
        if (scrollOffset < 0) {
            std::cout << "Mouse scrolled down!\n";
        }
        else if (scrollOffset > 0) {
            std::cout << "Mouse scrolled up!\n";
        }
    }

    /****************************************************************
    * @brief Handle key press event for the player.
    *
    * @param key - The key that was pressed.
    ***************************************************************/
    void onKeyPressed(char key) override {
        if (key == 'U') {
            std::cout << "Sound stopped\n";
            // Assume playerSound is an available object with a Stop method
            // playerSound->Stop();
        }

        if (key == 'K') {
            std::cout << "K is pressed!\n";
            // Assume showDebugDraw is a static member to toggle debug display
            DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
        }
    }

    /****************************************************************
    * @brief Handle animation play event for the player.
    *
    * @param animation - The name of the animation to play.
    ***************************************************************/
    void onAnimationPlay(const std::string& animation) override {
        std::cout << "Playing animation: " << animation << "\n";
    }

    /****************************************************************
    * @brief Update player's state based on velocity and play appropriate animation.
    *
    * @param velocityX - The X component of the velocity.
    * @param velocityY - The Y component of the velocity.
    ***************************************************************/
    void update(float velocityX, float velocityY) {
        if (velocityX == 0.0f && velocityY == 0.0f) {
            onAnimationPlay("IdleAnimation");
        }
    }
};