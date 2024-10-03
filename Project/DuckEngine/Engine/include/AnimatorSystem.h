/******************************************************************************/
/*!
\file       AnimatorSystem.h
\author     Jovan chua 2301244
\par        email c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Holds the declarations of the Animator System.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include "DuckEngine.h"
#include "System.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief Manages the updating and processing of animations for entities with an AnimatorComponent.
*************************************************************************/
class AnimatorSystem : public System
{
public:

    DUCKENGINE_API void Start() override;

    DUCKENGINE_API void Update() override;

private:
    std::unordered_map<AnimatorComponent*, Animation*> activeAnimations;
};