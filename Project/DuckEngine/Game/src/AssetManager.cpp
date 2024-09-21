#include "AssetManager.h"
#include "ImageLoader.h"
#include "Texture.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::textureMap;


void AssetManager::LoadAll()
{
	// load all assets here
	LoadTexture("../Resources/oldman.png");

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


std::shared_ptr<Texture> AssetManager::LoadTextureFromFile(const std::string& filePath)
{
	Texture sprite = ImageLoader::LoadTexture(filePath);
	return std::make_shared<Texture>(sprite);

}

void AssetManager::UnloadAll()
{
	textureMap.clear();
}