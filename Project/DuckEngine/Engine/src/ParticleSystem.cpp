#include "ParticleSystem.h"
#include "GraphicsManager.h"    // We'll add a new DrawFilledCircle(...) function there
#include <random>
#include <iostream>

std::vector<Particle> ParticleSystem::m_particles;

// If your engine provides a dt from somewhere, adapt this accordingly.
// For demonstration, let's assume "DuckEngine::GetDeltaTime()" or "DuckEngine::deltaTime" exists.
// We'll just define a small helper function here for random floats:
static float RandomRange(float minVal, float maxVal)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(minVal, maxVal);
    return dist(rng);
}

ParticleSystem::ParticleSystem(int maxCount /*=100*/)
{
    m_particles.resize(maxCount);
}

void ParticleSystem::Start()
{
    // Called once when the system is first started.
    // You can do any initialization here if needed.
    std::cout << "ParticleSystem: Start() called. Pool size = "
        << m_particles.size() << "\n";
}

void ParticleSystem::Update()
{
    // We assume your engine calls this once per frame. 
    // We'll guess dt is provided by the engine somehow.
    // If you have a "DuckEngine::GetDeltaTime()", do that.
    float dt = 1.0f / 60.0f;  // fallback if you don't have an actual dt
    // If your engine offers dt, do:
    // float dt = DuckEngine::deltaTime;

    UpdateParticles(dt);
}

void ParticleSystem::Render()
{
    //// Render all active particles
    //for (auto& p : m_particles)
    //{
    //    if (!p.active)
    //        continue;

    //    // We'll call a new function in GraphicsManager: DrawFilledCircle
    //    // 'true' means we want them in world space (camera transforms).
    //    GraphicsManager::DrawFilledCircle(
    //        p.position,
    //        p.scale,
    //        p.color,
    //        /*relativeToCamera=*/ true
    //    );
    //}
}

void ParticleSystem::Emit(const Vector2D& pos, const Vector2D& vel)
{
    // Find first inactive particle
    for (auto& p : m_particles)
    {
        if (!p.active)
        {
            p.active = true;
            p.position = pos;
            p.velocity = vel;

            p.lifetime = RandomRange(0.6f, 1.2f);
            p.startLifetime = p.lifetime;

            p.scale = RandomRange(0.1f, 0.3f);
            p.color = { 128, 128, 128, 255 }; // Medium gray
            return;
        }
    }
    // If we get here, no inactive found => skip or reuse index 0
}

void ParticleSystem::UpdateParticles(float dt)
{
    for (auto& p : m_particles)
    {
        if (!p.active)
            continue;

        // Move
        p.position += p.velocity * dt;

        // Fade out
        p.lifetime -= dt;
        float frac = p.lifetime / p.startLifetime;
        if (frac < 0.0f)
            frac = 0.0f;

        p.color.a = static_cast<unsigned char>(frac * 255);

        // Expand slightly
        p.scale += (dt * 0.1f);

        // Deactivate if done
        if (p.lifetime <= 0.f)
        {
            p.active = false;
        }
    }

    //std::cout << "updated\n";
}
