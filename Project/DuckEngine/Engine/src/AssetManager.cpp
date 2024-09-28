#include "AssetManager.h"
#include "ImageLoader.h"
#include "Texture.h"
#include "DuckEngine_Sound.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::textureMap;


void AssetManager::LoadAll()
{
	//// load all assets here
	//LoadTexture("../Resources/oldman.png");

	//// load sound
	//LoadSound("TestSound", "../Resources/Sounds/twitchAlert.wav");
	//LoadSound("TestSound2", "../Resources/Sounds/magnetic.mp3");

}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const std::string& filePath, int textureWidth, int textureHeight)
{
	std::vector<std::shared_ptr<Texture>> textures;

	if (textureMap.find(filePath + "_0") != textureMap.end())
	{
		int i = 0;
		std::string key = filePath + "_" + std::to_string(i);

		while (textureMap.find(key) != textureMap.end())
		{
			textures.push_back(textureMap[key]);
			++i;
			key = filePath + "_" + std::to_string(i);
		}

		return textures;
	}

	textures = LoadTextureFromFile(filePath, textureWidth, textureHeight);

	for (int i = 0; i < textures.size(); ++i)
	{
		std::string key = filePath + "_" + std::to_string(i);
		textureMap[key] = textures[i];
	}

	return textures;

}

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& filePath)
{
	if (textureMap.find(filePath) != textureMap.end())
	{
		return textureMap[filePath];
	}
	
	std::shared_ptr<Texture> newSprite = LoadTextureFromFile(filePath);
	textureMap[filePath] = newSprite;
	return newSprite;
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