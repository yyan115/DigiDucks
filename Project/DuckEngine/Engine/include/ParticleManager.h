/******************************************************************************/
/*!
\file       ParticleManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       Feburary 6 2025
\brief      Declares the ParticleManager class, which manages a global pool of 
            particles for the game engine. It supports multiple emitter types,
            spawns and updates particles over time, and integrates with the 
            GraphicsManager for rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "System.h"
#include "Vector2.h"
#include "Color.h"
#include "Emitter.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/**
 * A single Particle in the pool.
 */
struct Particle
{
    bool active = false;

    Vector2D position{ 0, 0 };
    Vector2D velocity{ 0, 0 };

    float lifetime = 0.0f;      // how many seconds left before it dies
    float startLifetime = 1.0f;     // the initial lifetime for fade-out computations

    float scale = 1.0f;       // can be used for radius or sprite scale
    Color color = { 255,255,255,255 };  // RGBA

    int layer = 0;
    int sortingOrder = 0;
};

/**
 * ParticleManager:
 *  - manages a global pool of Particles
 *  - has multiple "EmitterDefs" stored with string keys
 *  - spawns new Particles with ParticleManager::Emit()
 *  - updates them each frame in ParticleManager::Update()
 *  - renders them by adding draw commands to GraphicsManager
 */
class ParticleManager
{
public:

    // System overrides
    static void Start(int maxParticles = 2000);
    static void Update();
    static void Render();

    /**
     * Register a "type" of emitter using a string key.
     * Example: "dust", "chopping", "cooking_sparks"
     */
    static void RegisterEmitter(const std::string& type, const Emitter& def);

    /**
     * Emit spawns 1 or more Particles of the given type at position "pos".
     * Optionally pass a "direction" or velocity as well.
     */
    static void Emit(const std::string& type, const Vector2D& pos, const Vector2D& baseVel = { 0,0 });

private:
    static std::vector<Particle> m_particles;            // The global pool
    static std::unordered_map<std::string, Emitter> m_defs;  // Emitter definitions

    static void UpdateParticles(float dt);
    static void SpawnOneParticle(const Emitter& def, const Vector2D& pos, const Vector2D& baseVel);
};
