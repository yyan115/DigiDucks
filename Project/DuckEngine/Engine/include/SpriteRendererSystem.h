/******************************************************************************/
/*!
\file       SpriteRendererSystem.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the SpriteRendererSystem class, which is responsible for
			managing and rendering sprites in the game engine. This system
			handles the initialization and updates for rendering sprites.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "System.h"
#include "DuckEngine.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The SpriteRendererSystem class is responsible for managing the
	   initialization and rendering of sprites in the game engine. It
	   inherits from the System base class.
*************************************************************************/
class DUCKENGINE_API SpriteRendererSystem : public System
{
public:

	/************************************************************************
	@brief Initializes the SpriteRendererSystem. This method is called once
		   at the start of the system's lifecycle to set up necessary resources.
	*************************************************************************/
	void Start() override;

	/************************************************************************
	@brief Updates the SpriteRendererSystem. This method is called every frame
		   to update the rendering of sprites.
	*************************************************************************/
	void Update() override;

};