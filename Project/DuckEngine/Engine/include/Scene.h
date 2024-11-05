#pragma once

#include <unordered_map>
#include "Layer.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class Scene
{
public:
	DUCKENGINE_API virtual void Load() = 0;
	DUCKENGINE_API virtual void Start() = 0;
	DUCKENGINE_API virtual void Update() = 0;
	DUCKENGINE_API virtual void PostUpdate() = 0;
	DUCKENGINE_API virtual void Exit() = 0;
	DUCKENGINE_API virtual void Unload() = 0;

	DUCKENGINE_API virtual ~Scene() = default;

	DUCKENGINE_API void AddLayer(const std::string& name, Layer layer);
	DUCKENGINE_API Layer* GetLayer(const std::string& name);
	DUCKENGINE_API const std::unordered_map<std::string, Layer>& GetLayers() const;
	void AddEntityToLayer(const std::string& layerName, Entity* entity);
private:
	std::unordered_map<std::string, Layer> layers; // Layers in the scene
};