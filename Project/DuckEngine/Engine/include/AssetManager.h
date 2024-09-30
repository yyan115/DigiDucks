#pragma once
#include <vector>
#include <memory>

typedef unsigned int Texture;

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API AssetManager
{
public:
    AssetManager();

    static void LoadAll();

    static std::vector<std::shared_ptr<Texture>> LoadTexture(const char* filePath);
    static std::vector<std::shared_ptr<Texture>> LoadTexture(const char* filePath, int textureWidth, int textureHeight);
    static void LoadSound(const char* soundName, const char* filePath);

    static void Exit();

private:
    struct Impl;
    static Impl* impl;
};
