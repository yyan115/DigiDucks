/******************************************************************************/
/*!
\file       ButtonSystem.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Declares the ButtonSystem class, which handles the logic for
			managing button states, including rendering, clicks, and hover effects.

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

class DUCKENGINE_API ButtonSystem : public System
{
public:

	/// <summary>
	/// Initializes the ButtonSystem, preparing it to handle button interactions 
	/// such as hover and click events, as well as rendering states.
	/// </summary>
	void Start() override;

	/// <summary>
	/// Updates all active button components in the system, including checking for 
	/// hover and click events and updating their visual states accordingly.
	/// </summary>
	void Update() override;

};