#pragma once
#include <unordered_map>
#include <string>
#include <memory>

typedef unsigned int Sprite;

class AssetManager
{
public:
	static void LoadAll();

	static std::shared_ptr<Sprite> LoadSprite(const std::string& filePath);
	
	// unload all textures
	static void UnloadAll();

private:
	// Stores all loaded textures with their file paths as keys
	static std::unordered_map<std::string, std::shared_ptr<Sprite>> spriteMap;

	// Helper function to load a texture from the file
	static std::shared_ptr<Sprite> LoadSpriteFromFile(const std::string& filePath);

};