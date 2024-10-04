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
#include "DuckEngine_Sound.h"

std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> AssetManager::textureMap;


void AssetManager::LoadAll()
{
	//// load all assets here
	//LoadTexture("../Resources/oldman.png");

	//// load sound
	//LoadSound("TestSound", "../Resources/Sounds/twitchAlert.wav");
	//LoadSound("TestSound2", "../Resources/Sounds/magnetic.mp3");

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


void AssetManager::LoadSound(const std::string& soundName, const std::string& filePath)
{
	DuckEngine_Sound::LoadSound(soundName, filePath);
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

void AssetManager::UnloadAll()
{
	textureMap.clear();
}