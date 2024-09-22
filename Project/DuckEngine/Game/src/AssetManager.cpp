#include "AssetManager.h"
#include "ImageLoader.h"
#include "Texture.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::textureMap;


void AssetManager::LoadAll()
{
	// load all assets here
	LoadTexture("../Resources/oldman.png");

	// load sound
	LoadSound("TestSound", "../Resources/Sounds/twitchAlert.wav");
	LoadSound("TestSound2", "../Resources/Sounds/magnetic.mp3");

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
	Texture sprite = ImageLoader::LoadTexture(filePath);
	return std::make_shared<Texture>(sprite);

}

void AssetManager::UnloadAll()
{
	textureMap.clear();
}