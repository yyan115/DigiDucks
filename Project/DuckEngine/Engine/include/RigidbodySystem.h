/******************************************************************************/
/*!
\file		RigidbodySystem.h
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Sep 28 2024
\brief  	Implements the RigidbodySystem for managing physics-related 
            updates for entities with RigidbodyComponents.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "System.h"
#include "DuckEngine.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API RigidbodySystem : public System
{
public:
	void Start() override;
	void Update() override;

};