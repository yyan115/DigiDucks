/******************************************************************************/
/*!
\file       TextSystem.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Declares the TextSystem class, responsible for managing text
			components, handling rendering and updating of text entities in the engine.

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

class DUCKENGINE_API TextSystem : public System
{
public:

	/// <summary>
	/// Initializes the TextSystem, setting up any resources or states required 
	/// for managing and rendering text components in the engine.
	/// </summary>
	void Start() override;

	/// <summary>
	/// Updates all active text components in the system. This includes adjusting
	/// properties, such as position and color, based on the current frame's state.
	/// </summary>
	void Update() override;

	/// <summary>
	/// Renders all active text components in the system, drawing the text elements
	/// with their current properties (font, color, position, etc.) onto the screen.
	/// </summary>
	void Render() override;

};