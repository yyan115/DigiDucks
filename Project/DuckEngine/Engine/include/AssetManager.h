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
#include "Serialization.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

typedef unsigned int Texture;

class AssetManager
{
public:
	/************************************************************************
	@brief Loads all textures,sounds, shaders from the specified file path for the editor an game
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadAll();
	/************************************************************************
	@brief Unloads all textures,sounds, shaders from the specified file path for the editor an game
	@return void
	*************************************************************************/
	static DUCKENGINE_API void UnloadAll();


	// Textures
	/************************************************************************
	@brief Loads all textures from the specified file path for the editor an game
	@param directoryPath The path to the directory containing the textures
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadAllTextures(const std::string& directoryPath);

	/************************************************************************
	@brief Loads a texture from the specified file path
	@param filePath The path to the texture file
	@return vector of shared pointers to the loaded texture
	*************************************************************************/
	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath);

	/************************************************************************
	@brief Loads a texture from the specified file path with the specified width and height
	@param filePath The path to the texture file
	@param textureWidth The width of the texture
	@param textureHeight The height of the texture
	@return vector of shared pointers to the loaded texture
	*************************************************************************/
	static DUCKENGINE_API std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath, int textureWidth, int textureHeight);

	/************************************************************************
	@brief Checks if a texture is loaded
	@param fileName The name of the texture file
	@return bool
	*************************************************************************/
	static DUCKENGINE_API bool IsTextureLoaded(const std::string& fileName);

	/************************************************************************
	@brief Unloads a texture from the specified file path
	@param fileName The name of the texture file
	@return void
	*************************************************************************/
	static DUCKENGINE_API void UnloadTexture(const std::string& fileName);

	/************************************************************************
	@brief Reloads a texture from the specified file path
	@param fileName The name of the texture file
	@param filePath The path to the texture file
	@return bool
	*************************************************************************/
	static DUCKENGINE_API void ReloadTexture(const std::string& fileName, const std::string& filePath);

	/************************************************************************
	@brief Gets a texture from the specified file path
	@param fileName The name of the texture file
	@return shared pointer to the loaded texture
	*************************************************************************/
	static DUCKENGINE_API std::shared_ptr<Texture> GetTexture(const std::string& fileName);

	/************************************************************************
	@brief Gets a texture based on the texture name
	@param textureName The name of the texture
	@return Texture
	*************************************************************************/
	static DUCKENGINE_API Texture GetTextureByName(const std::string& textureName);

	/************************************************************************
	@brief Gets the texture path based on the texture ID
	@param textureID The ID of the texture
	@return string containing the texture path
	*************************************************************************/
	static DUCKENGINE_API std::string GetTexturePath(Texture textureID);


	// Sounds
	/************************************************************************
	@brief Loads all sounds from the specified file path
	@param directoryPath The path to the directory containing the sounds
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadAllSounds(const std::string& directoryPath);

	/************************************************************************
	@brief Loads a sound from the specified file path
	@param soundID The ID of the sound
	@param filePath The path to the sound file
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadSound(const std::string& soundID, const std::string& filePath);

	/************************************************************************
	@brief Unloads a sound from the specified file path
	@param soundID The ID of the sound
	@return void
	*************************************************************************/
	static DUCKENGINE_API void UnloadSound(const std::string& soundID);

	/************************************************************************
	@brief Reloads a sound from the specified file path
	@param soundID The ID of the sound
	@param filePath The path to the sound file
	@return void
	*************************************************************************/
	static DUCKENGINE_API void ReloadSound(const std::string& soundID, const std::string& filePath);

	/************************************************************************
	@brief Gets the FMOD system
	@return FMOD system
	*************************************************************************/
	static DUCKENGINE_API FMOD::System*& GetFMODSystem();

	/************************************************************************
	@brief Gets a sound from the specified SOUNDID
	@param soundID The ID of the sound
	@return FMOD sound
	*************************************************************************/
	static DUCKENGINE_API FMOD::Sound* GetSounds(const std::string& soundID);


	// Shaders
	/************************************************************************
	@brief Loads all shaders from the specified file path
	@param directoryPath The path to the directory containing the shaders
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadAllShaders(const std::string& directoryPath);


	// Fonts
	/************************************************************************
	@brief Loads all fonts from the specified file path
	@param directoryPath The path to the directory containing the shaders
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadAllFonts(const std::string& directoryPath);

	/************************************************************************
	@brief Gets all font names
	@return vector of strings containing all font names
	*************************************************************************/
	static DUCKENGINE_API const std::vector<std::string>& GetFontNames();


	// Scenes
	/************************************************************************
	@brief Preloads all scenes from the specified file path
	@param directoryPath The path to the directory containing the shaders
	@return void
	*************************************************************************/
	static DUCKENGINE_API void PreloadScenes(const std::string& directoryPath);
	/************************************************************************
	@brief Gets the level data from the specified file path
	@paraf levelName The name of the level
	@return json object containing the level data
	*************************************************************************/
	static DUCKENGINE_API nlohmann::json GetLevelData(const std::string& levelName);

	/************************************************************************
	@brief Adds an asset to the specified destination folder
	@param sourcePath The path to the source file
	@param destinationFolder The path to the destination folder
	@return bool
	*************************************************************************/
	static DUCKENGINE_API bool AddAsset(const std::string& sourcePath, const std::string& destinationFolder);

	/************************************************************************
	@brief Removes an asset from the specified destination folder
	@param assetPath The path to the asset file
	@return bool
	*************************************************************************/
	static DUCKENGINE_API bool RemoveAsset(const std::string& assetPath);

	/************************************************************************
	@brief Loads a font from the specified file path
	@param fontName The name of the font
	@param filePath The path to the font file
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadFont(const std::string& fontName, const std::string& filePath);

	/************************************************************************
	@brief Unloads a font from the specified file path
	@param fontName The name of the font
	@return void
	*************************************************************************/
	static DUCKENGINE_API void UnloadFont(const std::string& fontName);

	/************************************************************************
	@brief Loads a shader from the specified file path
	@param shaderName The name of the shader
	@param filePath The path to the shader file
	@return void
	*************************************************************************/
	static DUCKENGINE_API void LoadShader(const std::string& shaderName, const std::string& filePath);

	/************************************************************************
	@brief Unloads a shader from the specified file path
	@param shaderName The name of the shader
	@return void
	*************************************************************************/
	static DUCKENGINE_API void UnloadShader(const std::string& shaderName);


private:
	// Stores all loaded textures with their file paths as keys
	static std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> textureMap;
	static std::unordered_map<std::string, std::string> nameToFilePath;

	// Helper function to load a texture from the file
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight);

	// Stores all loaded sounds with their file paths as keys
	static std::unordered_map<std::string, FMOD::Sound*> soundMap;
	static FMOD::System* fmodSystem;

	//Stores all font names
	static std::vector<std::string> fontNames;

	// Stores all the scenes
	static std::unordered_map<std::string, nlohmann::json> levelDataMap;


};