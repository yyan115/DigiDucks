#include "Telemetry.h"

#ifdef QUACK_KITCHEN_TELEMETRY

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <string>

#include "DuckEngine.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "SceneManager.h"

namespace
{
	std::ofstream stream;
	double lastSampleSeconds = 0.0;
	bool sceneCensusWritten = false;
	std::string lastScene;

	// Four samples a second is plenty to steer by and keeps the file small
	// enough to read in one go after a run.
	constexpr double SampleInterval = 0.25;

	double NowSeconds()
	{
		using namespace std::chrono;
		return duration<double>(steady_clock::now().time_since_epoch()).count();
	}

	void WriteEntity(const char* key, const std::string& name)
	{
		auto entity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(name);
		if (!entity)
		{
			return;
		}
		auto* transform = DuckEngine::DUCKENGINE_ComponentManager
			.GetComponent<TransformComponent>(entity->entityID);
		if (!transform)
		{
			return;
		}
		const Vec2 position = transform->GetPosition();
		stream << ",\"" << key << "\":{\"x\":" << position.x
			<< ",\"y\":" << position.y << "}";
	}
}

void Telemetry::Initialise()
{
	const char* path = std::getenv("QUACK_KITCHEN_TELEMETRY_PATH");
	if (!path || !*path)
	{
		return;
	}
	stream.open(path, std::ios::out | std::ios::trunc);
}

void Telemetry::Sample()
{
	if (!stream.is_open())
	{
		return;
	}
	const double now = NowSeconds();
	if (now - lastSampleSeconds < SampleInterval)
	{
		return;
	}
	lastSampleSeconds = now;

	const std::string scene =
		DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName();
	if (scene != lastScene)
	{
		lastScene = scene;
		sceneCensusWritten = false;
	}

	stream << "{\"t\":" << now << ",\"scene\":\"" << scene << "\"";
	WriteEntity("player", "Player");
	stream << "}\n";
	stream.flush();
}

void Telemetry::Shutdown()
{
	if (stream.is_open())
	{
		stream.flush();
		stream.close();
	}
}

#else

void Telemetry::Initialise() {}
void Telemetry::Sample() {}
void Telemetry::Shutdown() {}

#endif
