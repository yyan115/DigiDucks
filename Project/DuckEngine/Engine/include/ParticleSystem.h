#pragma once
#include "GraphicsManager.h"
#include "System.h"       // Your base System class
#include "Vector2.h"
#include "Color.h"
#include <vector>
#include <iostream>

/// <summary>
/// A simple particle struct for cartoonish smoke or any effect.
/// </summary>
struct Particle
{
    bool active = false;
    Vector2D position;
    Vector2D velocity;
    float lifetime = 0.0f;      // current time left
    float startLifetime = 1.0f; // used to fade out
    float scale = 1.0f;         // used as radius
    Color color;                // holds alpha in color.a
};

/// <summary>
/// ParticleSystem inherits from System (your engine base).
/// It manages a pool of Particle objects, updating & rendering them each frame.
/// </summary>
class ParticleSystem : public System
{
public:
    /// <summary>
    /// Constructor: initializes a pool of 'maxCount' particles.
    /// </summary>
    /// <param name="maxCount">Max number of particles in the pool.</param>
    ParticleSystem(int maxCount = 100);

    /// <summary>
    /// Overrides the System::Start. Here we can init anything if needed.
    /// </summary>
    void Start() override;

    /// <summary>
    /// Overrides the System::Update. Called each frame to update particles.
    /// </summary>
    void Update() override;

    /// <summary>
    /// Overrides System::Render. Renders all active particles.
    /// </summary>
    void Render() override;

    static void RenderTemp()
    {
        // Render all active particles
        for (auto& p : m_particles)
        {
            if (!p.active)
                continue;

            // We'll call a new function in GraphicsManager: DrawFilledCircle
            // 'true' means we want them in world space (camera transforms).
            GraphicsManager::DrawFilledCircle(
                p.position,
                p.scale,
                p.color,
                /*relativeToCamera=*/ true
            );

            std::cout << "drawn\n";
        }

        std::cout << "drawn end\n";
    }

    /// <summary>
    /// Spawn (emit) a new particle at 'pos' with velocity 'vel'.
    /// Randomizes some properties like lifetime, scale, color, etc.
    /// </summary>
    /// <param name="pos">Spawn position in world coords.</param>
    /// <param name="vel">Initial velocity of the particle.</param>
    static void Emit(const Vector2D& pos, const Vector2D& vel);

private:
    static std::vector<Particle> m_particles;

    /// <summary>
    /// Helper function to update all active particles:
    /// moves them, fades alpha, deactivates them if lifetime <= 0
    /// </summary>
    /// <param name="dt">Delta time in seconds.</param>
    void UpdateParticles(float dt);
};

