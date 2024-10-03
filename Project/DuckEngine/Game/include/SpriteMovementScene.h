/******************************************************************************/
/*!
\file       SpriteMovementScene.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the SpriteMovementScene class, which is responsible for
			handling a scene where sprite movement is the primary focus. This
			class inherits from the base Scene class and implements methods
			for loading, starting, updating, and unloading the scene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Scene.h"

/************************************************************************
@brief The SpriteMovementScene class manages the lifecycle of a scene where
	   sprite movement occurs. It provides methods to load, start, update,
	   and unload the scene.
*************************************************************************/
class SpriteMovementScene : public Scene
{
public:
	/************************************************************************
	@brief Loads the resources needed for the scene. This method is called
		   before the scene starts.
	*************************************************************************/
	void Load() override;

	/************************************************************************
	@brief Starts the scene and initializes any necessary components.
	*************************************************************************/
	void Start() override;

	/************************************************************************
	@brief Updates the scene logic each frame, such as handling sprite movement.
	*************************************************************************/
	void Update() override;

	/************************************************************************
	@brief Called after the Update method to handle any post-update logic.
	*************************************************************************/
	void PostUpdate() override;

	/************************************************************************
	@brief Exits the scene, handling any cleanup required before switching
		   to another scene.
	*************************************************************************/
	void Exit() override;

	/************************************************************************
	@brief Unloads the resources used by the scene, freeing memory.
	*************************************************************************/
	void Unload() override;


};