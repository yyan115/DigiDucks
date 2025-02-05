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

    // You can add more options like "gravity", "rotationSpeed", etc.
};