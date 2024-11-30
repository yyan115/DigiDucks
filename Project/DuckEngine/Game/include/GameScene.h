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

    /****************************************************************
    * @brief Load all necessary resources for the scene.
    * This function is called before the scene starts.
    * ****************************************************************/
    void Load() override;
    
	/****************************************************************
	* @brief Start the scene, initializing game objects and 
    * setting up the initial state. This function is called when 
    * the scene begins running.
	* ****************************************************************/
    void Start() override;

	/****************************************************************
	* @brief Update the scene logic each frame, including processing
	* input and updating game objects. This function is called 
	* every frame. 
	* ****************************************************************/
    void Update() override;

    /****************************************************************
	* @brief PostUpdate function performs any operations after the
	* main update logic, such as cleanup or post-processing. This
	* function is called every frame, after the Update() method.
	* ****************************************************************/
    void PostUpdate() override;
    
	/****************************************************************
	* @brief Exit the scene, performing any cleanup necessary before
	* the scene is unloaded.
	* ****************************************************************/    
	void Exit() override;

    /****************************************************************
	* @brief Unload the scene and free any resources that were loaded
	* during the Load() phase.
	* ****************************************************************/
    void Unload() override;

	/****************************************************************
	* @brief Update the order texture to display the current order.
	* ****************************************************************/
	void UpdateOrderTexture();

	/****************************************************************
	* @brief Pause the game and display the pause menu.
	* @param isPaused - true if the game is paused, false if the game
	* is unpaused.
	* ****************************************************************/
	void PauseGame(bool);

	/****************************************************************
	* @brief Display the How To Play menu.
	* ****************************************************************/
	void HTPShow(bool);

};