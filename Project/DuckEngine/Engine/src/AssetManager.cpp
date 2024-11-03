/******************************************************************************/
/*
\file:      AssetManager.cpp
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:	    yankai.tan@digipen.edu


\brief:     Contains the definitions that are used to manage the assets

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#include "AssetManager.h"
#include "ImageLoader.h"
#include "Texture.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> AssetManager::textureMap;
std::unordered_map<std::string, FMOD::Sound*> AssetManager::soundMap;
FMOD::System* AssetManager::fmodSystem = nullptr;

std::string NormalizePath(const std::string& path) {
	std::string normalizedPath = path;
	std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
	return normalizedPath;
}

void AssetManager::LoadAll()
{
	// Load all textures and sounds from respective directories
	LoadAllTextures("../Resources/Sprites");
	LoadAllSounds("../Resources/Sounds");

}

void AssetManager::LoadAllTextures(const std::string& directoryPath) {
	// Iterate over texture files in the specified directory
	for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
		if (entry.is_regular_file()) {
			std::string filePath = NormalizePath(entry.path().string());
			std::string fileExtension = entry.path().extension().string();

			// Check for valid image extensions
			if (fileExtension == ".png" || fileExtension == ".jpg" || fileExtension == ".jpeg") {
				LoadTexture(filePath);
				std::cout << "Loaded texture: " << filePath << std::endl;
			}
		}
	}
}


std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const std::string& filePath)
{
	if (textureMap.find(filePath) != textureMap.end())
	{
		return textureMap[filePath];
	}

	std::vector<std::shared_ptr<Texture>> textures;
	textures.push_back(LoadTextureFromFile(filePath));

	textureMap[filePath] = textures;

	return textures;
}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const std::string& filePath, int textureWidth, int textureHeight)
{
	if (textureMap.find(filePath) != textureMap.end())
	{
		return textureMap[filePath];
	}

	std::vector<std::shared_ptr<Texture>> textures = LoadTextureFromFile(filePath, textureWidth, textureHeight);

	textureMap[filePath] = textures;

	return textures;
}

// Unload a specific texture by its filename
void AssetManager::UnloadTexture(const std::string& fileName) {
	auto it = textureMap.find(fileName);
	if (it != textureMap.end()) {
		it->second.clear();  // Clear texture data
		textureMap.erase(it);  // Remove from textureMap
		std::cout << "Texture unloaded: " << fileName << std::endl;
	}
	else {
		std::cout << "Texture not found: " << fileName << std::endl;
	}
}

// Check if a texture with the specified filename is loaded
bool AssetManager::IsTextureLoaded(const std::string& fileName) {
	return textureMap.find(fileName) != textureMap.end();
}

// Reloads a texture from a file path and updates the texture map
void AssetManager::ReloadTexture(const std::string& fileName, const std::string& filePath) {
	if (IsTextureLoaded(fileName)) {
		textureMap[fileName].clear();
		textureMap[fileName].push_back(LoadTextureFromFile(filePath));
		std::cout << "Texture reloaded: " << fileName << std::endl;
	}
}

// Get a texture by its filename if it's loaded
std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& fileName) {
	if (IsTextureLoaded(fileName)) {
		return textureMap[fileName][0];
	}
	return nullptr;
}

std::shared_ptr<Texture> AssetManager::LoadTextureFromFile(const std::string& filePath)
{
	Texture texture = ImageLoader::LoadTexture(filePath);
	return std::make_shared<Texture>(texture);

}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight)
{
	std::vector<Texture> textures = ImageLoader::LoadSpriteSheet(filePath, textureWidth, textureHeight);
	std::vector<std::shared_ptr<Texture>> texturePtrs;
	texturePtrs.reserve(textures.size());

	for (Texture& texture : textures)
	{
		texturePtrs.push_back(std::make_shared<Texture>(texture));
	}

	return texturePtrs;
}

FMOD::System*& AssetManager::GetFMODSystem() {
	return fmodSystem;
}

// Load all sounds from a specific directory
void AssetManager::LoadAllSounds(const std::string& directoryPath) {
	if (!fmodSystem) {
		std::cerr << "FMOD System not initialized!" << std::endl;
		return;
	}

	// Iterate over sound files in the specified directory
	for (const auto& entry : fs::directory_iterator(directoryPath)) {
		if (entry.is_regular_file()) {
			std::string filePath = NormalizePath(entry.path().string());
			std::string fileExtension = entry.path().extension().string();

			// Load the sound if it has a valid audio extension
			if (fileExtension == ".wav" || fileExtension == ".mp3" || fileExtension == ".ogg") {
				LoadSound(filePath, filePath);
				std::cout << "Loaded sound: " << filePath << " from " << filePath << std::endl;
			}
		}
	}
}

void AssetManager::LoadSound(const std::string& soundID, const std::string& filePath) {
	if (!fmodSystem) {
		std::cerr << "FMOD System not initialized!" << std::endl;
		return;
	}

	if (soundMap.find(soundID) != soundMap.end()) return;  // Already loaded

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK) {
		std::cerr << "Error loading sound: " << filePath << std::endl;
		return;
	}
	soundMap[soundID] = sound;
}

FMOD::Sound* AssetManager::GetSounds(const std::string& soundID) {
	auto it = soundMap.find(soundID);
	return it != soundMap.end() ? it->second : nullptr;
}

void AssetManager::UnloadAll()
{
	textureMap.clear();

	// sound
	for (auto& [id, sound] : soundMap) {
		sound->release();
	}
	soundMap.clear();
	if (fmodSystem) {
		fmodSystem->close();
	}
}