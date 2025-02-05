/******************************************************************************/
/*!
\file       Emitter.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       Feburary 6 2025
\brief      Defines the Emitter struct, which is used for the Particle System. Contains
            all the data needed to render one type of particle.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Color.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/**
 * An EmitterDef describes the default parameters or ranges
 * for a particular "type" of particle effect.
 */
struct Emitter
{
    int spawnCountMin = 1;
    int spawnCountMax = 3;

    float lifetimeMin = 0.6f;
    float lifetimeMax = 1.2f;

    float scaleMin = 0.1f;
    float scaleMax = 0.3f;

    float speedMin = 0.0f;
    float speedMax = 1.0f;

    Color baseColor = { 128,128,128,255 };  // Default grey

    int layer = 0;
    int sortingOrder = 0;

    // Can add more options like "gravity", "rotationSpeed", etc, if needed.
};