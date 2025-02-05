#include "ParticleManager.h"

#include <random>
#include <iostream>
#include <cmath>    // for sinf, cosf if you want random directions
#include "GraphicsManager.h"   // to add draw commands
#include "RenderData.h"        // for DrawOptions or whatever data you need
#include "DuckEngine.h"        // for deltaTime

std::vector<Particle> ParticleManager::m_particles;            // The global pool
std::unordered_map<std::string, Emitter> ParticleManager::m_defs;  // Emitter definitions

namespace {
    /**
     * A small helper for random floats in [minVal, maxVal].
     */
    static float RandRange(float minVal, float maxVal)
    {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(minVal, maxVal);
        return dist(rng);
    }

    /**
     * Generates a random velocity in a circular range.
     */
    static Vector2D RandDirection2D(float rmin, float rmax)
    {
        float angle = RandRange(0.0f, 3.14159f * 2.0f);
        float mag = RandRange(rmin, rmax);
        return { cosf(angle) * mag, sinf(angle) * mag };
    }
}

//ParticleManager::ParticleManager(int maxParticles)
//{
//    m_particles.resize(maxParticles);
//}

void ParticleManager::Start(int maxParticles)
{
    m_particles.resize(maxParticles);
    std::cout << "[ParticleManager] Start. Pool size = " << m_particles.size() << "\n";
}

void ParticleManager::Update()
{
    float dt = DuckEngine::DeltaTime();  // or whatever your engine uses
    UpdateParticles(dt);
}

void ParticleManager::Render()
{
    int count = 0;

    for (auto& p : m_particles)
    {
        if (!p.active) continue;

        // Add draw command to the GraphicsManager queue
        ParticleRenderCommand drawOpt;
        drawOpt.scale = { p.scale * 2.0f, p.scale * 2.0f };
        drawOpt.rotation = 0.0f;
        drawOpt.translation = p.position;
        drawOpt.useColor = true;
        drawOpt.color = p.color;
        drawOpt.relativeToCamera = true;

        UnifiedRenderCommand cmd;
        cmd.layer = p.layer;
        cmd.sortingOrder = p.sortingOrder;
        cmd.type = RenderCommandType::Particle;
        cmd.command = drawOpt;

        GraphicsManager::AddToDrawQueue(cmd);
        count++;
    }

    std::cout << "particle count: " << count << "\n";
    count = 0;
}

void ParticleManager::RegisterEmitter(const std::string& type, const Emitter& def)
{
    m_defs[type] = def;
}

void ParticleManager::Emit(const std::string& type, const Vector2D& pos, const Vector2D& baseVel /*= {0,0}*/)
{
    auto it = m_defs.find(type);
    if (it == m_defs.end())
    {
        std::cout << "[ParticleManager] WARNING: No EmitterDef registered for type '" << type << "'!\n";
        return;
    }
    const Emitter& def = it->second;

    int spawnCount = static_cast<int>(RandRange(
        float(def.spawnCountMin),
        float(def.spawnCountMax + 0.999f)
    ));

    for (int i = 0; i < spawnCount; ++i)
    {
        SpawnOneParticle(def, pos, baseVel);
    }
}

void ParticleManager::SpawnOneParticle(const Emitter& def, const Vector2D& pos, const Vector2D& baseVel)
{
    for (auto& p : m_particles)
    {
        if (!p.active)
        {
            p.active = true;
            p.position = pos;
            p.lifetime = RandRange(def.lifetimeMin, def.lifetimeMax);
            p.startLifetime = p.lifetime;
            p.color = def.baseColor;
            p.scale = RandRange(def.scaleMin, def.scaleMax);
            p.velocity = baseVel + RandDirection2D(def.speedMin, def.speedMax);
            return;
        }
    }
}

void ParticleManager::UpdateParticles(float dt)
{
    for (auto& p : m_particles)
    {
        if (!p.active) continue;

        p.position += p.velocity * dt;

        p.lifetime -= dt;
        float frac = p.lifetime / p.startLifetime;
        p.color.a = static_cast<unsigned char>(frac * 255);

        if (p.lifetime <= 0.0f)
        {
            p.active = false;
        }
    }
}
