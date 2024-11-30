/******************************************************************************/
/*!
\file       HowToPlay.h
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Definition of the HowToPlay class, it loads up the screen to show the instructions on how to play the game whenever it is triggered.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Scene.h"

class HowToPlay : public Scene {
    /****************************************************************
    * @brief Load all necessary resources for the scene.
    * This function is called before the scene starts.
    * ****************************************************************/
    void Load() override;

    /****************************************************************
    * @brief Starts the scene, initializing game objects and setting up the initial state.
    * This function is called when the scene begins running.
    * ****************************************************************/
    void Start() override;

    /****************************************************************
    * @brief Updates the scene logic each frame, including processing input and updating game objects.
    * This function is called every frame.
    * ****************************************************************/
    void Update() override;

    /****************************************************************
    * @brief Performs any operations after the main update logic, such as cleanup or post-processing.
    * This function is called every frame, after the Update() method.
    * ****************************************************************/
    void PostUpdate() override;

    /****************************************************************
    * @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
    * ****************************************************************/
    void Exit() override;

    /****************************************************************
    * @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
    * ****************************************************************/
    void Unload() override;
};