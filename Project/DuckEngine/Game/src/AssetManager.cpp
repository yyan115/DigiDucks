#include "AssetManager.h"
#include "ImageLoader.h"
#include "Sprite.h"

std::unordered_map<std::string, std::shared_ptr<Sprite>> AssetManager::spriteMap;


void AssetManager::LoadAll()
{
	// load all assets here
	LoadSprite("../Resources/oldman.png");

}

std::shared_ptr<Sprite> AssetManager::LoadSprite(const std::string& filePath)
{
	if (spriteMap.find(filePath) != spriteMap.end())
	{
		return spriteMap[filePath];
	}
	
	std::shared_ptr<Sprite> newSprite = LoadSpriteFromFile(filePath);
	spriteMap[filePath] = newSprite;
	return newSprite;
}


std::shared_ptr<Sprite> AssetManager::LoadSpriteFromFile(const std::string& filePath)
{
	Sprite sprite = ImageLoader::LoadTexture(filePath);
	return std::make_shared<Sprite>(sprite);

}

void AssetManager::UnloadAll()
{
	spriteMap.clear();
}