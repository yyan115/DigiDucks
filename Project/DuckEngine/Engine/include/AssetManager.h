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
#include <fmod.hpp>



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

	static DUCKENGINE_API void LoadAllTextures(const std::string& directoryPath);
	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath);
	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath, int textureWidth, int textureHeight);
	static DUCKENGINE_API bool IsTextureLoaded(const std::string& fileName);
	static DUCKENGINE_API void UnloadTexture(const std::string& fileName);
	static DUCKENGINE_API void ReloadTexture(const std::string& fileName, const std::string& filePath);
	static DUCKENGINE_API std::shared_ptr<Texture> GetTexture(const std::string& fileName);

	static DUCKENGINE_API void LoadAllSounds(const std::string& directoryPath);
	static DUCKENGINE_API void LoadSound(const std::string& soundID, const std::string& filePath);
	static DUCKENGINE_API void LoadAllShaders(const std::string& directoryPath);
	static DUCKENGINE_API void LoadAllFonts(const std::string& directoryPath);

	// Get
	static DUCKENGINE_API FMOD::System*& GetFMODSystem();
	static DUCKENGINE_API FMOD::Sound* GetSounds(const std::string& soundID);
	static DUCKENGINE_API const std::vector<std::string>& GetFontNames();
	
	// unload all textures
	static DUCKENGINE_API void UnloadAll();

private:
	// Stores all loaded textures with their file paths as keys
	static std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> textureMap;

	// Helper function to load a texture from the file
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight);

	// Stores all loaded sounds with their file paths as keys
	static std::unordered_map<std::string, FMOD::Sound*> soundMap;
	static FMOD::System* fmodSystem;

	//Stores all font names
	static std::vector<std::string> fontNames;

};