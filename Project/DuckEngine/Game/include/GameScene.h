/******************************************************************************/
/*!
\file       GameScene.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Definition of the GameScene class, which handles the game logic
            for the main game scene. This includes loading assets, initializing
            game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Scene.h"

class GameScene : public  Scene{
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

    void FixedUpdate() override;

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

};