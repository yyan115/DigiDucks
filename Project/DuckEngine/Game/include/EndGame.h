/******************************************************************************/
/*!
\file       EndGame.h
\author     Tan Yan Kai, yankai.tan, 2301312
\par        yankai.tan@digipen.edu
\date       November 30 2024
\brief      Definition of the EndScene class, it loads up the necessary assets needed for the UI and display of the game score before going back to main menu.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Scene.h"
#include "Entity.h"
#include "DuckEngine.h"

class EndScene : public  Scene {
public:
    /// <summary>
    /// Loads all necessary resources for the scene.
    /// This function is called before the scene starts.
    /// </summary>
    void Load() override;

    /// <summary>
    /// Starts the scene, initializing game objects and setting up the initial state.
    /// This function is called when the scene begins running.
    /// </summary>
    void Start() override;

    /// <summary>
    /// Updates the scene logic each frame, including processing input and updating game objects.
    /// This function is called every frame.
    /// </summary>
    void Update() override;

    /// <summary>
    /// Performs any operations after the main update logic, such as cleanup or post-processing.
    /// This function is called every frame, after the Update() method.
    /// </summary>
    void PostUpdate() override;

    /// <summary>
    /// Exits the scene, performing any cleanup necessary before the scene is unloaded.
    /// </summary>
    void Exit() override;

    /// <summary>
    /// Unloads the scene and frees any resources that were loaded during the Load() phase.
    /// </summary>
    void Unload() override;

private:
    Entity* MainMenuButton = nullptr;
    ButtonComponent* MainMenu = nullptr;
    SpriteRendererComponent* MainMenu_Spt = nullptr;

    ButtonComponent* LastMainMenu = nullptr;
    SpriteRendererComponent* LastMainMenu_Spt = nullptr;

    Entity* Star_1 = nullptr;
    SpriteRendererComponent* Star1 = nullptr;

    Entity* Star_2 = nullptr;
    SpriteRendererComponent* Star2 = nullptr;

    Entity* Star_3 = nullptr;
    SpriteRendererComponent* Star3 = nullptr;

    Entity* Score = nullptr;
    TextComponent* ScoreText = nullptr;

    Entity* Background = nullptr;
    SoundComponent* BGMSound = nullptr;
    bool isHover = false;

    TextComponent* FPSText = nullptr;

    Entity* FadeInScreen = nullptr;
    SpriteRendererComponent* FadeInSpriteRenderer = nullptr;

    Entity* RestartButton = nullptr;
    ButtonComponent* Restart = nullptr;
    SpriteRendererComponent* Restart_Spt = nullptr;

    Entity* NextButton = nullptr;
    ButtonComponent* Next = nullptr;
    SpriteRendererComponent* Next_Spt = nullptr;

    float fadeInDuration = 3.0f;  // Duration for fade-in effect
    float fadeInElapsedTime = 0.0f;
    bool isFadingIn = false;

    int iStar_1, iStar_2, iStar_3;

    bool isUsingController = false;
    float controllerNavigationCooldown = 0.0f;
    const float controllerNavigationDelay = 0.2f;
    const float buttonScaleIncrease = 1.1f;

    enum class EndButtonSelection {
        RESTART = 0,
        MENU = 1,
        NEXT = 2,
        LASTMENU = 3,
        COUNT
    };

    enum class EndSceneState {
        WIN,         // Normal win - Next and Menu buttons
        LOSE,        // Loss - Restart and Menu buttons
        COMPLETE     // Final level completion - Only LastMenu button
    };

    EndSceneState currentState = EndSceneState::LOSE;
    EndButtonSelection currentButtonSelection = EndButtonSelection::RESTART;

    TransformComponent* restartTransform = nullptr;
    TransformComponent* menuTransform = nullptr;
    TransformComponent* nextTransform = nullptr;
    TransformComponent* lastMenuTransform = nullptr;
    Vec2 restartOriginalScale;
    Vec2 menuOriginalScale;
    Vec2 nextOriginalScale;
    Vec2 lastMenuOriginalScale;

    void UpdateEndMenuSelection();
    void SelectButton(EndButtonSelection selection);
    void DeselectAllButtons();
    void ActivateSelectedButton(const std::string& lastPlayedSceneName);
    void DetermineCurrentState();
};