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

class DUCKENGINE_API AssetManager
{
public:
	static void LoadAll();

	static std::shared_ptr<Texture> LoadTexture(const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& filePath, int textureWidth, int textureHeight);
	static void LoadSound(const std::string& soundName, const std::string& filePath);
	
	// unload all textures
	static void UnloadAll();

private:
	// Stores all loaded textures with their file paths as keys
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

	// Helper function to load a texture from the file
	static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight);

};