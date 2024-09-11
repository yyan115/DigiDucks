#pragma once

#include "System.h"
#include "DuckEngine.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API CameraSystem : public System
{
public:
	void Update() override;

	//static inline std::vector<Entity*> GetActiveCameras() { return activeCameras; };

private:

	// Pointer to all the entities with an active camera component
	//static std::vector<Entity*> activeCameras;
};