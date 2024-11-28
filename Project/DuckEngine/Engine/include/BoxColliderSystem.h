/******************************************************************************/
/*!
\file		BoxColliderSystem.h
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Sep 20 2024
\brief  	Implementation of the BoxColliderSystem class for handling 
			collision detection and response between bounding boxes and circles.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "System.h"
#include "DuckEngine.h"
#include "Bounding.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API BoxColliderSystem : public System
{
public:
	void Start() override;
	void EditorUpdate() override;
	void Update() override;
	void Render() override;
}; 
