/******************************************************************************/
/*
\file:		AssetManager.h
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:		yankai.tan@digipen.edu


\brief:     Contains the declarations that are used to manage the assets

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <string>
#include <memory>


#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

typedef unsigned int Texture;

class AssetManager
{
public:
	static DUCKENGINE_API void LoadAll();

	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath);
	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath, int textureWidth, int textureHeight);
	static DUCKENGINE_API void LoadSound(const std::string& soundName, const std::string& filePath);

	// unload all textures
	static DUCKENGINE_API void UnloadAll();

private:
	// Stores all loaded textures with their file paths as keys
	static std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> textureMap;

	// Helper function to load a texture from the file
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight);

};