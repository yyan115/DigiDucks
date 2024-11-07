/******************************************************************************
\file       Scene.h
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Declaration of the Scene class, which manages layers and entities
			within a game scene. The class provides methods for lifecycle
			management, including loading, updating, and unloading the scene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

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
	/**************************************************************************
	@brief Pure virtual function to load resources and initialize the scene.
	**************************************************************************/
	DUCKENGINE_API virtual void Load() = 0;

	/**************************************************************************
	@brief Pure virtual function called at the start of the scene.
	**************************************************************************/
	DUCKENGINE_API virtual void Start() = 0;

	/**************************************************************************
	@brief Pure virtual function called every frame to update the scene.
	**************************************************************************/
	DUCKENGINE_API virtual void Update() = 0;

	/**************************************************************************
	@brief Pure virtual function called after each update, typically for post-
		   update tasks.
	**************************************************************************/
	DUCKENGINE_API virtual void PostUpdate() = 0;

	/**************************************************************************
	@brief Pure virtual function to handle exit procedures for the scene.
	**************************************************************************/
	DUCKENGINE_API virtual void Exit() = 0;

	/**************************************************************************
	@brief Pure virtual function to unload resources associated with the scene.
	**************************************************************************/
	DUCKENGINE_API virtual void Unload() = 0;

	/**************************************************************************
	@brief Virtual destructor for Scene, allowing safe inheritance.
	**************************************************************************/
	DUCKENGINE_API virtual ~Scene() = default;

	/**************************************************************************
	@brief Adds a layer to the scene if it doesn’t already exist.
	@param name The name of the layer to add.
	@param layer The Layer object to add to the scene.
	**************************************************************************/
	DUCKENGINE_API void AddLayer(const std::string& name, Layer layer);

	/**************************************************************************
	@brief Retrieves a pointer to a layer by name.
	@param name The name of the layer to retrieve.
	@return Pointer to the layer if found, nullptr otherwise.
	**************************************************************************/
	DUCKENGINE_API Layer* GetLayer(const std::string& name);

	/**************************************************************************
	@brief Gets a constant reference to the layers map in the scene.
	@return An unordered_map containing the layers in the scene.
	**************************************************************************/
	DUCKENGINE_API const std::unordered_map<std::string, Layer>& GetLayers() const;

	/**************************************************************************
	@brief Adds an entity to a specified layer within the scene.
	@param layerName The name of the layer to add the entity to.
	@param entity Pointer to the entity to add.
	**************************************************************************/
	void AddEntityToLayer(const std::string& layerName, Entity* entity);

	std::string sceneName;
private:
	std::unordered_map<std::string, Layer> layers; // Layers in the scene
};