#include "AssetManager.h"
#include <unordered_map>
#include <iostream> 
#include "ImageLoader.h"

struct AssetManager::Impl
{
    std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> textureMap; 

    // Simple texture loading without size specifications
    std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath)
    {
        if (textureMap.find(filePath) != textureMap.end())
        {
            return textureMap[filePath];
        }

        Texture texture = ImageLoader::LoadTexture(filePath);
        std::vector<std::shared_ptr<Texture>> textures;
        textures.push_back(std::make_shared<Texture>(texture));

        textureMap[filePath] = textures;

        return textures;
    }

    std::vector<std::shared_ptr<Texture>> LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight)
    {
        if (textureMap.find(filePath) != textureMap.end())
        {
            return textureMap[filePath];
        }

        std::vector<Texture> textures = ImageLoader::LoadSpriteSheet(filePath, textureWidth, textureHeight);
        std::vector<std::shared_ptr<Texture>> texturePtrs;
        texturePtrs.reserve(textures.size());

        for (Texture& texture : textures)
        {
            texturePtrs.push_back(std::make_shared<Texture>(texture));
        }

        textureMap[filePath] = texturePtrs;

        return texturePtrs;
    }

    void UnloadAllTextures()
    {
        textureMap.clear();
    }
};

AssetManager::Impl* AssetManager::impl = new Impl();

AssetManager::AssetManager()
{
}

void AssetManager::LoadAll()
{
    std::cout << "All assets loaded." << std::endl;
}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const char* filePath)
{
    return impl->LoadTextureFromFile(filePath); 
}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const char* filePath, int textureWidth, int textureHeight)
{
    return impl->LoadTextureFromFile(filePath, textureWidth, textureHeight); 
}

void AssetManager::LoadSound(const char* soundName, const char* filePath)
{
    std::cout << "Loading sound: " << soundName << " from file: " << filePath << std::endl;
}

void AssetManager::Exit()
{
    delete impl;
}
